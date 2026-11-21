///=======================================================================================================================
///
/// @file instru.cpp
/// 
// 
/// @author Eric GENOTELLE
/// @date 2026
///
///=======================================================================================================================

#include "tools.h"

#include "midiinfo.h"
#include  "instru.h"

// ---------------------------------------------------------------------------------
// HandPAN
// ---------------------------------------------------------------------------------
#if defined(HANDPAN)
T_Card sdHandPan1 = 
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
  } 
};
T_Card sdHandPan2 = 
{
  SD_HANDPAN2, "HandPan2" , 6, 
  { 
    MIDI_NOTE_C3 + MIDI_NOTE_A_SHARP,       // 0
    MIDI_NOTE_C3 + MIDI_NOTE_C_SHARP,       // 1
    MIDI_NOTE_C4 + MIDI_NOTE_D_SHARP,       // 2
    MIDI_NOTE_C4 + MIDI_NOTE_F_SHARP,       // 3
    MIDI_NOTE_C4 + MIDI_NOTE_G_SHARP,       // 4
    MIDI_NOTE_C4 + MIDI_NOTE_A_SHARP,       // 5
    0,       // 6
    0,       // 7
  } 
};
T_Instru instru =
{
  SD_HANDPAN, "HandPan",2,
  {
    & sdHandPan1,
    & sdHandPan2,
    NULL,
    NULL,
    NULL,
    NULL
  }
};
#endif



// ---------------------------------------------------------------------------------
// TENOR BASS
// ---------------------------------------------------------------------------------
#if defined (TENORBASS)   

T_Card sdTenorBass1 = 
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
  } 
};

T_Card sdTenorBass2 = 
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
  } 
};

T_Card sdTenorBass3 = 
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
  } 
};


T_Card sdTenorBass4 = 
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
  } 
};

T_Instru instru =
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
};
#endif //TENORBASS

// ---------------------------------------------------------------------------------
// DOUBLE GUITAR
// ---------------------------------------------------------------------------------
#if defined(DOUBLEGUITAR)
T_Card sdDoubleGuitar1a = 
{
  SD_DOUBLEGUITAR1A, "Double Guitar 1a" , 8, 
  { 
    MIDI_NOTE_C2 + MIDI_NOTE_C_SHARP,       // 0
    MIDI_NOTE_C2 + MIDI_NOTE_E_FLAT,       // 1
    MIDI_NOTE_C2 + MIDI_NOTE_F,       // 2
    MIDI_NOTE_C2 + MIDI_NOTE_G,       // 3
    MIDI_NOTE_C2 + MIDI_NOTE_A,       // 4
    MIDI_NOTE_C2 + MIDI_NOTE_B,       // 5
    MIDI_NOTE_C3 + MIDI_NOTE_C_SHARP,       // 6
    MIDI_NOTE_C3 + MIDI_NOTE_E_FLAT,       // 7
  } 
};

T_Card sdDoubleGuitar1b = 
{
  SD_DOUBLEGUITAR1B, "Double Guitar 1b" , 2, 
  { 
    MIDI_NOTE_C3 + MIDI_NOTE_F,       // 0
    MIDI_NOTE_C3 + MIDI_NOTE_G,       // 1
    0,       // 2
    0,       // 3
    0,       // 4
    0,       // 5
    0,       // 6
    0        // 7
  } 
};

T_Card sdDoubleGuitar2a = 
{
  SD_DOUBLEGUITAR2A, "Double Guitar 2a" , 8, 
  { 
    MIDI_NOTE_C2 + MIDI_NOTE_D,       // 0
    MIDI_NOTE_C2 + MIDI_NOTE_E,       // 1
    MIDI_NOTE_C2 + MIDI_NOTE_F_SHARP,       // 2
    MIDI_NOTE_C2 + MIDI_NOTE_G_SHARP,       // 3
    MIDI_NOTE_C2 + MIDI_NOTE_B_FLAT,       // 4
    MIDI_NOTE_C3 + MIDI_NOTE_C,       // 5
    MIDI_NOTE_C3 + MIDI_NOTE_D,       // 6
    MIDI_NOTE_C3 + MIDI_NOTE_E,       // 7
  } 
};

T_Card sdDoubleGuitar2b = 
{
  SD_DOUBLEGUITAR2B, "Double Guitar 2b" , 2, 
  { 
    MIDI_NOTE_C3 + MIDI_NOTE_F_SHARP,       // 0
    MIDI_NOTE_C3 + MIDI_NOTE_G_SHARP,       // 1
    0,       // 2
    0,       // 3
    0,       // 4
    0,       // 5
    0,       // 6
    0        // 7
  } 
};

T_Instru instru =
{

  SD_DOUBLEGUITAR, "Double Guitar",2,
  {
    & sdDoubleGuitar1a,
    & sdDoubleGuitar1b,
    & sdDoubleGuitar2a,
    & sdDoubleGuitar2b,
    NULL,
    NULL
  }
};
#endif  // DOUBLEGUITAR

