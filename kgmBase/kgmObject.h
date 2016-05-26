#pragma once
#include "kgmList.h"
#include <stdlib.h>
#include <string.h>
#include <kgmIObject.h>

/*
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
*/
// Base class for kgm_engine objects.

class kgmObject: public kgmIObject
{
  KGM_OBJECT(kgmObject);

protected:
  template <class... Args> class Signal;

  class BaseSlot
  {
  public:
    virtual ~BaseSlot(){}
  };

  template <class... Args>
  class AbstractSlot: public BaseSlot
  {
  protected:
    virtual ~AbstractSlot()
    {
      for(int i = 0; i < signals.length(); i++)
        signals[i]->disconnect(this);
    }

    friend class Signal<Args...>;

    virtual void add(Signal<Args...>* e)
    {
      signals.add(e);
    }

    virtual void remove(Signal<Args...>* e)
    {
      signals.erase(e);
    }

    virtual void call(Args... args) = 0;

  protected:
    kgmList<Signal<Args...>*> signals;
  };

  template <class T, class... Args>
  class Slot: public AbstractSlot<Args...>
  {
  public:
    Slot()
    {
      this->t = null;
      this->f = null;
    }

    bool connect(T* t, void(T::*f)(Args...), Signal<Args...> *s)
    {
      if (this->t != null || this->f != null)
        return false;

      this->t = t;
      this->f = f;

      AbstractSlot<Args...>::add(s);
      s->connect(this);
    }

  private:
    Slot(const Slot&)
    {

    }

    void operator=(const Slot&)
    {

    }

    friend class Signal<Args...>;

    virtual void call(Args... args)
    {
      if(!t || !f)
        return;

      (t->*f)(args...);
    }

  private:
    T* t;
    void (T::*f)(Args...);
  };

  template<class... Args>
  class Signal
  {
    kgmList<AbstractSlot<Args...>*> list;

  public:
    Signal()
    {

    }

    ~Signal()
    {
      for(int i = 0; i < list.length(); i++)
        list[i]->remove(this);
    }

    void connect(AbstractSlot<Args...> *s)
    {
      if(!s)
        return;

      list.add(s);

      s->add(this);
    }

    void disconnect(AbstractSlot<Args...> *s)
    {
      list.erase(s);
    }

    void operator()(Args... args)
    {
      for(int i = 0; i < list.length(); i++)
        list[i]->call(args...);
    }
  };

private:

public:
  kgmObject()
  {
  }

  kgmObject(const kgmObject& o)
  {
  }

  ~kgmObject()
  {
  }

  virtual kgmObject* clone()
  {
    return null;
  }
};
