#include "StampedValue.cpp"
#include "AtomicMRSW.cpp"
#include <stdlib.h>
#include <limits.h>

class AtomicMRMWRegister {
  private:
    StampedValue<AtomicMRSWRegister> *a_table;
    int capacity;

  public:
    void init(int capacity, int i){
      this->capacity = capacity;
      a_table = (StampedValue<AtomicMRSWRegister> *)malloc(sizeof(StampedValue<AtomicMRSWRegister>)*capacity);
      AtomicMRSWRegister init;
      init.init(i,capacity);
      StampedValue<AtomicMRSWRegister> value(init);
      for(int j = 0;j< capacity;j++) {
        a_table[j] = value;
      }
    }

    void write(int value,int threadID) {
      StampedValue<AtomicMRSWRegister> max(NULL);
      for(int i = 0; i < capacity;i++){
        max = StampedValue::max(max, a_table[i]);
      }
      AtomicMRSWRegister va;
      v.init(value,capacity);
      StampedValue<AtomicMRSWRegister> v(max.stamp + 1, va);
      a_table[threadID] = v;
    }

    int read() {
      StampedValue<AtomicMRSWRegister> max(NULL);
      for(int i =0;i< capacity;i++) {
        max = StampedValue::max(max,a_table[i]);
      }
      return max.value.read();
    }
};
