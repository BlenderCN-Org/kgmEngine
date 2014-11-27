#pragma once
#include "kgmMain.h"
#include "kgmTypes.h"
#include "kgmList.h"
#include <stdlib.h>
#include <string.h>
#include <vector>

class kgmApp;
class kgmObject;

struct kgmRuntime
{
  const char* nClass; //class id
  u32         sClass; //size of class
  kgmRuntime* pClass; //parent class
};

#define KGM_OBJECT(o_class)                     \
  public:                                	      \
  static  kgmRuntime   Class;	                  \
  static  o_class*     cast(kgmObject*);        \
  virtual kgmRuntime&  runtime();               \
  private:

#define KGMOBJECT_IMPLEMENT(o_class, o_parent)                                         \
  kgmRuntime  o_class::Class = {  #o_class, sizeof(class o_class), &o_parent::Class};  \
  o_class*    o_class::cast(kgmObject* o) { return (o_class*)o; }                      \
  kgmRuntime& o_class::runtime() { return o_class::Class; }

// Base class for kgm_engine objects.

class kgmObject
{
  KGM_OBJECT(kgmObject);

  template<class... Args>
  class Action
  {
  public:
    virtual void call(Args... args) = 0;
  };

public:
  template<class... Args>
  class Event
  {
    kgmList<Action<Args...>*> list;

    friend void kgmObject::connect();
    friend void kgmObject::spread();

  public:
    void operator()(Args... args)
    {
      for(int i = 0; i < list.length(); i++)
        (list[i])->call(args...);
    }
  };

private:
  unsigned int m_references;

protected:
  virtual ~kgmObject()
  {
  }

public:
  kgmObject()
  {
    m_references = 1;
  }

  kgmObject(const kgmObject& o)
  {
    m_references = o.m_references;
  }

  bool isClass(kgmObject& o)
  {
    return !strcmp(runtime().nClass, o.runtime().nClass);
  }

  bool isClass(kgmRuntime& o)
  {
    return !strcmp(runtime().nClass, o.nClass);
  }

  bool isClass(const char* o)
  {
    return !strcmp(runtime().nClass, o);
  }

  bool isType(kgmObject& o)
  {
    kgmRuntime* r = &runtime();

    while(r != 0)
    {
      if(!strcmp(r->nClass, o.runtime().nClass))
        return true;

      if(r == r->pClass)
        break;

      r = r->pClass;
    }

    return false;
  }

  bool isType(kgmRuntime& o)
  {
    kgmRuntime* r = &runtime();

    while(r != 0)
    {
      if(!strcmp(r->nClass, o.nClass))
        return true;

      if(r == r->pClass)
        break;

      r = r->pClass;
    }

    return false;
  }

  bool isType(const char* o)
  {
    kgmRuntime* r = &runtime();

    while(r != 0){
      if(!strcmp(r->nClass, o))
        return true;

      if(r == r->pClass)
        break;

      r = r->pClass;
    }

    return false;
  }

  u32 references()
  {
    return m_references;
  }

  void increment()
  {
    m_references += 1;
  }

  void release()
  {
    if(!kgmObject::isValid(this))
      return;

    m_references--;

    if(m_references < 1)
      delete this;
  }

  static kgmObject* assign(kgmObject* dest, kgmObject* src)
  {
    if (src == null)
      return null;

    dest = src;

    src->increment();

    return dest;
  }

  template<class T, class... Args>
  void connect(T *t, void(T::*f)(Args...), Event<Args...> &s)
  {
  }

  virtual kgmObject* clone()
  {
    return null;
  }

  void* operator new(size_t size);
  void  operator delete(void* p);

  friend class kgmApp;

  //private:
  static void releaseObjects();
  static int  objectCount();
  static bool isValid(kgmObject*);
#ifdef DEBUG
  static void listObjects();
#endif

private:
  template<class... Args>
  void spread(Event<Args...> &evt);
};
///////////////////




