///=======================================================================================================================
///
/// @file device.cpp
/// 
/// Body file for devices management (i2s excluded): LED, piezzo, EEPROM, Keyboard (debug)
/// 
/// @author Eric GENOTELLE
///
///=======================================================================================================================
#include <EEPROM.h>
#include "tools.h"
#include <adafruit_neopixel.h>
#include <WiFi.h>

#ifdef ESPNOW
#include <esp_now.h>
#endif

#ifdef I2C
#include <Wire.h>
#endif

#include "config.h"
#include "global.h"
#include "device.h"
#include "midifile.h"

#include "instru.h"
#include "engine.h"


#ifdef MIDI_SEND
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MidiUsb);
#endif

#define NUMPIXELS 1
#define LED_PIN 48
Adafruit_NeoPixel pixels(NUMPIXELS,LED_PIN,NEO_GRB + NEO_KHZ800);

T_NetCard netCards[ NB_CARDS_MAX ];



// Frame
//   0 : cardId
//   1 : command
//   2 : param
//   3 : value MSB 
//   4 : value LSB    
#define FRAME_LEN           5
#define DATA_CARDID                   0
#define DATA_CMD                      1
#define DATA_PARAM                    2
#define DATA_VALUE_MSB                3
#define DATA_VALUE_LSB                4


// Pad -> Master
#define CMD_PING                      0
#define CMD_CNX                       1
#define CMD_HIT_PAD                   2

// Master -> Pad
#define CMD_SET_LED                 100   
#define CMD_SET_PARAM               101      // See PARAM_*
#define CMD_GO_ADC                  102
#define CMD_SET_THRESHOLD           103

#define PARAM_CARDID                  0
#define PARAM_PIEZZO_VOLUME_MIN       1
#define PARAM_PIEZZO_LEVEL_MIN        2
#define PARAM_PIEZZO_LEVEL_MAX        3  
#define PARAM_DELAY_PIEZZO_MIN        4
#define PARAM_DELAY_PIEZZO_MAX        5
#define PARAM_NB_NOTES                6


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//     Internal LED function
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

void iled_color(uint8_t r, uint8_t g, uint8_t b)
{
  pixels.setPixelColor(0, pixels.Color(r,g,b));  
//  pixels.setBrightness(255);

  pixels.show();
}

void iled_color_blue()
{
  iled_color(0,0,255);
}



void iled_off()
{
  iled_color(0,0,0);
}



//------------------------------------------------------------------------------------------------------------------------
/// @brief Setup all devices
void setupDevices()
{
  initConfig();
  setupEEPROM();
  setupMIDI();
}

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//     EEPROM functions
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
/// @brief  Save a int value `val` EEPROM at address `*paddr`. Increment paddr to the next address to be written
/// @param paddr Pointer to address to save value
/// @param val Value to save
void saveEEPROMInt(int * paddr,int16_t val) 
{
  EEPROM.put( *paddr ,val);
  *paddr = *paddr + sizeof(val);
}


//------------------------------------------------------------------------------------------------------------------------
/// @brief Read a int value `*pval` EEPROM at address `*paddr`. Increment paddr to the next address to be read
/// @param paddr Pointer to address to read value
/// @param pval Pointer to value to read
/// @param min Min value
/// @param max Max value
/// @return false if value has to be corrected regarding min/max
bool readEEPROMInt(int * paddr,int16_t * pval,int16_t min, int16_t max) 
{
  bool ret = true;
  EEPROM.get( *paddr , *pval);
  *paddr = *paddr + sizeof(int16_t);

  if (*pval<min) {
    *pval = min;
    ret = false;
  }
    
  if (*pval>max) {
    *pval = max;
    ret = false;
  }
  return ret;
}

//------------------------------------------------------------------------------------------------------------------------
/// @brief  Save a int value `val` EEPROM at address `*paddr`. Increment paddr to the next address to be written
/// @param paddr Pointer to address to save value
/// @param val Value to save
void saveEEPROMUByte(int * paddr,uint8_t val) 
{
  EEPROM.put( *paddr ,val);
  *paddr = *paddr + sizeof(val);
}


//------------------------------------------------------------------------------------------------------------------------
/// @brief Read a int value `*pval` EEPROM at address `*paddr`. Increment paddr to the next address to be read
/// @param paddr Pointer to address to read value
/// @param pval Pointer to value to read
/// @param min Min value
/// @param max Max value
/// @return false if value has to be corrected regarding min/max
bool readEEPROMUByte(int * paddr,uint8_t * pval,uint8_t min, uint8_t max) 
{
  bool ret = true;
  EEPROM.get( *paddr , *pval);
  *paddr = *paddr + sizeof(uint8_t);

  if (*pval<min) {
    *pval = min;
    ret = false;
  }
    
  if (*pval>max) {
    *pval = max;
    ret = false;
  }
  return ret;
}


//------------------------------------------------------------------------------------------------------------------------
/// @brief setup EEPROM
void setupEEPROM()
{
  // Init EEPROM
  DEBUG2("EEPROM_SIZE:",EEPROM_SIZE);
  bool ret = EEPROM.begin(EEPROM_SIZE) ;
  DEBUG1 ( ret ? "  setup EEPROM OK." : "  setup EEPROM ERROR.");

  readConfigEEPROM();
}


//------------------------------------------------------------------------------------------------------------------------
/// @brief read config (sounds velocity, ...) from EEPROM
void readConfigEEPROM()
{
  int addr=0;
  int16_t crc;
  bool saveEEPROOM=false;
  DEBUG1("readConfigEEPROM()")

  // EEPROM CRC
  readEEPROMInt( &addr , &crc, 0,30000 );
  if (crc != EEPROM_CRC) {
    DEBUG3("EEPROM_CRC Failed : initConfig()...", crc,EEPROM_CRC);
    resetConfigEEPROM();
  } else {

    for (int i=0 ; i<NB_PIEZZO; i++) {
      if (! readEEPROMInt( &addr , &sounds[i].threshold, 10,3000 ) ) {
        saveEEPROOM = true;
      }
    }

    for (int i=0; i<nbParameters; i++) {
      if (! readEEPROMInt( &addr , parameters[i].pValue , parameters[i].min, parameters[i].max ) ) {
        saveEEPROOM = true;
      }
    }
    if (conf.DELAY_I2S != 0) {
      conf.DELAY_I2S = 0;
      conf.I2S_BUFFER_NB = I2S_BUFFER_NB_DEF;
      conf.I2S_BUFFER_SIZE = I2S_BUFFER_SIZE_DEF;
      saveEEPROOM = true;
    }
  }
  if (saveEEPROOM)  saveConfigEEPROM();
}


//------------------------------------------------------------------------------------------------------------------------
/// @brief save config (general volume, sounds velocity, ...) in EEPROM
void saveConfigEEPROM()
{
  int addr=0;
  int16_t crc = EEPROM_CRC;

  DEBUG1("saveConfigEEPROM()")

  // EEPROM CRC
  saveEEPROMInt( &addr , crc );

  for (int i=0 ; i<NB_PIEZZO; i++) {
    saveEEPROMInt( &addr , sounds[i].threshold );
  }

  for (int i=0; i<nbParameters; i++) {
    saveEEPROMInt( &addr , *(parameters[i].pValue) );
  }

  bool ret = EEPROM.commit();
  DEBUG2(" EEPROM.commit()",ret?"OK":"KO");
}

//------------------------------------------------------------------------------------------------------------------------
/// @brief reset config (general volume, sounds velocity, ...) in EEPROM
void resetConfigEEPROM()
{
  DEBUG1("resetConfigEEPROM()")

  initConfig();

  saveConfigEEPROM();
}



//------------------------------------------------------------------------------------------------------------------------
/// @brief Switch ON/OFF a LED  related to a sound
/// @param idxSound  Index of a sound
/// @param on true=To switch ON, false=to switch OFF, 
/// @param hand if on=true , Right or Left . If on=false, switch off both LED
void displayLED(int idxSound, bool on, HandLeftRight hand ) 
{

    // DataPacket packet;
    // packet.cardId = sounds[idxSound].cardId;
    // packet.data1 = sounds[idxSound].noteMidi;

    uint8_t cardId = sounds[idxSound].cardId;
    uint8_t no = sounds[idxSound].no;

    // Define with LED to be displayed
    if (!on)  {
      sendCommSetLED( cardId , no , HandLeft, false );
      sendCommSetLED( cardId , no , HandRight, false );
      sounds[idxSound].ledLeftOn = false;
      sounds[idxSound].ledRightOn = false;
    } else {
      switch (hand) {
        case HandLeft:
          sendCommSetLED( cardId , no , HandLeft, true );
          sendCommSetLED( cardId , no , HandRight, false );
          sounds[idxSound].ledLeftOn = true;
          sounds[idxSound].ledRightOn = false;
          break;
        case HandRight:
          sendCommSetLED( cardId , no , HandLeft, false );
          sendCommSetLED( cardId , no , HandRight, true );
          sounds[idxSound].ledLeftOn = false;
          sounds[idxSound].ledRightOn = true;
          break;
        case HandBoth:
          sendCommSetLED( cardId , no , HandLeft, true );
          sendCommSetLED( cardId , no , HandRight, true );
          sounds[idxSound].ledLeftOn = true;
          sounds[idxSound].ledRightOn = true;
          break;
        default:
          sendCommSetLED( cardId , no , HandLeft, false );
          sendCommSetLED( cardId , no , HandRight, false);
          sounds[idxSound].ledLeftOn = false;
          sounds[idxSound].ledRightOn = false;
      }
    }

    // TBD
    if (on && !song.playing) {
      sounds[idxSound].timeMsStop = millis() + conf.DELAY_LED;
    }

    // Send LED
    // DEBUG3("displayLED",packet.data1,packet.data2);


}



//------------------------------------------------------------------------------------------------------------------------
/// @brief Check if there are some LED to switch ON/OFF
void loopLED() 
{
  static unsigned long timeLastLoop = 0;
  
  // Check if LEDs need to be set/unset
  unsigned long now = millis();
  if ( (now-timeLastLoop)< DELAY_LOOP_LED ) {
    return;
  }
  timeLastLoop = now;

  // For Internal LED
  if ( (timeLastLoop - song.tsiLED) > DELAY_ILED ) {
    iled_off();
  }

  if ( song.playing ) {
    if (!song.modeAuto)  {
      // In playing LEARNING mode, just checck trille mode. In not trille, LED will be checked in touchNote()
      if (song.trille) {
        // change note LED ?
        if ( (song.trilleNoteLastTime + conf.DELAY_LED_TRILLE) < now) {
          song.trilleNoteLastTime = now;
          if ( song.trilleNoteCurrent==song.trilleNote1 ) {
            song.trilleNoteCurrent = song.trilleNote2;
            displayLED(song.trilleNote1, false, song.trilleNote1Hand);
            displayLED(song.trilleNote2, true, song.trilleNote2Hand);
          } else {
            song.trilleNoteCurrent = song.trilleNote1;
            displayLED(song.trilleNote1, true, song.trilleNote1Hand);
            displayLED(song.trilleNote2, false, song.trilleNote2Hand);
          }
        }
      }
      return;
    } else {
      // In playing AUTO mode, switch OFF all leds if time's up
      for (int i=0; i<NB_PIEZZO; i++) {
        if ( sounds[i].timeMsStop < millis() ) {
            displayLED(i, false, HandBoth);
          }
      }
      return;  
    }
  } else {
    // Playing free
    for (int i=0; i<NB_PIEZZO; i++) {
      if (  ( (sounds[i].ledRightOn) || (sounds[i].ledLeftOn)) && 
            (sounds[i].timeMsStop < millis() ) ) {
           displayLED(i, false, HandBoth);
//         if (!conf.testMode) {
//           displayLED(i, false, HandBoth);
//         } else {
//           // in test mode, if left LED is ON, turn OFF left and ON right
//           if ( (sounds[i].timeMsStop < millis()) && (sounds[i].ledLeftOn) ) {
//             displayLED(i, false, HandLeft);
//             displayLED(i, true, HandRight);
// //            sounds[i].timeMsStop = millis() + conf.DELAY_LED;
//           }
//           if ( (sounds[i].timeMsStop < millis()) && (sounds[i].ledRightOn) ) {
//             displayLED(i, false, HandRight);
//           }
//         }
      }
    }
  }
}


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//     TOUCH functions 
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
///  @brief Play the sound.  
///        If not playing a MIDI  song : display LED, If playing a MIDI song in mode "LEARN" display the LED
/// @param idxSound index of the sound to play
/// @param velocity velocity (0 -> 127 ) of the sound to play
void touchNote(int idxSound, int velocity) 
{
  // -------------------------------------------------------
  // Play the sound
  // -------------------------------------------------------
  playSound(idxSound,velocity);
  int noteMidi = sounds[idxSound].noteMidi;


  // -------------------------------------------------------
  // Send MIDI note on Serial
  // -------------------------------------------------------
#ifdef MIDI_SEND
  DEBUG2("MIDI.sendNoteOn",noteMidi)
  MidiUsb.sendNoteOn(noteMidi,velocity,1);
  sounds[idxSound].lastMidiMessage = millis();
#endif

  // -------------------------------------------------------
  // Display internal LED
  // -------------------------------------------------------
  iled_color_blue();
  song.tsiLED = millis();

  // -------------------------------------------------------
  // if playing free
  // -------------------------------------------------------
  if ( ! song.playing ) {
    displayLED(idxSound,true,HandBoth);
    return;
  }

  // -------------------------------------------------------
  // mode SEQUENCE : do nothing
  // -------------------------------------------------------
  if ( song.modeAuto ) {
    return;
  }

  // -------------------------------------------------------
  // mode LEARN
  // -------------------------------------------------------
  // User has played the right note ?
  if ( (song.lNotes[song.noteCur].state == ToBePlayed) && (song.lNotes[song.noteCur].noteMidi ==  noteMidi) ) {
    // switch OFF the LED of the note if not double notes
    if (!song.doubleNotes) {
      displayLED( song.lNotes[song.noteCur].idxSound,false,HandBoth );
    }
    DEBUG2("  Played",song.noteCur);
    song.lNotes[song.noteCur].state = Played;
    song.lNotes[song.noteCur].lastPlayed = millis();
  }

  // check if note or the 2 simultaneous notes has/have being played
  if (song.doubleNotes) {

    if ( (song.lNotes[song.noteCur+1].state == ToBePlayed) && (song.lNotes[song.noteCur+1].noteMidi == noteMidi ) ){
      // switch OFF the LED of the note
      song.lNotes[song.noteCur+1].state = Played;
      song.lNotes[song.noteCur+1].lastPlayed = millis();
      DEBUG2("  Played n2",song.noteCur+1);
    } 

    if ( (song.lNotes[song.noteCur].state == Played) && (song.lNotes[song.noteCur+1].state == Played) ) {
      DEBUG3("2 notes played",song.noteCur,song.noteCur+1);
      // check if the 2 notes are played in a short amount of time
      long delta = song.lNotes[song.noteCur+1].lastPlayed - song.lNotes[song.noteCur].lastPlayed;
      if (delta<0) 
        delta = - delta;
      if ( delta < conf.DELAY_MAX_DOUBLE_NOTES ) {
        DEBUG1("OK for the 2 notes !")
        displayLED( song.lNotes[song.noteCur].idxSound,false,HandBoth );
        displayLED( song.lNotes[song.noteCur+1].idxSound,false,HandBoth );
        song.noteCur += 2;
      } else {
        // too slow : 2 notes to be replayed
        DEBUG1("too slow for the 2 notes : replayed!")
        song.lNotes[song.noteCur].state = ToBePlayed;
        song.lNotes[song.noteCur+1].state = ToBePlayed;
        song.lNotes[song.noteCur].lastPlayed = 0;
        song.lNotes[song.noteCur+1].lastPlayed = 0;
      }
    } 
  } else {
    if (song.lNotes[song.noteCur].state == Played) {
      song.noteCur ++;
    }
  }
}



//------------------------------------------------------------------------------------------------------------------------
/// @brief same as <touchNote()> but with MIDI note in input 
/// @param noteMidi  MIDI note
/// @param velocity  velocity of note to play
void touchNoteMidi(int noteMidi, int velocity) 
{
  int idxSound = getSoundIndexByNoteMidi(noteMidi);
  touchNote(idxSound, velocity);
}



//=======================================================================================================================
//
//  Functions for piezzo management
//
//=======================================================================================================================



// #define PIEZZO_NB_SCAN_MAX  50           // Nb of max scan to perform to read a piezzo
// #define PIEZZO_SCAN_DELAI   250  

//------------------------------------------------------------------------------------------------------------------------
// /// @brief check if a note has to be played: check piezzo queue. executed by main thread
// void loopPlayPiezzo()
// {
//   T_Note note;
//   if (xQueueReceive(queuePiezzo, &note, 0) == pdPASS) {
//     DEBUG3("** xQueueReceiv",note.note, note.velocity);
// //DEBUG2("** loopPlayPiezzo;",millis());
//     touchNote(note.note, note.velocity);
//   }
// }

//------------------------------------------------------------------------------------------------------------------------
/// @brief check if MIDI note should be NOTE OFF
void loopMidiMessage()
{
#ifdef MIDI_SEND
  static unsigned long lastLoopMidiMessage=0;
  unsigned long now = millis();

  MidiUsb.read();

  if ( (lastLoopMidiMessage+DELAY_LOOP_MIDI_MESSAGE) < now ) {
    lastLoopMidiMessage = now;
    for (int i=0; i<NB_SOUNDS; i++) {
      if ( (sounds[i].lastMidiMessage>0) && ((sounds[i].lastMidiMessage+DELAY_MIDI_MESSAGE) < now) ){
        sounds[i].lastMidiMessage = 0;
        DEBUG2("sendNoteOff",sounds[i].noteMidi);
        MidiUsb.sendNoteOff(sounds[i].noteMidi,0,1);
      }
    }
  }
#endif
}


//------------------------------------------------------------------------------------------------------------------------
/// @brief Get general volume 
void loopVolumeGen()
{
  static unsigned long timeLastVolume=0;
  unsigned long now = millis();
  if ( (timeLastVolume+DELAY_LOOP_VOLUMEGEN) < now ) {
    timeLastVolume = now;

    if (conf.POTENTIOMETER==1) {
      uint16_t val = analogRead(PIN_VOLUME);
      conf.volumeGen = map(val,0,4095,0,100);   
// DEBUG2("Volume",conf.volumeGen);
    } else {
      conf.volumeGen = 100;     
    }
    
  }
}


//------------------------------------------------------------------------------------------------------------------------
/// @brief Initialize / reset general config parameters
void initConfig()
{
  conf.volumeGen = 100;
  conf.testMode = false;
  
  for (int i=0; i<nbParameters; i++) {
    *(parameters[i].pValue) = parameters[i].def;
  }
}


void handleMIDINoteOn(byte channel, byte noteMidi, byte velocity)
{
#ifdef MIDI_SEND
  DEBUG3("handleMIDINoteOn",noteMidi,velocity);
  int idxSound = getSoundIndexByNoteMidi(noteMidi);
  if ( idxSound == NOTE_ERROR ) {
    return;
  }
  playSound(idxSound,velocity);
  HandLeftRight h =  getHandByVelocity( velocity);  
  displayLED(idxSound, true, h);
#endif
}


void handleMIDINoteOff(byte channel, byte noteMidi, byte velocity)
{
#ifdef MIDI_SEND
  DEBUG3("handleMIDINoteOff",noteMidi,velocity);
  int idxSound = getSoundIndexByNoteMidi(noteMidi);
  if ( idxSound == NOTE_ERROR ) {
    return;
  }
  displayLED(idxSound, false, HandBoth);
#endif
}




void setupMIDI()
{
#ifdef MIDI_SEND
  DEBUG1("setupMIDI()...")

  TinyUSBDevice.setManufacturerDescriptor("PIO");
  TinyUSBDevice.setProductDescriptor(APPLI_MIDI_NAME);
  
  MidiUsb.setHandleNoteOn(handleMIDINoteOn);  
  MidiUsb.setHandleNoteOff(handleMIDINoteOff);
  MidiUsb.begin(MIDI_CHANNEL_OMNI);
#endif
}


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//     COMMUNICATION functions
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------


#ifdef ESPNOW
bool isNewCardConnection(uint8_t cardId,const uint8_t *adr_recv)
{
  for (int i=0; i<conf.nbCards; i++) {
    if ( netCards[i].cardId == cardId ) {
      // already referenced
      return false;
    }
  }

  DEBUG2("New card",cardId);

  netCards[ conf.nbCards ].cardId = cardId;
  for (int i=0; i<conf.instru->nbBowls; i++) {
    if ( conf.instru->cards[i]->id == cardId ) {
      netCards[ conf.nbCards ].card = conf.instru->cards[i];
      i = conf.instru->nbBowls;
    }
  }

  memcpy(netCards[ conf.nbCards ].mac, adr_recv, 6);

  // Register card peer to send info
  memcpy(netCards[ conf.nbCards ].peerInfo.peer_addr, adr_recv, 6);
  netCards[ conf.nbCards ].peerInfo.channel = 0;
  netCards[ conf.nbCards ].peerInfo.encrypt = false;
  
  c_add_peer(& netCards[ conf.nbBowls ].peerInfo);


  // Register CardPeer in sounds table
  for (int i=0; i<NB_SOUNDS; i++) {
    if ( sounds[i].cardId == cardId ) {
      sounds[i].netCard = & netCards[ conf.nbCards ];
    }
  }

  conf.nbCards++;

  // Check if all cards are connected
  if (conf.nbCards == conf.instru->nbBowls) {
    DEBUG1("all cards sonnected");
    conf.allCardsConnected = true;
  }

  return true;
}

#endif


#ifdef ESPNOW
//------------------------------------------------------------------------------------------------------------------------
/// @brief Callback function when receiving packet from MASTER
/// @param adr_recv 
/// @param incomingData 
/// @param len 
void OnDataRecv(const uint8_t *adr_recv,
                const uint8_t *incomingData,
                int len)
{
  DataPacket packet;
  memcpy(&packet, incomingData, sizeof(packet));

  if (!conf.allCardsConnected) {
    isNewCardConnection( packet.cardId, adr_recv );
  }

  DEBUG4("MASTER OnDataRecv", packet.cardId, packet.data1, packet.data2);
  // conf.lastReceived = millis();

  // Set lastReceived
  for (int i=0; i<conf.nbBowls; i++) {
    if ( netCards[i].cardId =  packet.cardId ) {
      netCards[i].lastReceived = millis();
      i = conf.nbBowls;
    }
  }

  // Note ?
  if (packet.data1<128) {
    touchNoteMidi(packet.data1,packet.data2);
    return;
  }
  
  // PING response
  if ( packet.data1 == 255 ) {
    DEBUG2("PING Response to",packet.cardId)
    esp_now_send(adr_recv,
            (uint8_t *)&packet,
            sizeof(packet));
    return;
  }

}
#endif // ESPNOW



//------------------------------------------------------------------------------------------------------------------------
/// @brief Setup communication with MASTER
void setupComm()
{

  DEBUG1("setupComm()...");

#ifdef I2C
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, I2C_FREQ);
    Wire.setTimeOut(50);                    // timeout en ms
#endif

for (int cardId=0; cardId<  instru.nbCards; cardId++) {
  netCards[cardId].i2cSlaveAddr = (int)I2C_CARD_ADDR_BASE + cardId;
  netCards[cardId].lastReceived = 0;  
  netCards[cardId].lastSent = 0;  
  netCards[cardId].connected = false;
}

#ifdef ESPNOW
  WiFi.mode(WIFI_AP_STA);

  if (esp_now_init() != ESP_OK)   {
      DEBUG1("esp_now_init() error");
      return;
  }
  DEBUG1("  esp_now_init() OK");

  Serial.print("MAC : ");
  Serial.println(WiFi.macAddress());
 
  esp_now_register_recv_cb(OnDataRecv);  
#endif



}

void sendCommValue(int8_t cardId, int8_t cmd, int8_t param, uint16_t value)
{
    uint8_t data[FRAME_LEN];
    uint32_t written = 0;

  DEBUG6("sendCommValue (addr,id,cmd,param,value)",netCards[cardId].i2cSlaveAddr,cardId,cmd,param,value);
  netCards[cardId].lastSent = millis();

#ifdef I2C
    data[DATA_CARDID] = cardId;
    data[DATA_CMD] = cmd;
    data[DATA_PARAM] = param;
    data[DATA_VALUE_MSB] = (value >> 8) & 0xFF;
    data[DATA_VALUE_LSB] = value & 0xFF;

    Wire.beginTransmission( netCards[cardId].i2cSlaveAddr ); 
    Wire.write(data, FRAME_LEN);
    Wire.endTransmission();
#endif
}

void sendCommSetLED(uint8_t cardId , uint8_t no, HandLeftRight hand, bool on ) 
{
  int8_t offset = (hand == HandRight) ? 1 : 0 ;
  if ( netCards[cardId].connected) {
    sendCommValue( cardId, CMD_SET_LED, (no * 2) + offset, on ? 1 : 0);
  }
}

void sendCommGoADC(uint8_t cardId , bool on)
{
  sendCommValue( cardId, CMD_GO_ADC, 0, on ? 1 : 0);
}

void sendCommThreshold(uint8_t cardId, uint8_t no, uint16_t threshold)
{
  sendCommValue(cardId, CMD_SET_THRESHOLD, no, threshold);
}


void recvCommPing(uint8_t cardId, uint8_t param, uint16_t value)
{
  DEBUG4("recv CMD_PING from card", cardId, param, value);
  sendCommValue( cardId, CMD_PING, param, value);
}

void recvCommCnx(uint8_t cardId, uint8_t param, uint16_t value)
{
  DEBUG4("recv CMD_CNX from card", cardId, param, value);
  checkNewCard( cardId ) ;
  sendCardConfig( cardId );
  sendCommGoADC( cardId , true );
}


void recvCommHitPad(uint8_t cardId, uint8_t param, uint16_t value)
{
  DEBUG4("recv CMD_HIT_PAD from card", cardId, param ,value);
  touchNoteMidi( getMidiNoteByCardId(cardId, param) , value); 
}



void loopComm()
{

static unsigned long lastcommLoop = 0; 
static uint16_t cnt = 0;
  if ( (millis() - lastcommLoop) < 2000 ) {
      return;
  }
  lastcommLoop = millis();
  sendCommValue( 0, CMD_PING, 0, cnt);
  cnt++;
  return;

#ifdef I2C
  uint8_t    data[FRAME_LEN];

  for (int i=0; i<instru.nbCards; i++) {
    int i2cCardAddr =  (int)I2C_CARD_ADDR_BASE + i;
unsigned long now = millis();
    uint8_t nbRead = Wire.requestFrom( i2cCardAddr , FRAME_LEN);
DEBUG3("requesfrom (nb, delai)",nbRead, (millis()-now));
    if ( nbRead == FRAME_LEN ) {
      for (int i = 0; i < FRAME_LEN; i++) {
        data[i] = Wire.read();
      }
      uint8_t cmd = data[DATA_CMD];
      uint8_t cardId = data[DATA_CARDID];
      uint8_t param = data[DATA_PARAM];
      uint16_t value = (data[DATA_VALUE_MSB] << 8) | data[DATA_VALUE_LSB];

      if ( cardId != i ) {
        DEBUG3("ERREUR: CardId does not match i2cCardAddr",cardId,i)
      }
      netCards[cardId].lastReceived = millis();

      switch ( cmd ) 
      {
        case CMD_PING:
          recvCommPing( cardId, param, value );
          break;

        case CMD_CNX:
          recvCommCnx( cardId, param, value );
          break;

        case CMD_HIT_PAD:
          recvCommHitPad( cardId, param, value );
          break;

        default:
          DEBUG3("recv Unknown command from card", cardId, cmd );
      }
    }
  }
#endif
}

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//     CARD functions
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

int getMidiNoteByCardId(uint8_t cardId, uint8_t no)
{
  if ( ! checkCardId(cardId) ) 
    return 0;
  if (no >= NB_NOTES_MAX) 
    return 0;

  return instru.cards[cardId]->notes[ no ];
}



/// --------------------------------------------------------------
/// @brief Send config of the card to the card
/// @param cardId 
/// @return 
void sendCardConfig(uint8_t cardId)
{
  sendCommValue(cardId, CMD_SET_PARAM, PARAM_PIEZZO_VOLUME_MIN, conf.PIEZZO_VOLUME_MIN);
  sendCommValue(cardId, CMD_SET_PARAM, PARAM_PIEZZO_LEVEL_MIN, conf.PIEZZO_LEVEL_MIN);
  sendCommValue(cardId, CMD_SET_PARAM, PARAM_PIEZZO_LEVEL_MAX, conf.PIEZZO_LEVEL_MAX);
  sendCommValue(cardId, CMD_SET_PARAM, PARAM_DELAY_PIEZZO_MIN, conf.DELAY_PIEZZO_MIN);
  sendCommValue(cardId, CMD_SET_PARAM, PARAM_DELAY_PIEZZO_MAX, conf.DELAY_PIEZZO_MAX);
  sendCommValue(cardId, CMD_SET_PARAM, PARAM_NB_NOTES, instru.cards[cardId]->nbNotes);
  sendCommValue(cardId, CMD_SET_PARAM, PARAM_CARDID, cardId);

  for (uint8_t i=0; i<instru.cards[cardId]->nbNotes; i++) {
    sendCommThreshold(cardId, i, sounds [ getIndexSoundByCard(cardId, i) ].threshold );
  }
}


bool checkNewCard(uint8_t cardId)
{
  if ( netCards[cardId].connected == false ) {
    DEBUG2("New card",cardId);

    netCards[cardId].connected = true;
    return true;
  }
  return false;
}

bool checkCardId(uint8_t id)
{
  if (id>=NB_CARDS_MAX)
    return false;
  if ( instru.cards[id] == NULL ) 
    return false;
  return true;
}


T_Card * getCardByID(uint8_t id) 
{
  return instru.cards[id];
}

uint8_t getIndexSoundByCard(uint8_t cardId, uint8_t no)
{
  if ( ! checkCardId(cardId) ) 
    return 0;
  if (no >= NB_NOTES_MAX) 
    return 0;

  return netCards[cardId].idxSounds[no];
}

uint8_t getParamIdByName(const char * name)
{
  if ( strcmp(name, "PIEZZO_VOLUME_MIN") == 0 ) {
    return PARAM_PIEZZO_VOLUME_MIN;
  }
  if ( strcmp(name, "PIEZZO_LEVEL_MIN") == 0 ) {
    return PARAM_PIEZZO_LEVEL_MIN;
  }
  if ( strcmp(name, "PIEZZO_LEVEL_MAX") == 0 ) {
    return PARAM_PIEZZO_LEVEL_MAX;
  }
  if ( strcmp(name, "DELAY_PIEZZO_MIN") == 0 ) {
    return PARAM_DELAY_PIEZZO_MIN;
  }
  if ( strcmp(name, "DELAY_PIEZZO_MAX") == 0 ) {
    return PARAM_DELAY_PIEZZO_MAX;
  }
  return 255;
}


void sendAllCardsParam(const char * paramName, int16_t value)
{
  bool cont = true;

  uint8_t paramId = getParamIdByName(paramName);
  if ( paramId == 255 ) {
    DEBUG1("sendCardsParam() : unknown paramName");
    return;
  }

  for (int cardId=0; cardId < conf.nbCards; cardId++) {
    sendCommValue(cardId, CMD_SET_PARAM, paramId, value);
  }
}