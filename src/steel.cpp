///=======================================================================================================================
///
/// @file steel.cpp
/// 
// 
/// @author Eric GENOTELLE
/// @date 2026
///
///=======================================================================================================================

#include "tools.h"

#include "midiinfo.h"
#include  "steel.h"


// ---------------------------------------------------------------------------------
// TENOR BASS
// ---------------------------------------------------------------------------------

T_Bidon sdTenorBass1 = 
{
  SD_TENORBASS1, "Tenor Bass 1" , 5, 
  { 
    MIDI_NOTE_C1 + MIDI_NOTE_F,       // 0
    MIDI_NOTE_C1 + MIDI_NOTE_A,       // 1
    MIDI_NOTE_C2 + MIDI_NOTE_C_SHARP,       // 2
    MIDI_NOTE_C2 + MIDI_NOTE_F,       // 3
    MIDI_NOTE_C2 + MIDI_NOTE_A,       // 4
    0,       // 5
    0,       // 6
    0,       // 7
    0,       // 8
    0,       // 9
    0,       // 10
    0,       // 11
    0,       // 12
    0,       // 13
    0,       // 14
    0        // 15
  } 
};

T_Bidon sdTenorBass2 = 
{
  SD_TENORBASS2, "Tenor Bass 2" , 5, 
  { 
    MIDI_NOTE_C1 + MIDI_NOTE_F_SHARP,       // 0
    MIDI_NOTE_C1 + MIDI_NOTE_B_FLAT,       // 1
    MIDI_NOTE_C2 + MIDI_NOTE_D,       // 2
    MIDI_NOTE_C2 + MIDI_NOTE_F_SHARP,       // 3
    MIDI_NOTE_C2 + MIDI_NOTE_B_FLAT,       // 4
    0,       // 5
    0,       // 6
    0,       // 7
    0,       // 8
    0,       // 9
    0,       // 10
    0,       // 11
    0,       // 12
    0,       // 13
    0,       // 14
    0        // 15
  } 
};

T_Bidon sdTenorBass3 = 
{
  SD_TENORBASS3, "Tenor Bass 3" , 5, 
  { 
    MIDI_NOTE_C1 + MIDI_NOTE_G,       // 0
    MIDI_NOTE_C1 + MIDI_NOTE_B,       // 1
    MIDI_NOTE_C2 + MIDI_NOTE_E_FLAT,       // 2
    MIDI_NOTE_C2 + MIDI_NOTE_G,       // 3
    MIDI_NOTE_C2 + MIDI_NOTE_B,       // 4
    0,       // 5
    0,       // 6
    0,       // 7
    0,       // 8
    0,       // 9
    0,       // 10
    0,       // 11
    0,       // 12
    0,       // 13
    0,       // 14
    0        // 15
  } 
};


T_Bidon sdTenorBass4 = 
{
  SD_TENORBASS4, "Tenor Bass 4" , 5, 
  { 
    MIDI_NOTE_C1 + MIDI_NOTE_G_SHARP,  // 0
    MIDI_NOTE_C2 + MIDI_NOTE_C,       // 1
    MIDI_NOTE_C2 + MIDI_NOTE_E,       // 2
    MIDI_NOTE_C2 + MIDI_NOTE_G_SHARP,      // 3
    MIDI_NOTE_C3 + MIDI_NOTE_C,       // 4
    0,       // 5
    0,       // 6
    0,       // 7
    0,       // 8
    0,       // 9
    0,       // 10
    0,       // 11
    0,       // 12
    0,       // 13
    0,       // 14
    0        // 15
  } 
};


// ---------------------------------------------------------------------------------
// DOUBLE GUITAR
// ---------------------------------------------------------------------------------

T_Bidon sdDoubleGuitar1 = 
{
  SD_DOUBLEGUITAR1, "Double Guitar 1" , 10, 
  { 
    MIDI_NOTE_C2 + MIDI_NOTE_C_SHARP,       // 0
    MIDI_NOTE_C2 + MIDI_NOTE_E_FLAT,       // 1
    MIDI_NOTE_C2 + MIDI_NOTE_F,       // 2
    MIDI_NOTE_C2 + MIDI_NOTE_G,       // 3
    MIDI_NOTE_C2 + MIDI_NOTE_A,       // 4
    MIDI_NOTE_C2 + MIDI_NOTE_B,       // 5
    MIDI_NOTE_C3 + MIDI_NOTE_C_SHARP,       // 6
    MIDI_NOTE_C3 + MIDI_NOTE_E_FLAT,       // 7
    MIDI_NOTE_C3 + MIDI_NOTE_F,       // 8
    MIDI_NOTE_C3 + MIDI_NOTE_G,       // 9
    0,       // 10
    0,       // 11
    0,       // 12
    0,       // 13
    0,       // 14
    0        // 15
  } 
};

T_Bidon sdDoubleGuitar2 = 
{
  SD_DOUBLEGUITAR2, "Double Guitar 2" , 10, 
  { 
    MIDI_NOTE_C2 + MIDI_NOTE_D,       // 0
    MIDI_NOTE_C2 + MIDI_NOTE_E,       // 1
    MIDI_NOTE_C2 + MIDI_NOTE_F_SHARP,       // 2
    MIDI_NOTE_C2 + MIDI_NOTE_G_SHARP,       // 3
    MIDI_NOTE_C2 + MIDI_NOTE_B_FLAT,       // 4
    MIDI_NOTE_C3 + MIDI_NOTE_C,       // 5
    MIDI_NOTE_C3 + MIDI_NOTE_D,       // 6
    MIDI_NOTE_C3 + MIDI_NOTE_E,       // 7
    MIDI_NOTE_C3 + MIDI_NOTE_F_SHARP,       // 8
    MIDI_NOTE_C3 + MIDI_NOTE_G_SHARP,       // 9
    0,       // 10
    0,       // 11
    0,       // 12
    0,       // 13
    0,       // 14
    0        // 15
  } 
};


// ---------------------------------------------------------------------------------
// HandPAN
// ---------------------------------------------------------------------------------
T_Bidon sdHandPan1 = 
{
  SD_HANDPAN1, "HandPan1" , 8, 
  { 
    MIDI_NOTE_C3 + MIDI_NOTE_A,       // 0
    MIDI_NOTE_C3 + MIDI_NOTE_B,       // 1
    MIDI_NOTE_C4 + MIDI_NOTE_C,       // 2
    MIDI_NOTE_C4 + MIDI_NOTE_D,       // 3
    MIDI_NOTE_C4 + MIDI_NOTE_E,       // 4
    MIDI_NOTE_C4 + MIDI_NOTE_F,       // 5
    MIDI_NOTE_C4 + MIDI_NOTE_G,       // 6
    MIDI_NOTE_C4 + MIDI_NOTE_A,       // 7
    0,       // 8
    0,       // 9
    0,       // 10
    0,       // 11
    0,       // 12
    0,       // 13
    0,       // 14
    0        // 15
  } 
};


// ---------------------------------------------------------------------------------
// Test
// ---------------------------------------------------------------------------------


T_Bidon sdBass1 = 
{
  SD_BASS1 , "Bass1" , 3, 
  { 1, 2, 3, 0,
    0, 0, 0, 0,
    0, 0 ,0, 0,
    0, 0, 0, 0 } 
};
T_Bidon sdBass2 = 
{
  SD_BASS2 , "Bass2" , 3, 
  { 1, 2, 3, 0,
    0, 0, 0, 0,
    0, 0 ,0, 0,
    0, 0, 0, 0 } 
};
T_Bidon sdBass3 = 
{
  SD_BASS3 , "Bass3" , 3, 
  { 1, 2, 3, 0,
    0, 0, 0, 0,
    0, 0 ,0, 0,
    0, 0, 0, 0 } 
};



T_Bidon sdTenor1 = 
{
  SD_TENOR1, "Tenor 1" , 4, 
  { 
    MIDI_NOTE_C2 + MIDI_NOTE_C,       // 0
    MIDI_NOTE_C2 + MIDI_NOTE_D,       // 1
    MIDI_NOTE_C2 + MIDI_NOTE_E,       // 2
    MIDI_NOTE_C2 + MIDI_NOTE_D,       // 3
    MIDI_NOTE_C2 + MIDI_NOTE_F,       // 4
    MIDI_NOTE_C2 + MIDI_NOTE_G,       // 5
    MIDI_NOTE_C2,        // 6
    MIDI_NOTE_C2,       // 7
    MIDI_NOTE_C2,        // 8
    MIDI_NOTE_C2,       // 9
    MIDI_NOTE_C2,        // 10
    MIDI_NOTE_C2,       // 11
    MIDI_NOTE_C2,        // 12
    MIDI_NOTE_C2,        // 13
    MIDI_NOTE_C2,        // 14
    MIDI_NOTE_C2        // 15
  } 
};

T_Bidon sdTenor2 = 
{
  SD_TENOR2, "Tenor 2" , 5, 
  { 
    MIDI_NOTE_C3 + MIDI_NOTE_C,       // 0
    MIDI_NOTE_C3 + MIDI_NOTE_D,       // 1
    MIDI_NOTE_C3 + MIDI_NOTE_E,       // 2
    MIDI_NOTE_C3 + MIDI_NOTE_D,       // 3
    MIDI_NOTE_C3 + MIDI_NOTE_F,       // 4
    MIDI_NOTE_C3 + MIDI_NOTE_G,       // 5
    MIDI_NOTE_C3,        // 6
    MIDI_NOTE_C3,       // 7
    MIDI_NOTE_C3,        // 8
    MIDI_NOTE_C3,       // 9
    MIDI_NOTE_C3,        // 10
    MIDI_NOTE_C3,       // 11
    MIDI_NOTE_C3,        // 12
    MIDI_NOTE_C3,        // 13
    MIDI_NOTE_C3,        // 14
    MIDI_NOTE_C3        // 15
  } 
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  Liste des instruments
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

T_Instru lSD[] = 
{
  {
    SD_TENORBASS, "Tenor Bass",4,
    {
      & sdTenorBass1,
      & sdTenorBass2,
      & sdTenorBass3,
      & sdTenorBass4,
      NULL,
      NULL
    }
  },
  {
    SD_DOUBLEGUITAR, "Double Guitar",2,
    {
      & sdDoubleGuitar1,
      & sdDoubleGuitar2,
      NULL,
      NULL,
      NULL,
      NULL
    }
  },  
    {
    SD_HANDPAN, "HandPan",1,
    {
      & sdHandPan1,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL
    }
  }, 
  {
    SD_TENOR, "Tenor",2,
    {
      & sdTenor1,
      & sdTenor2,
      NULL,
      NULL,
      NULL,
      NULL
    }
  },
};

uint8_t nbSD = sizeof(lSD) / sizeof(T_Instru);



T_Bidon * getBidonByID(uint8_t id) 
{
    for (int i=0; i<nbSD; i++) {
        for (int j=0; j < lSD[i].nbBidons; j ++) {
          if ( lSD[i].bidons[j]->id == id )  {
            return lSD[i].bidons[j];
          }
        }
    }  
    return NULL;
}