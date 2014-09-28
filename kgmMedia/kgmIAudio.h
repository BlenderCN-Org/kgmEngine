#pragma once
#include "../kgmBase/kgmInterface.h"
#include "../kgmMath/kgmMath.h"

//class kgmWave;
//class kgmSound;

class kgmIAudio: public kgmInterface
{
public: 
  enum FMT
  {
    FMT_NONE,
    FMT_MONO8,
    FMT_MONO16,
    FMT_STEREO8,
    FMT_STEREO16
  };

  typedef void* Sound;

public:
  virtual Sound  create(FMT fmt, u16 freq, u32 size, void* data) = 0;
  virtual void   remove(Sound snd)                              = 0;

  virtual void   volume(Sound snd, u16 vol)                     = 0;
  virtual void   pause(Sound snd, bool stat)                    = 0;
  virtual void   play(Sound snd, bool loop)                     = 0;
  virtual void   stop(Sound snd)                                = 0;
  virtual void   pan(Sound snd, s16 pan)                        = 0;
};
