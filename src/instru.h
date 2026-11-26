#pragma once

#include "arduino.h"
#include "config.h"

// Liste des instruments
#define SD_HANDPAN          0
#define SD_TENORBASS        1
#define SD_DOUBLEGUITAR     2
#define SD_TENOR            3


// Instru HANDPAN
#define SD_HANDPAN1         0
#define SD_HANDPAN2         1


// Instru TENOR BASS
#define SD_TENORBASS1       0
#define SD_TENORBASS2       1
#define SD_TENORBASS3       2
#define SD_TENORBASS4       3

// Instru DOUBLE GUITARE
#define SD_DOUBLEGUITAR1A   0
#define SD_DOUBLEGUITAR1B   1
#define SD_DOUBLEGUITAR2A   2
#define SD_DOUBLEGUITAR2B   3


// #define SD_TENOR1           0
// #define SD_TENOR2           1

// #define SD_BASS             0
// #define SD_BASS1            1
// #define SD_BASS2            2
// #define SD_BASS3            3


typedef struct 
{
  uint8_t             id;
  String              nom;
  uint8_t             nbNotes;
  uint8_t             notesMidi[NB_NOTES_MAX];

  uint8_t             idxSounds [NB_NOTES_MAX];
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

} T_Card;


typedef struct 
{
  uint8_t id;
  String nom;
  uint8_t nbCards;
  T_Card * cards[NB_CARDS_MAX];
} T_Instru;

