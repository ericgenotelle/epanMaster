#pragma once

#include "arduino.h"

#define NB_BIDONS_MAX       6
#define NB_PIEZZO_MAX      16  // per bidon

// Liste des instruments
#define SD_TENORBASS        0
#define SD_DOUBLEGUITAR     1
#define SD_TENOR            2
#define SD_HANDPAN          9

// Listes des bidons TENOR BASS
#define SD_TENORBASS1       0
#define SD_TENORBASS2       1
#define SD_TENORBASS3       2
#define SD_TENORBASS4       3

#define SD_DOUBLEGUITAR1    4
#define SD_DOUBLEGUITAR2    5

#define SD_TENOR1           6
#define SD_TENOR2           7

#define SD_BASS             10
#define SD_BASS1            11
#define SD_BASS2            12
#define SD_BASS3            13

#define SD_HANDPAN1         99


typedef struct 
{
  uint8_t id;
  String nom;
  uint8_t nbNotes;
  uint8_t notes[NB_PIEZZO_MAX];
} T_Bidon;


typedef struct 
{
  uint8_t id;
  String nom;
  uint8_t nbBidons;
  T_Bidon * bidons[NB_BIDONS_MAX];
} T_Instru;


T_Bidon * getBidonByID(uint8_t id);