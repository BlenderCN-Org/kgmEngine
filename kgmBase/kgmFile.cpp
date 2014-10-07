#include "kgmFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#ifdef WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif


kgmFile::kgmFile()
{
  m_file = 0;
}

kgmFile::~kgmFile()
{
  close();
}

bool kgmFile::open(kgmCString& file, u32 mode)
{
  u32 smode;

#ifdef WIN32
#define O_EXCL    _O_EXCL
#define O_RDWR    _O_RDWR
#define O_CREAT   _O_CREAT
#define O_RDONLY  _O_RDONLY
#define O_WRONLY  _O_WRONLY
#define O_BINARY  _O_BINARY
#defind open      _open
#defind close     _close
#defind read      _read
#defind write     _write
#endif

  if((mode & Write) && (mode & Read))
    smode = O_RDWR;
  else if(mode & Write)
    smode = O_WRONLY;
  else if(mode & Read)
    smode = O_RRONLY;

  if(mode & Create)
    smode |= (O_CREAT | O_EXCL);

  smode |= 

  m_file = ::open(file, smode);
  
  if(!m_file)
    return false;

  return true;
}

void kgmFile::close()
{
  if(m_file)
    close(m_file);

  m_file = 0;
}

void kgmFile::flush()
{
  fflush(m_file);
}

u32 kgmFile::read(void *dst, u32 cnt)
{
  return fread(dst, 1, cnt, m_file);
}

u32 kgmFile::write(void *src, u32 cnt)
{
  return fwrite(src, 1, cnt, m_file);
}

u32 kgmFile::append(void *src, u32 cnt)
{
  return 0;
}

u32 kgmFile::erase(u32 from, u32 size)
{
  return 0;
}

u32 kgmFile::length()
{
  long size = 0;
  long cpos = 0;
  cpos = ftell(m_file);
  fseek(m_file, 0, SEEK_END);
  size = ftell(m_file);
  fseek(m_file, cpos, SEEK_SET);

  return (u32)size;
}

u32 kgmFile::length(u32 len)
{
#ifdef WIN32
  HANDLE hFile = (HANDLE)_get_osfhandle(m_file->_file);

  if(hFile)
  {
    SetFilePointer(hFile, (LONG)len, NULL, FILE_BEGIN);

    if(SetEndOfFile(hFile))
      return len;
    else
      return 0;
  }
#elif defined ANDROID
  if(m_file && m_file->_file)
    return ftruncate(m_file->_file, len);
#else
  if(m_file && m_file->_fileno)
    return ftruncate(m_file->_fileno, len);
#endif

  return 0;
}

u32 kgmFile::position()
{
  return (u32)ftell(m_file);
}

u32 kgmFile::seek(u32 pos)
{
  if(!m_file)
    return -1;

  return fseek(m_file, pos, SEEK_SET);
}

bool kgmFile::eof()
{
  return feof(m_file);
}

void* kgmFile::mmap()
{
  u32 size = 0;

  if(!m_file)
    return 0;

  size = length();
 
 if(!size)
    return 0;

  char* v = (char*)malloc(sizeof(char) * size);

  if(!v)
    return 0;

  seek(0);
  read(v, size);
  seek(0);

  return v;
}

void kgmFile::munmap(void* v)
{
  if(v)
    free(v);
}

bool kgmFile::remove_file(kgmString path)
{
  if(remove(path.data()))
    return false;

  return true;
}

bool kgmFile::rename(kgmString oname, kgmString nname)
{
  if(rename(oname.data(), nname.data()))
    return false;

  return true;
}

bool kgmFile::make_directory(kgmString& path)
{
  int res = 0;

#ifdef WIN32
  res = CreateDirectory(path.data(), NULL);
#else
  res = mkdir(path.data(), 0777);
#endif

  if(res)
    return false;

  return true;
}



