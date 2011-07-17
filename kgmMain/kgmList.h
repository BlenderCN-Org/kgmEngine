#pragma once 
#include "kgmObject.h"
#include "malloc.h"

typedef unsigned int uint;

template <class T> class kgmList;
template <class T> class kgmArray;

template <class T> 
class kgmList{
private:
 struct _Node{
  T data;
  _Node *prev, *next;

  _Node(){ prev = next = 0; }
  inline void* operator new(unsigned int size){
   return malloc(size);
  }
  inline void  operator delete(void *p){
   if(p) free(p);
  }
 };


public:
//// ITERATOR
 class iterator{
  friend class kgmList<T>;
  _Node* _Ptr;
 public:
  iterator(){ _Ptr = 0; }
  T& operator*(){
   return _Ptr->data;
  }
  void operator++(){
   if(_Ptr) _Ptr = _Ptr->next;
  }
  void operator--(){
   if(_Ptr) _Ptr = _Ptr->prev;
  }
  bool operator!=(iterator i){
   return (_Ptr != i._Ptr);
  }
  bool operator==(iterator i){
   return (_Ptr == i._Ptr);
  }
 };
//// *********************
 protected:
 _Node *_First;
 uint csize;


 _Node *_first(){
  return _First;
 }
 _Node *_last(){
  _Node *node = _First;
  while(node && node->next) node = node->next;
  return node;
 }

public:
 kgmList():_First(0),csize(0){
 }
 ~kgmList(){
//  clear();
 }

 T& operator[](unsigned int i){
  _Node *node = _First;
  while(node && i){
   node = node->next;
   i--;
  }
  return node->data;
 }
 int size(){
  return csize;
 }
 int length(){
  return csize;
 }
 void push_back(T t){
  add(t);
 }
 void add(T t){
  _Node *node = new _Node();
  node->data = t;
  _Node *lnode = _last();
  if(!lnode)
   _First = node;
  else{
   node->prev = lnode;
   lnode->next = node;
  }
  csize++;
 }
 void append(T t){
  _Node *node = new _Node();
  node->data = t;
  _Node *lnode = _last();
  if(!lnode)
   _First = node;
  else{
   node->prev = lnode;
   lnode->next = node;
  }
  csize++;
 }
 iterator erase(iterator i){
  if(!i._Ptr) return i;
  _Node *prev, *next;
  prev = i._Ptr->prev;
  next = i._Ptr->next;
  if(prev) prev->next = next;
  if(next) next->prev = prev;
  delete i._Ptr;
  if(i._Ptr == _First)
   _First = i._Ptr = next;
  else
   i._Ptr = next;
  csize--;
  return i;
 }
 void erase(int i){
  _Node *node = _First;
  if(i >= csize)
	  return;
  while(node && i){
   node = node->next;
   i--;
  }
  if(node){
   if(node->prev)
    node->prev->next = node->next;
   if(node->next)
    node->next->prev = node->prev;
   if(node == _First)
    _First = 0;
   delete node;
   csize--;
  }
 }
 void clear(){
  _Node *node = _First;
  while(node){
   _Node *prev = node;
   node = node->next;
   delete prev;
  }
  _First = 0;
  csize = 0;
 }
 iterator begin(){
  iterator i;
  i._Ptr = _First;
  return i;
 }
 iterator end(){
  iterator i;
  i._Ptr = 0;
  return i;
 }
};

