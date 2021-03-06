#ifndef KGMPOINTER_H
#define KGMPOINTER_H

template <class T>
class kgmPointer
{
  T*   pointer = 0;
  int* counter = 0;

  template <class A, class B>
  friend  kgmPointer<A> kgm_ptr_cast(kgmPointer<B>& var);
public:

  kgmPointer()
  {
    pointer = 0L;
    counter = 0;
  }

  kgmPointer(T* ptr)
  {
    if(ptr == 0L)
      return;

    counter = new int;

    *counter = 1;

    pointer = ptr;
  }

  kgmPointer(const kgmPointer<T> &object)
  {
    pointer = object.pointer;
    counter = object.counter;

    if(pointer)
      (*counter)++;
  }

  ~kgmPointer()
  {
    if(counter == 0)
      return;

    (*counter)--;

    if((*counter) > 0)
      return;

    delete counter;

    if(pointer)
      delete pointer;

    counter = 0;
    pointer = 0;
  }

  /*
  void operator=(const T& data)
  {
    (*pointer) = data;
  }
  */

  void operator=(const kgmPointer<T>& ptr)
  {
    if(pointer)
    {
      (*counter)--;

      if((*counter) < 1)
      {
        delete pointer;
        delete counter;

        pointer = 0;
        counter = 0;
      }
    }

    pointer = ptr.pointer;
    counter = ptr.counter;

    if(pointer)
      (*counter)++;
  }

  T* operator->() const
  {
    return pointer;
  }

  operator T*() const
  {
    return pointer;
  }

  bool valid()
  {
    if(pointer != 0 && (*counter) > 0)
      return true;

    return false;
  }

  void reset()
  {
    if(counter == 0)
      return;

    (*counter)--;

    if((*counter) < 1)
    {
      delete counter;

      if(pointer)
        delete pointer;
    }

    counter = 0;
    pointer = 0;
  }
};

template <class A, class B>
kgmPointer<A> kgm_ptr_cast(kgmPointer<B>& var)
{
  kgmPointer<A> cast;

  if(var.pointer != null)
  {
    cast.pointer = (A*) var.pointer;
    cast.counter = var.counter;

    (*var.counter)++;
  }

  return cast;
}

#define kgmPtr  kgmPointer

#define kgm_ptr kgmPointer

#endif // KGMPOINTER_H
