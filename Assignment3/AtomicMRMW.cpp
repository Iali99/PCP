#include "StampedValue.cpp"
#include <stdlib.h>
#include <limits.h>

class AtomicMRMWRegister {
  private:
    StampedValue *a_table;
    int capacity;

  public:
    void init(int capacity, int i){
      this->capacity = capacity;
      a_table = (StampedValue *)malloc(sizeof(StampedValue)*capacity);
      StampedValue value(i);
      for(int j = 0;j< capacity;j++) {
        a_table[j] = value;
      }
    }

    void write(int value,int threadID) {
      StampedValue max(INT_MIN);
      for(int i = 0; i < capacity;i++){
        max = StampedValue::max(max, a_table[i]);
      }
      StampedValue v(max.stamp + 1, value);
      a_table[threadID] = v;
    }

    int read() {
      StampedValue max(INT_MIN);
      for(int i =0;i< capacity;i++) {
        max = StampedValue::max(max,a_table[i]);
      }
      return max.value;
    }
};
