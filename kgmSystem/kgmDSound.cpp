// kgmOALDevice.cpp: implementation of the kgmOALDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmDSound.h"
#include "../kgmBase/kgmTime.h"
#include "../kgmBase/kgmLog.h"

#ifdef WIN32
#include <mmsystem.h>
#endif

KGMOBJECT_IMPLEMENT(kgmDSound, kgmIAudio);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static u32 error = 0;

#ifdef DSOUND
/*struct _Sound
{
  LPDIRECTSOUNDBUFFER pSb;

  _Sound(LPDIRECTSOUNDBUFFER sb);
  ~_Sound();

  void release();
  void stop();
  void play(bool loop);
  void pause();
  void volume(float vol);
  void emit(vec3& pos, vec3& vel);

  void drop();
};

_Sound::_Sound(LPDIRECTSOUNDBUFFER sb)
{
  pSb = sb;
}

_Sound::~_Sound()
{
}

void _Sound::release()
{
  if(pSb)
    pSb->Release();
}

void _Sound::stop()
{
}

void _Sound::play(bool loop)
{
  // should be check because don't play
  if(pSb)
  {
    if(FAILED(pSb->Play(0, 0, (loop) ? (DSBPLAY_LOOPING) : (0))))
    {
#ifdef DEBUG
    kgm_log() << "Error: can't play sound buffer.\n";
#endif
    }
  }
}

void _Sound::volume(float vol)
{
}

void _Sound::pause()
{
}
*/

void CALLBACK DirectSoundProc(UINT uID, UINT uReserved, DWORD_PTR dwUser,
                                         DWORD_PTR dwReserved1, DWORD_PTR dwReserved2)
{
  kgmDSound *self = (kgmDSound*)dwUser;
  DWORD PlayCursor,WriteCursor;
  BYTE *WritePtr1,*WritePtr2;
  DWORD WriteCnt1,WriteCnt2;
  WAVEFORMATEX OutputType;
  DWORD BytesPlayed;
  DWORD BufSize;
  HRESULT DSRes;

  (void)uID;
  (void)uReserved;
  (void)dwReserved1;
  (void)dwReserved2;

  BufSize = self->m_mixer.getFrames() * self->m_mixer.getBytesPerFrame();

  if(BufSize < 1)
    return;

  // Get current play and write cursors
  IDirectSoundBuffer_SetCurrentPosition(self->m_pSbuf, 0);
  IDirectSoundBuffer_GetCurrentPosition(self->m_pSbuf, &PlayCursor, &WriteCursor);

  // Get the output format and figure the number of bytes played (block aligned)
  IDirectSoundBuffer_GetFormat(self->m_pSbuf, &OutputType, sizeof(WAVEFORMATEX),NULL);

  //BytesPlayed=((((WriteCursor<pData->OldWriteCursor)?(BufSize+WriteCursor-pData->OldWriteCursor):(WriteCursor-pData->OldWriteCursor))/OutputType.nBlockAlign)*OutputType.nBlockAlign);

  // Lock output buffer started at 40msec in front of the old write cursor (15msec in front of the actual write cursor)
  DSRes = IDirectSoundBuffer_Lock(self->m_pSbuf, 0, BufSize,(LPVOID*)&WritePtr1, &WriteCnt1,
                                  (LPVOID*)&WritePtr2, &WriteCnt2, DSBLOCK_ENTIREBUFFER);

  // If the buffer is lost, restore it, play and lock
  if (DSRes==DSERR_BUFFERLOST)
  {
    IDirectSoundBuffer_Restore(self->m_pSbuf);
    IDirectSoundBuffer_Play(self->m_pSbuf,0,0,DSBPLAY_LOOPING);
    DSRes = IDirectSoundBuffer_Lock(self->m_pSbuf, 0, BufSize,(LPVOID*)&WritePtr1, &WriteCnt1,
                                    (LPVOID*)&WritePtr2, &WriteCnt2, DSBLOCK_ENTIREBUFFER);
  }

  // Successfully locked the output buffer
  if (DSRes==DS_OK)
  {
    // If we have an active context, mix data directly into output buffer otherwise fill with silence
    //SuspendContext(NULL);
    kgmThread::lock(self->m_mutex);
    u32 t1 = kgmTime::getTicks();
    //if (WritePtr1)
    //  aluMixData(pDevice->Context, WritePtr1, WriteCnt1, pDevice->Format);
    //if (WritePtr2)
    //  aluMixData(pDevice->Context, WritePtr2, WriteCnt2, pDevice->Format);
    memcpy(WritePtr1, self->m_mixer.getBuffer(), WriteCnt1);
    //ProcessContext(NULL);

    // Unlock output buffer only when successfully locked
    IDirectSoundBuffer_Unlock(self->m_pSbuf, WritePtr1, WriteCnt1, WritePtr2, WriteCnt2);

    self->m_mixer.clean();

    u32 t2 = kgmTime::getTicks();

    u32 t3 = t2 - t2;

    if(t3 < self->m_mixer.getMsTime())
    {
      s32 wtime = self->m_mixer.getMsTime() - t3;

      kgmThread::sleep(wtime - 10);
    }

    kgmThread::unlock(self->m_mutex);
  }
}

struct _Sound
{
  enum State
  {
    StStop,
    StPlay,
    StPause
  };

  void *data;
  u32   size;
  u32   cursor;

  s16   vol;
  s16   pan;

  int bps;
  int rate;
  int frames;
  int channels;

  bool loop;
  bool remove;

  State state;

  _Sound(kgmIAudio::FMT fmt, u16 freq, u32 size, void* data)
  {
    this->data   = null;
    this->size   = 0;

    cursor = 0;

    pan      = 0;
    vol      = 0;

    bps      = 0;
    rate     = freq;
    frames   = 0;
    channels = 0;

    loop = false;
    state = StStop;

    remove = false;

    rate = freq;

    switch(fmt)
    {
    case kgmIAudio::FMT_MONO8:
      bps = 8;
      channels = 1;
      break;
    case kgmIAudio::FMT_MONO16:
      bps = 16;
      channels = 1;
      break;
    case kgmIAudio::FMT_STEREO8:
      bps = 8;
      channels = 2;
      break;
    case kgmIAudio::FMT_STEREO16:
    default:
      bps = 16;
      channels = 2;
    }

    if(data != null && size > 0)
    {
      this->data = malloc(size);

      memcpy(this->data, data, size);

      this->size = size;
    }
  }

  ~_Sound()
  {
    if(data)
      free(data);
  }

  void release()
  {
    remove = true;
  }

  void stop()
  {
    state  = StStop;

    cursor = 0;
  }

  void play(bool lp)
  {
    state = StPlay;

    loop = lp;
  }

  void pause(bool state)
  {
    if(state)
      state = StPause;
    else
      state = StPlay;
  }

  void volume(float vol)
  {

  }
};

kgmDSound::kgmDSound()
{
  m_pSnd = null;
  m_pSbuf = null;

  m_proceed = false;

  if(FAILED(DirectSoundCreate(0, &m_pSnd, 0)))
  {
#ifdef DEBUG
    kgm_log() << "Error: can't create direct sound.\n";
#endif
    return;
  }

  //Set Cooperative Level
   HWND hWnd = GetForegroundWindow();

   if (hWnd == NULL)
   {
    hWnd = GetDesktopWindow();
   }

   if(m_pSnd->SetCooperativeLevel((HWND)hWnd, DSSCL_PRIORITY) != DS_OK)
  {
#ifdef DEBUG
    kgm_log() << "Error: can't set cooperative level.\n";
#endif
  }

  m_mixer.prepare(2, 16, 44100);

  u32   size = m_mixer.getLength();
  void* data = m_mixer.getBuffer();

   DSBUFFERDESC dsb = {0};
   WAVEFORMATEX wf = {0};

   if(!m_pSnd)
   {
 #ifdef DEBUG
     kgm_log() << "Error: not valid direct sound or data.\n";
 #endif

     return;
   }

   wf.cbSize          = sizeof(WAVEFORMATEX);
   wf.nSamplesPerSec  = m_mixer.getRate();
   wf.wFormatTag      = WAVE_FORMAT_PCM;

   switch(m_mixer.getFormat())
   {
   case FMT_MONO8:
     wf.nChannels = 1;
     wf.wBitsPerSample = 8;
     break;
   case FMT_MONO16:
     wf.nChannels = 1;
     wf.wBitsPerSample = 16;
     break;
   case FMT_STEREO8:
     wf.nChannels = 2;
     wf.wBitsPerSample = 8;
     break;
   case FMT_STEREO16:
     wf.nChannels = 2;
     wf.wBitsPerSample = 16;
     break;
   default:
     m_pSnd->Release();
     m_pSnd = null;

     return;
   }

   wf.nBlockAlign = wf.nChannels * ( wf.wBitsPerSample / 8 );
   wf.nAvgBytesPerSec = wf.nBlockAlign * wf.nSamplesPerSec;

   dsb.dwSize = sizeof(DSBUFFERDESC);
   dsb.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
   dsb.dwBufferBytes = size;
   dsb.lpwfxFormat = &wf;

   if(FAILED(m_pSnd->CreateSoundBuffer(&dsb, &m_pSbuf, 0)))
   {
 #ifdef DEBUG
     kgm_log() << "Error: can't create sound buffer.\n";
 #endif
     m_pSbuf = null;

     m_pSnd->Release();
     m_pSnd = null;

     return;
   }

   if(m_pSbuf)
   {
     if(FAILED(m_pSbuf->Play(0, 0, DSBPLAY_LOOPING)))
     {
       m_pSbuf->Release();
       m_pSnd->Release();
       m_pSbuf = null;
       m_pSnd = null;
     }
     else
     {
       m_timer = timeSetEvent(m_mixer.getMsTime() - 10, 0, (LPTIMECALLBACK)DirectSoundProc, (DWORD)this,
                              (UINT)TIME_CALLBACK_FUNCTION | TIME_PERIODIC);

       m_proceed = true;

       m_thread.start(this, (int(*)(kgmDSound*))&kgmDSound::proceed);
       m_thread.priority(kgmThread::PrIdle);

       m_mutex = kgmThread::mutex();
     }
   }
}

kgmDSound::~kgmDSound()
{
  m_proceed = false;

  if(m_timer)
    timeKillEvent(m_timer);

  Sleep(100);

  m_thread.join();

  kgmThread::mxfree(m_mutex);


  if(m_pSbuf)
  {
    if(FAILED(m_pSbuf->Release()))
    {
#ifdef DEBUG
      kgm_log() << "Error: can't release direct sound buffer.\n";
#endif
    }
  }

  if(m_pSnd)
  {
    if(FAILED(m_pSnd->Release()))
    {
#ifdef DEBUG
      kgm_log() << "Error: can't release direct sound.\n";
#endif
    }
  }
}

kgmIAudio::Sound kgmDSound::create(FMT fmt, u16 freq, u32 size, void* data)
{
  _Sound* sound = new _Sound(fmt, freq, size, data);

  sound->vol = kgmIAudio::VolMax;
  sound->pan = kgmIAudio::PanBalance;

  m_sounds.add(sound);

  return sound;
}

void kgmDSound::remove(Sound snd)
{
  if(snd)
    ((_Sound*)snd)->remove = true;
}

void kgmDSound::channel(Sound snd, s16 pan)
{
  ((_Sound*)snd)->pan = pan;
}

void kgmDSound::volume(Sound snd, u16 vol)
{
  ((_Sound*)snd)->vol = vol;
}

void kgmDSound::pause(Sound snd, bool stat)
{
  if(snd)
    ((_Sound*)snd)->pause(stat);
}

void kgmDSound::play(Sound snd, bool loop)
{
  if(snd)
    ((_Sound*)snd)->play(loop);
}

void kgmDSound::stop(Sound snd)
{
  if(snd)
    ((_Sound*)snd)->stop();
}

int kgmDSound::proceed()
{
  static u32 max_sounds = 10;

  m_proceed = true;

  while(m_proceed)
  {
    u32 snd_cound = 0;

    kgmThread::lock(m_mutex);

#ifdef DEBUG
    kgm_log() << "kgmDSound:: proceed lock " << kgmTime::getTimeText() << "\n";
#endif

    m_mixer.clean();

    u32 t1 = kgmTime::getTicks();

    for(kgmList<_Sound*>::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
    {
      _Sound* sound = (*i);

      if(snd_cound > max_sounds)
        break;

      if(sound->remove)
      {
        i = m_sounds.erase(i);

        delete sound;

        continue;
      }

      if(sound->state != _Sound::StPlay)
        continue;

      u32 size = m_mixer.mixdata((sound->data + sound->cursor),
                                 (sound->size - sound->cursor),
                                 sound->channels,
                                 sound->bps,
                                 sound->rate,
                                 sound->vol,
                                 sound->pan);

      if((sound->cursor + size) == sound->size)
      {
        sound->cursor = 0;

        if(!sound->loop)
          sound->stop();
      }
      else
      {
        sound->cursor += size;
      }

      snd_cound++;
    }

    //render();

    kgmThread::unlock(m_mutex);

#ifdef DEBUG
    kgm_log() << "kgmDSound:: proceed unlock " << kgmTime::getTimeText() << "\n";
#endif

    u32 t2 = kgmTime::getTicks();

    u32 t3 = t2 - t1;

    if(t3 < m_mixer.getMsTime())
    {
      s32 wtime = m_mixer.getMsTime() - t3;

      kgmThread::sleep(wtime / 2);
    }
  }

  return 0;
}

#endif
