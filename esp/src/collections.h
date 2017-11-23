
template <typename T> class Array{
private:
  int size;
  T* data;
  bool ifDeleteElements;
public:
  Array(int size, bool ifDeleteElements=false){
    this->size = size;
    this->ifDeleteElements = ifDeleteElements;
    this->data = new T[size];
    clear();
  }

  T& operator[] (int el) {
    if(el >= size){
      int newSize = size<<1;
      while(newSize <= el) newSize <<= 1;
      T* newData = new T[newSize];
      memset(newData, 0, sizeof(T)*newSize);
      memcpy(newData, data, sizeof(T)*size);
      delete [] data;
      data = newData;
      size = newSize;
    }
    return data[el];
  }

  const T & operator[](int el) const {
    return data[el];
  }

  void deleteElements(){
    for(int i = 0; i < size; i++){
      if(data[i] != 0){
        delete data[i];
      }
    }
  }

  void clear(){
    memset(data, 0, sizeof(T)*size);
  }

  ~Array(){
    if(ifDeleteElements){
      deleteElements();
    }
    delete [] data;
  }
};
