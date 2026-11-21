///=======================================================================================================================
///
/// @file config.h
/// 
/// Configuration file 
/// 
/// @author Eric GENOTELLE
/// @date 2024
///
///=======================================================================================================================
#pragma once

#include "config_secret.h"
// #include "steel.h"


//------------------------------------------------------------------------------------------------
// Version
//------------------------------------------------------------------------------------------------
#define PROGRAMME_VERSION               "2.0a"      // Version of this program
// #define PROGRAMME_VERSION               "0104"      // Version of this program
#define PROGRAMME_DATE                  __DATE__    // Date of this programf
#define PROGRAMME_TIME                  __TIME__    // Time of this program
#define EEPROM_CRC                      3       // checksum for EEPROM. For first run or memory map change

// Select one of instruments
#define HANDPAN                         
// #define TENORBASS                       
// #define DOUBLEGUITAR     
// #define TENOR             

// Select one communication mode
#define I2C
//#define ESPNOW

//------------------------------------------------------------------------------------------------
// For debugging purpose
//------------------------------------------------------------------------------------------------
#define DEBUGAPP                    // Uncomment to output trace 
#define JUST1WAV                    // Uncomment to debug prg : few waves file loaded
#define PAN_WIFI_MODE_STA           //  Uncomment when pan should connect in STATION mode to WIFI box
// #define MIDI_SEND                      // send MIDI on serial
// #define CAPTURE_PIEZZO              // Uncomment to store values in array when hiting a piezzo (parameter = # of piezzo)
// #define TRACE_DELAI_HIT             // to measure hit performance
// #define TRACE_MIDI                  // MIDI functions to debug 
// #define DEBUG_KEYBOARD              // to enter note with keyboard

#ifdef MIDI_SEND
    #undef DEBUGAPP
#endif  // MIDI_SEND


//------------------------------------------------------------------------------------------------
// Define Sound Bank to use. all WAV files bank should referenced in sound.cpp 
//------------------------------------------------------------------------------------------------

#if defined(HANDPAN)
    #define BANKNAME       "HANDPAN"
    #define STEELDRUM_ID   SD_HANDPAN
#elif defined(BANK2)
    #define BANKNAME       "BANK2"
#elif defined(BANK3)
    #define BANKNAME       "BANK3"
#elif defined(BANK3)f
    #define BANKNAME       "BANK4"
#else 
    #define BANKNAME       "HANDPAN"
#endif 


#define APPLI_NAME                  "Master"         //Application Name in web root page
#define APPLI_MIDI_NAME             "[MIDI] DIY Master"
#define PAN_WIFI_AP_SSID            "MASTER"                //SSID prefix in WIFI AP mode
#define PAN_WIFI_AP_PASSWORD        "Pan1234!"              //Password related to SSID prefix in WIFI AP mode
#define PAN_WIFI_STA_SSID           "BoxGnl_5GHz"           //SSID in WIFI STA mode
#define PAN_WIFI_IP                 "10.0.0.1"              //IP address in WIFI STA mode 
#define PAN_WIFI_MASQUE             "255.255.255.0"         //IP mask and gateway address in WIFI STA mode 

#define NB_CARDS_MAX       6
#define NB_NOTES_MAX       8   // per card

#define I2C_FREQ    100000
#define I2C_CARD_ADDR_BASE          0x28     //  0x29, 0x2A... for next cards
#define I2C_MASTER_ADDR             0x40 


#define FILE_MIDI_MAX_SIZE              20000               // Max size of a MIDI file
// #define NOTE_MIDI_BASE      (MIDI_NOTE_C2 + MIDI_NOTE_A)    // first MIDI note of the PAN
#define BPM_DEF                         60          // Default BPM if not in the song
#define CLIC_VELOCITY                   100         // Velocity for 'clic' note

#define NB_PIEZZO                       16          // Nb of piezzo <T_Sound> in <sounds> table
#define NB_SOUNDS                       17          // Nb of sounds <T_Sound> in <sounds> table
#define NB_NOTES_POLY                   9           // Nb of poly <T_Notes_Poly> in <notesPoly> table

#define WAV_BUFFER_SIZE                 512         // of <int> (x2 bytes)
#define EEPROM_SIZE                     512         // EEPROM_SIZE 
#define NB_LOG_PIEZZO                   10          // Nb piezzo hit to log in test mode 


//------------------------------------------------------------------------------------------------
// Default values 
//------------------------------------------------------------------------------------------------
#define DELAY_I2S_DEF                   0           // Delai in ms before sending another I2S packet

#define DELAY_MAX_DOUBLE_NOTES_DEF      200         // Max delay in ms when playing double notes 
#define DELAY_LOOP_LED                  20          // Delay in ms between 2 LEDs loop
#define DELAY_LOOP_VOLUMEGEN            1000        // Delay in ms between 2 VOLUMEGEN  checks
#define DELAY_LOOP_WEBPARAM             2000        // Delay in ms between 2 Web parameter retrieves 
#define DELAY_LOOP_MIDI_MESSAGE         20          // Delay in ms to check MIDI message

#define DELAY_MIDI_MESSAGE              35

#define DELAY_LED_DEMO_DEF              400         // Time in ms LED is ON when displayed for the DEMO
#define DELAY_LED_DEF                   500         // Time in ms LED is ON when playing free
#define DELAY_ILED                      200         // Time in ms internal LED is ON when a note is played

#define BPM_MIN_DEF                     20          // BPM minimum
#define BPM_MAX_DEF                     180         // BPM maximum

#define DELAY_LED_LEARN_DEF             500         // Delay en ms before displaying next LED in LEARN mode 
#define DELAY_LED_TRILLE_DEF            150         // Delay en ms when displaying trille note
#define I2S_BUFFER_SIZE_DEF              64         // I2S buffer size
#define I2S_BUFFER_NB_DEF                 3         // I2S number of buffers

#define PIEZZO_THRESHOLD_DEF            500         // Threshold for piezzo to be considered as hit
#define PIEZZO_VOLUME_MIN_DEF           20          // Volume at PIEZZO_LEVEL_MIN : 0 -> 127
#define PIEZZO_LEVEL_MIN_DEF            400         // When getting analopg value from piezzo, level (0 -> 4095) to consider piezzo with volume min (=PIEZZO_VOLUME_MIN_DEF)
#define PIEZZO_LEVEL_MAX_DEF            2000        // When getting analopg value from piezzo, level (0 -> 4095) to consider piezzo with volume max (=127)
#define DELAY_PIEZZO_MIN_DEF            100         // Delay min in ms between 2 piezzo hits - to avoid rebonce
#define DELAY_PIEZZO_MAX_DEF            300         // Delay max in ms between 2 piezzo hits  // TBD 10 ?


//------------------------------------------------------------------------------------------------------------------------
// PIN configuration
//------------------------------------------------------------------------------------------------------------------------
#define PIN_VOLUME      2           // PIN for volume (potentiometer)

#define PIN_I2S_DATA    4           // PIN for I2S : DATA
#define PIN_I2S_WSK     5           // PIN for I2S : WSK    
#define PIN_I2S_BCK     6           // PIN for I2S : BSK
#define PIN_I2S_MUT     7           // NOT USED

#define PIN_I2C_SDA     8
#define PIN_I2C_SCL     9


