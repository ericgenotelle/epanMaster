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

#include "device.h"
#include "config.h"
#include "global.h"
#include "midifile.h"

#include "steel.h"


#ifdef MIDI_SEND
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MidiUsb);
#endif

#define NUMPIXELS 1
#define LED_PIN 48
Adafruit_NeoPixel pixels(NUMPIXELS,LED_PIN,NEO_GRB + NEO_KHZ800);

T_BidonPeer bidonsPeer[NB_BIDONS_MAX];



// Frame
//   0 : bowlId
//   1 : command
//   2 : param
//   3 : value MSB 
//   4 : value LSB    
#define FRAME_LEN           5
#define DATA_BOWLID                   0
#define DATA_CMD                      1
#define DATA_PARAM                    2
#define DATA_VALUE_MSB                3
#define DATA_VALUE_LSB                4


// Pad -> Master
#define CMD_PING                  1
#define CMD_HIT_PAD               2

// Master -> Pad
#define CMD_SET_LED             100   
#define CMD_SET_PARAM           101
#define CMD_GO_ADC              102
#define CMD_SET_THRESHOLD       103


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
void displayLED(int idxSound, bool on, HandLeftRight hand) 
{

    DataPacket packet;
    packet.bidonId = sounds[idxSound].bidonId;
    packet.data1 = sounds[idxSound].noteMidi;

    // Define with LED to be displayed
    if (!on)  {
      packet.data2 = 0b00;
      sounds[idxSound].ledRightOn = false;
      sounds[idxSound].ledLeftOn = false;
    } else {
      switch (hand) {
        case HandLeft:
          packet.data2 = 0b10;
          sounds[idxSound].ledRightOn = false;
          sounds[idxSound].ledLeftOn = true;
          break;
        case HandRight:
          packet.data2 = 0b01;
          sounds[idxSound].ledRightOn = true;
          sounds[idxSound].ledLeftOn = false;
          break;
        case HandBoth:
          packet.data2 = 0b11;
          sounds[idxSound].ledRightOn = true;
          sounds[idxSound].ledLeftOn = true;
          break;
        default:
          sounds[idxSound].ledRightOn = false;
          sounds[idxSound].ledLeftOn = false;
          packet.data2 = 0b00;
      }
    }

    if (on && !song.playing) {
      sounds[idxSound].timeMsStop = millis() + conf.DELAY_LED;
    }

    // Send LED
    DEBUG3("displayLED",packet.data1,packet.data2);

    #ifdef ESPNOW    
    esp_now_send(sounds[idxSound].bidonPeer->mac,
                (uint8_t *)&packet,
                sizeof(packet));
#endif

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
  int idxSound = getSoundIndexByNote(noteMidi);
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
  int idxSound = getSoundIndexByNote(noteMidi);
  if ( idxSound == NOTE_ERROR ) {
    return;
  }
  playSound(idxSound,velocity);
  HandLeftRight h =  getHandVelocity( velocity);  
  displayLED(idxSound, true, h);
#endif
}


void handleMIDINoteOff(byte channel, byte noteMidi, byte velocity)
{
#ifdef MIDI_SEND
  DEBUG3("handleMIDINoteOff",noteMidi,velocity);
  int idxSound = getSoundIndexByNote(noteMidi);
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


//=======================================================================================================================
//
//  Functions for communication ESP NOW
//
//=======================================================================================================================

bool isNewBidonConnection(uint8_t bidonId,const uint8_t *adr_recv)
{
  for (int i=0; i<conf.nbBidons; i++) {
    if ( bidonsPeer[i].bidonId == bidonId ) {
      // already referenced
      return false;
    }
  }

  DEBUG2("New bidon",bidonId);

  bidonsPeer[ conf.nbBidons ].bidonId = bidonId;
  for (int i=0; i<conf.instru->nbBidons; i++) {
    if ( conf.instru->bidons[i]->id == bidonId ) {
      bidonsPeer[ conf.nbBidons ].bidon = conf.instru->bidons[i];
      i = conf.instru->nbBidons;
    }
  }

  memcpy(bidonsPeer[ conf.nbBidons ].mac, adr_recv, 6);

  // Register bidon peer to send info
  memcpy(bidonsPeer[ conf.nbBidons ].peerInfo.peer_addr, adr_recv, 6);
  bidonsPeer[ conf.nbBidons ].peerInfo.channel = 0;
  bidonsPeer[ conf.nbBidons ].peerInfo.encrypt = false;
  
  #ifdef ESPNOW
  c_add_peer(& bidonsPeer[ conf.nbBidons ].peerInfo);
  #endif


  // Register BidonPeer in sounds table
  for (int i=0; i<NB_SOUNDS; i++) {
    if ( sounds[i].bidonId == bidonId ) {
      sounds[i].bidonPeer = & bidonsPeer[ conf.nbBidons ];
    }
  }

  conf.nbBidons++;

  // Check if all bidons are connected
  if (conf.nbBidons == conf.instru->nbBidons) {
    DEBUG1("allBidonConnected");
    conf.allBidonConnected = true;
  }

  return true;
}



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

  if (!conf.allBidonConnected) {
    isNewBidonConnection( packet.bidonId, adr_recv );
  }

  DEBUG4("MASTER OnDataRecv", packet.bidonId, packet.data1, packet.data2);
  // conf.lastReceived = millis();

  // Set lastReceived
  for (int i=0; i<conf.nbBidons; i++) {
    if ( bidonsPeer[i].bidonId =  packet.bidonId ) {
      bidonsPeer[i].lastReceived = millis();
      i = conf.nbBidons;
    }
  }

  // Note ?
  if (packet.data1<128) {
    touchNoteMidi(packet.data1,packet.data2);
    return;
  }
  
  // PING response
  if ( packet.data1 == 255 ) {
    DEBUG2("PING Response to",packet.bidonId)
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

#ifdef I2C
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, I2C_FREQ);
    Wire.setTimeOut(50);                    // timeout en ms
#endif

}


void sendCommValue(int8_t bowlId, int8_t cmd, int8_t param, uint16_t value)
{
    uint8_t data[FRAME_LEN];
    uint32_t written = 0;

    data[DATA_BOWLID] = bowlId;
    data[DATA_CMD] = cmd;
    data[DATA_PARAM] = param;
    data[DATA_VALUE_MSB] = (value >> 8) & 0xFF;
    data[DATA_VALUE_LSB] = value & 0xFF;

    Wire.beginTransmission( I2C_SLAVE_ADDR ); // TBD
    Wire.write(data, FRAME_LEN);
    Wire.endTransmission();
}
    

void loopComm()
{
#ifdef I2C
  uint8_t    data[FRAME_LEN];

  if (Wire.requestFrom((int)I2C_SLAVE_ADDR, FRAME_LEN) != FRAME_LEN) {
      return ;
  }
  for (int i = 0; i < FRAME_LEN; i++) {
    data[i] = Wire.read();
  }
  uint16_t value = (data[DATA_VALUE_MSB] << 8) | data[DATA_VALUE_LSB];
  
  switch (data[DATA_CMD]) {

    case CMD_PING:
      DEBUG2("CMD_PING from bowl",data[DATA_BOWLID]);
      sendCommValue(data[DATA_BOWLID], CMD_PING, 0, 0);
      break;

      case CMD_HIT_PAD:
      DEBUG4("CMD_HIT_PAD from bowl",data[DATA_BOWLID],data[DATA_PARAM],value);
//      touchNoteMidi(data[DATA_PARAM],value);
      break;
    default:
      DEBUG3("Unknown command from bowl",data[DATA_BOWLID],data[DATA_CMD]);
  }


#endif
}
