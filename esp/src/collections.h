#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <Arduino.h>

template <typename T> class Array{
private:
  int _size;
  T* data;
public:
  Array(int size = 2){
    this->_size = size;
    this->data = new T[size];
  }

  T& at(int el){
    return this->operator[] (el);
  }

  const T & at(int el) const{
    return this->operator[] (el);
  }

  T& operator[] (int el) {
    if(el >= _size){
      int newSize = _size<<1;
      while(newSize <= el) newSize <<= 1;
      T* newData = new T[newSize];
      memcpy(newData, data, sizeof(T)*_size);
      delete [] data;
      data = newData;
      _size = newSize;
    }
    return data[el];
  }

  const T & operator[](int el) const {
    return data[el];
  }

  void trim(int n){
    T* newData = new T[n];
    memcpy(newData, data, sizeof(T)*n);
    delete [] data;
    data = newData;
    _size = n;
  }

  void sort(int(*compar)(const void*, const void*), int size){
    qsort(this->data, size, sizeof(T), compar);
  }

  int size(){
    return _size;
  }

  ~Array(){
    delete [] data;
  }
};

#endif //COLLECTIONS_H
