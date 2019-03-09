
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
      // printf("size of int = %d\n",sizeof(T) );
      // a_table = (AtomicMRSWRegister<StampedValue<T>> *)malloc(sizeof(AtomicMRSWRegister<StampedValue<T>>)*capacity);
      a_table = new AtomicMRSWRegister<StampedValue<T>>[capacity];
      printf("a\n" );
      for(int j=0; j< capacity; j++)
        a_table[j].init(i,capacity);

      // AtomicMRSWRegister init;
      // init.init(i,capacity);
      // StampedValue<AtomicMRSWRegister> value(init);
      for(int j = 0;j< capacity;j++) {
        a_table[j].write(i);
      }
    }

    void write(T value,int threadID) {
      // AtomicMRSWRegister r;
      // r.init(0,0);
      StampedValue<T> max;
      for(int i = 0; i < capacity;i++){
        max = StampedValue<T>::max(max, a_table[i].read(threadID));
      }
      // AtomicMRSWRegister va;
      // va.init(value,capacity);
      StampedValue<T> v(max.stamp + 1, value);
      a_table[threadID].write(v);
    }

    T read(int threadID) {
      // AtomicMRSWRegister r;
      // r.init(0,0);
      StampedValue<T> max;
      for(int i =0;i< capacity;i++) {
        // printf("a\n" );
        max = StampedValue<T>::max(max,a_table[i].read(threadID));
      }
      return max.value;
    }
};
