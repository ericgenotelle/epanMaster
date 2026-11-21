///=======================================================================================================================
///
/// @file device.h
/// 
/// Header file for devices management (i2s excluded): LED, piezzo, EEPROM, Keyboard (debug)
/// 
/// @author Eric GENOTELLE
/// @date 2024
///
///=======================================================================================================================

#pragma once

#include "sound.h"

// struct DataPacket
// {
//   uint8_t cardId;
//   uint8_t data1;
//   uint8_t data2;
// };


typedef struct 
{
    uint8_t             idxSounds [NB_NOTES_MAX];
    uint8_t             ledLeftOn[NB_NOTES_MAX];
    uint8_t             ledRightOn[NB_NOTES_MAX];

    bool                connected = false;
    unsigned long       lastReceived=0;
    unsigned long       lastSent=0;
#ifdef ESPNOW
    uint8_t             mac[6];
    esp_now_peer_info_t peerInfo = {};
#endif
#ifdef I2C
    uint8_t             i2cSlaveAddr;
#endif
} T_NetCard;

void setupDevices();

void setupEEPROM();
void readConfigEEPROM();
void saveConfigEEPROM();
void resetConfigEEPROM();

void displayLED(int idxSound, bool on, HandLeftRight hand) ;
void loopLED();

void iled_color(uint8_t r, uint8_t g, uint8_t b);
void iled_off();

void touchNote(int noteMidi, int velocity );

void setupMIDI();

#ifdef DEBUG_KEYBOARD
void loopKeyboard();
#endif

void loopPlayPiezzo();
void loopMidiMessage();
void loopVolumeGen();

void initConfig();

void setupComm();
void sendCommValue(int8_t cardId, int8_t cmd, int8_t param, uint16_t value);
void sendCommSetLED(uint8_t cardId, uint8_t no, HandLeftRight hand, bool on);
void sendCommGoADC(uint8_t cardId);
void loopComm();

int getMidiNoteByCardId(uint8_t cardId, uint8_t no);
void sendCommThreshold(uint8_t cardId, uint8_t no, uint16_t threshold);
void sendCardConfig(uint8_t cardId);
bool checkNewCard(uint8_t cardId);
bool checkCardId(uint8_t id);
void sendAllCardsParam(const char *paramName, int16_t value);
