
#include "AtomicMRSW.cpp"
#include <stdlib.h>
#include <limits.h>

template <typename T>
class AtomicMRMWRegister {
  private:
    AtomicMRSWRegister<StampedValue<T>> *a_table;
    int capacity;

  public:
    void init(int capacity, T i){
      this->capacity = capacity;
      a_table = new AtomicMRSWRegister<StampedValue<T>>[capacity];

      for(int j=0; j< capacity; j++)
        a_table[j].init(i,capacity);

      for(int j = 0;j< capacity;j++) {
        a_table[j].write(i);
      }
    }

    void write(T value,int threadID) {

      StampedValue<T> max;
      for(int i = 0; i < capacity;i++){
        max = StampedValue<T>::max(max, a_table[i].read(threadID));
      }

      StampedValue<T> v(max.stamp + 1, value);
      a_table[threadID].write(v);
    }

    T read(int threadID) {
      StampedValue<T> max;
      for(int i =0;i< capacity;i++) {
        max = StampedValue<T>::max(max,a_table[i].read(threadID));
      }
      return max.value;
    }
};
