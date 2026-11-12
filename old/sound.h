

#ifndef __SOUND_H
#define __SOUND_H

#include "arduino.h"
#include "midifile.h"

///@brief unset to debug prg : few waves file loaded
#define DEBUG_PRG

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//     Parameters
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------



/*
#define NOTE_NONE      0
#define NOTE_PLAYING   1

#define NOTE_NOT_PLAYED    0
#define NOTE_TO_BE_PLAYED  1
#define NOTE_PLAYED        2
*/

/// @brief Status of a note
typedef enum {
  Idle          = 0,
  ToBePlayed    = 1,
  Playing       = 2,
  Played        = 3,
} NoteState;


///@brief Left or Right
typedef enum {
  rightHand = 0,
  leftHand  = 1,
  bothHand  = 2,
} RightLeftHand;




//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//     WAVE and NOTES structures
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
/// @brief Structure for WAV header
typedef struct 
{
  //   RIFF Section    
  char RIFFSectionID[4];      // Letters "RIFF"
  uint32_t Size;              // Size of entire file less 8
  char RiffFormat[4];         // Letters "WAVE"
  
  //   Format Section    
  char FormatSectionID[4];    // letters "fmt"
  uint32_t FormatSize;        // Size of format section less 8
  uint16_t FormatID;          // 1=uncompressed PCM
  uint16_t NumChannels;       // 1=mono,2=stereo
  uint32_t SampleRate;        // 44100, 16000, 8000 etc.
  uint32_t ByteRate;          // =SampleRate * Channels * (BitsPerSample/8)
  uint16_t BlockAlign;        // =Channels * (BitsPerSample/8)
  uint16_t BitsPerSample;     // 8,16,24 or 32

  // Data Section
  char DataSectionID[4];      // The letters "data"
  uint32_t DataSize;          // Size of the data that follows
} T_WavHeader;



//------------------------------------------------------------------------------------------------------------------------
/// @brief Structure for a sound : note + wavHeader + wav signal
typedef struct 
{
    int                   noteMidi;           // MIDI note 
    char                  noteName [4];   // MIDI note name
    T_WavHeader           wavHeader;         // WAV header
//    uint8_t               velocity;       // velocity : 0 --> 100
    const unsigned char * wavBody;            // WAV body
    unsigned long         timeMsStop = 0;     // when the sound was finished to be played
    int                   ledPINRight;
    int                   ledPINLeft;
    bool                  ledRightOn = false;
    bool                  ledLeftOn = false;
    int                   threshold = 200;
} T_Sound;



//------------------------------------------------------------------------------------------------------------------------
/// @brief Structure for a played note
typedef struct 
{
  NoteState    state;  // 'Idle' or 'Playing'
  T_Sound *     pSound;
  uint32_t      posWav;    // cursor position in sound WAV
  int           volume;    // 0 -> 127  //TBD velocity ?
  uint32_t      chrono;    // unique incremental ID (used to get the oldest note)
} T_NotePoly;



//------------------------------------------------------------------------------------------------------------------------
/// @brief A note to be played in a song T_Song
typedef struct 
{
	int			        num;             // Num of the note in the song
	int			        noteMidi;        // MIDI Note
  NoteState      state;           // Idle, ToBePlayed, Played
	int			        velocity;        // Velocity of the note : 0->127
  int             measure;
  int             idxSound;        // index of the sound  
	unsigned long   timeMidiStart;   // MIDI tick when the note starts
	unsigned long   timeMidiStop;    // MIDI tick when the note stop
	unsigned long   timeMsStart;     // time in ms position when the note start (t0=0)
	unsigned long   timeMsStop;      // time in ms position when the note stop (t0=0)
} T_SongNote;


//------------------------------------------------------------------------------------------------------------------------
/// @brief A song
typedef struct 
{
  uint16_t idChange = 0;  
  char    filename[100];       // song name
	unsigned long t0=0;      // when song started
	bool 		playing=false;   // the song is currently playing ?
	int     noteCur;     // current playing note
	int     measureCur;     // current playing measure
  int     measureStart;    // First Measure to be played
  int     measureStop;     // Last Measure to be played
  bool    loop=false;      // play the song in loop
  
	int 		ppqn;            // PPQN of the song
	int 		bpmBase=0;       // orginal BPM of the song
	int 		bpm=0;           // BPM of the song
	int 		nbNotes = 0;     // nb notes in the song
	int 		nbMeasures = 0;     // nb measures in the song
	bool		modeAuto=true;   // song in mode AUTO (ESP is playing) and LEARNING
  bool    withSound=true;   // true=play song, false=do not play sound (LED only)
	T_SongNote * 	lNotes=NULL;  // list of notes in the song
  bool    doubleNotes;
} T_Song;



//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//     Global variables needed outsite 'sound' module
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

extern int volumeGen ;
extern T_Sound sounds[];
extern T_NotePoly notesPoly[];
extern int nbPlayingNotes;  
extern T_Song song;
extern unsigned char fileMidi[]; 
extern size_t fileMidiLen;



//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//     WAVE and NOTES functions
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
void setupSound(int idx, const unsigned char * son_wav, int note,const char * noteName, int ledPINLeft, int ledPINRight);
void setupSounds();
int getSoundIndexByNote(int note);

void resetNotePoly(int i);
void setupNotesPoly();
int getNotePolyIndexAvailable();

int playSound(int idxSound, int volume);
int playNote(int noteMidi, int volume);

void dumpData(const char *Data, uint8_t NumBytes);
bool validWavData(T_WavHeader * Wav);
void dumpWAVHeader(T_WavHeader * Wav);
void loopPlayWaves();

float midiClic2Ms(T_Song * pSong);
RightLeftHand getHandSongNote(T_SongNote * pSongNote);
bool initiateSong(T_Song* pSong) ;
bool generateSong(const MIDI_FILE *pMF,T_Song* pSong);
bool computeSongTime(T_Song* pSong,int bpm);
bool dumpSong(T_Song* pSong);
bool loadSong(T_Song * pSong, const BYTE * mem, size_t memSize) ;
bool startSong();
bool stopSong();
void loopSong();





#endif