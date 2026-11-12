//=======================================================================================================================
///
/// @file main.cpp
/// 
/// Main programm file
/// 
/// @author Eric GENOTELLE
/// @date 2024
/// 
///=======================================================================================================================

//------------------------------------------------------------------------------------------------------------------------
// Includes
#include "arduino.h"
#include "driver/i2s.h"                       // Library of I2S routines, comes with ESP32 standard install
#include "tools.h"

#include "config.h"
#include "sound.h"
#include "web.h"
#include "device.h"
#include "global.h"



//=====================================================================2104===============
/// @brief  SETUP for ESP32
void setup() 
{
#ifdef DEBUGAPP  
  Serial.begin(115200);     
  while (! Serial);
#endif
  delay(300);

  DEBUG1("SETUP...");
  setupDevices();
  setupSounds();
  setupComm();
  resetAllNotesPoly();
  setupSong();
  setupWeb();

  DEBUG2("Nb parameters", nbParameters);

  DEBUG2("CORE ID:",xPortGetCoreID());

  // Display GREEN internal LED when ready
  iled_color(false,true,false);
  delay(400);
  iled_off();

  DEBUG1("setup OK.");

}


//====================================================================================
///
/// @brief  LOOP for ESP32
void loop() 
{
//  loopPlayPiezzo();
  loopVolumeGen();
  loopSong();
  loopSong(); // recheck for double note
  loopLED();
  loopPlayWaves();
  loopWeb();
  loopComm();
#ifdef MIDI_SEND
  loopMidiMessage();
#endif
}
