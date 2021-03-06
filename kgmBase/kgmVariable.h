#ifndef KGMVARIABLE_H
#define KGMVARIABLE_H

#pragma once

#include "kgmObject.h"
#include "kgmString.h"
#include "kgmConvert.h"

class kgmVariable
{
  typedef kgmString str;

public:

  enum Type
  {
    TNone = 0,
    TFloat,
    TString,
    TInteger,
    TBoolean,
    TPointer
  };

  enum Sync
  {
    SyncNone,
    SyncToLink,
    SyncFromLink
  };

private:
  kgmString id;
  Type      type;

  f32    v_float   = 0.0;
  str    v_string  = "";
  s32    v_integer = 0;
  bool   v_boolean = false;
  void*  v_pointer = null;

  void*  x_linked = null;  //user data pointer, which linked to update automatically

public:
  kgmVariable()
  {
    type = TNone;

    v_float   = 0.0;
    v_string  = "";
    v_integer = 0;
    v_boolean = false;
    v_pointer = null;

    x_linked  = null;
  }

  kgmVariable(kgmString name, f32 data, void* xptr = null)
  {
    id = name;
    type = TFloat;
    v_float = data;
    x_linked = xptr;

    if(x_linked)
      v_float = *((float*)x_linked);
  }

  kgmVariable(kgmString name, kgmString data, void* xptr = null)
  {
    id = name;
    type = TString;
    v_string = data;
    x_linked = xptr;

    if(x_linked)
      v_string = *((kgmString*)x_linked);
  }

  kgmVariable(kgmString name, s32 data, void* xptr = null)
  {
    id = name;
    type = TInteger;
    v_integer = data;
    x_linked = xptr;

    if(x_linked)
      v_integer = *((s32*)x_linked);
  }

  kgmVariable(kgmString name, bool data, void* xptr = null)
  {
    id = name;
    type = TBoolean;
    v_boolean = data;
    x_linked = xptr;

    if(x_linked)
      v_boolean = *((bool*)x_linked);
  }

  kgmVariable(kgmString name, void* data, void* xptr = null)
  {
    id = name;
    type = TPointer;
    v_pointer = data;
    x_linked = xptr;
  }

  void* getXlinked()
  {
    return x_linked;
  }

  Type getType()
  {
    return type;
  }

  kgmString getName()
  {
    return id;
  }

  kgmString toString()
  {
    switch((u32) type)
    {
    case TFloat:
      return kgmConvert::toString(v_float);
    case TInteger:
      return kgmConvert::toString(v_integer);
    case TBoolean:
      return (v_boolean == false) ? kgmString("false") : kgmString("true");
    case TPointer:
      return kgmConvert::toString((s32)(size_t)v_pointer);
    case TString:
      return v_string;
    }

    return "";
  }

  kgmString getString()
  {
    return v_string;
  }

  s32 getInteger()
  {
    return v_integer;
  }

  f32 getFloat()
  {
    return v_float;
  }

  bool getBoolean()
  {
    return v_boolean;
  }

  void* getPointer()
  {
    return v_pointer;
  }

  void setFloat(f32 val)
  {
    if(type == TFloat)
      v_float = val;

    if(x_linked)
      *((float*)x_linked) = val;
  }

  void setInteger(s32 val)
  {
    if(type == TInteger)
      v_integer = val;

    if(x_linked)
      *((s32*)x_linked) = val;
  }

  void setString(str val)
  {
    if(type == TString)
      v_string = val;

    if(x_linked)
      *((kgmString*)x_linked) = val;
  }

  void setBoolean(bool val)
  {
    if(type == TBoolean)
      v_boolean = val;

    if(x_linked)
      *((bool*)x_linked) = val;
  }

  void setPointer(void* val)
  {
    if(type == TPointer)
      *((size_t*)v_pointer) = (size_t)val;
  }

  void sync(Sync s = SyncNone)
  {
    if(!x_linked || s == SyncNone)
      return;

    if(s == SyncToLink)
    {
      switch((u32) type)
      {
      case TFloat:
        *((f32*)x_linked) = v_float;
        break;
      case TInteger:
        *((s32*)x_linked) = v_integer;
        break;
      case TBoolean:
        *((bool*)x_linked) = v_boolean;
        break;
      case TPointer:
        *((size_t*)x_linked) = (size_t)v_pointer;
        break;
      case TString:
        *((kgmString*)x_linked) = v_string;
        break;
      }
    }
    else if(s == SyncFromLink)
    {
      switch((u32) type)
      {
      case TFloat:
        v_float = *((f32*)x_linked);
        break;
      case TInteger:
        v_integer = *((s32*)x_linked);
        break;
      case TBoolean:
        v_boolean = *((bool*)x_linked);
        break;
      case TPointer:
        v_pointer = (void*)*((size_t*)x_linked);
        break;
      case TString:
        v_string = *((kgmString*)x_linked);
        break;
      }
    }
  }

  void fromString(kgmString s)
  {
    switch((u32) type)
    {
    case TFloat:
      v_float = kgmConvert::toDouble(s);
      break;
    case TInteger:
      v_integer = kgmConvert::toInteger(s);
      break;
    case TBoolean:
      v_boolean = (s == "false") ? (false) : (true);
      break;
    case TPointer:
      v_pointer = (void*) (size_t) kgmConvert::toInteger(s);
      break;
    case TString:
      v_string = s;
      break;
    }
  }
};

#endif // KGMVARIABLE_H
