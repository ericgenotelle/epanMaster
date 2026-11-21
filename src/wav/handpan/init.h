#include "../../config.h"
#include "../../midiinfo.h"

#include "a2_wav.h"
#ifndef JUST1WAV
    #include "as2_wav.h"
    #include "b2_wav.h"
    #include "c3_wav.h"
    #include "cs3_wav.h"
    #include "d3_wav.h"
    #include "ds3_wav.h"
    #include "e3_wav.h"
    #include "f3_wav.h"
    #include "fs3_wav.h"
    #include "g3_wav.h"
    #include "gs3_wav.h"
    #include "a3_wav.h"
    #include "as3_wav.h"
#endif

void initWaves()
{
  uint8_t note = MIDI_NOTE_C3 + MIDI_NOTE_A;
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
#ifndef JUST1WAV
  setupSound(as2_wav,  sizeof(as2_wav) / 2,  note++, 0);
  setupSound(b2_wav,   sizeof(b2_wav) / 2,   note++, 0);
  setupSound(c3_wav,   sizeof(c3_wav) / 2,   note++, 0);
  setupSound(cs3_wav,  sizeof(cs3_wav) / 2,  note++, 0);
  setupSound(d3_wav,   sizeof(d3_wav) / 2,   note++, 0);
  setupSound(ds3_wav,  sizeof(ds3_wav) / 2,  note++, 0);
  setupSound(e3_wav,   sizeof(e3_wav) / 2,   note++, 0);
  setupSound(f3_wav,   sizeof(f3_wav) / 2,   note++, 0);
  setupSound(fs3_wav,  sizeof(fs3_wav) / 2,  note++, 0);
  setupSound(g3_wav,   sizeof(g3_wav) / 2,   note++, 0);
  setupSound(gs3_wav,  sizeof(gs3_wav) / 2,  note++, 0);
  setupSound(a3_wav,   sizeof(a3_wav) / 2,   note++, 0);
  setupSound(as3_wav,  sizeof(as3_wav) / 2,  note++, 0);
#else
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
  setupSound(a2_wav, sizeof(a2_wav) / 2,   note++ , 0); 
#endif
}