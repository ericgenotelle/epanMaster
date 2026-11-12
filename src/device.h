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

struct DataPacket
{
  uint8_t bidonId;
  uint8_t data1;
  uint8_t data2;
};

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
void sendCommValue(int8_t bowlId, int8_t cmd, int8_t param, uint16_t value);
void loopComm();
