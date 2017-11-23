
template <typename T> class Array{
private:
  int _size;
  T* data;
public:
  Array(int size){
    this->_size = size;
    this->data = new T[size];
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

  int size(){
    return _size;
  }

  ~Array(){
    delete [] data;
  }
};