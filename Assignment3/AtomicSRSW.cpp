#include "StampedValue.cpp"

template <typename T>
class AtomicSRSWRegister {
  public :
    long lastStamp;
    StampedValue<T> lastRead;
    StampedValue<T> r_value; // regular SRSW timestamp-value pair

    AtomicSRSWRegister(){
      // r_value.init(0);
      lastStamp = 0;
      // lastRead.init(0);
    }
    AtomicSRSWRegister(T i){
      r_value.init(i);
      lastStamp = 0;
      lastRead.init(i);
    }

    void init(T i) {
      r_value.init(i);
      lastStamp = 0;
      lastRead.init(i);
    }

    T read() {
      // printf("c\n" );
      StampedValue<T> value = r_value;
      StampedValue<T> last = lastRead;
      // printf("c\n" );
      StampedValue<T> result = StampedValue<T>::max(value,last);
      // printf("d\n" );
      lastRead.set(result.stamp,result.value);
      return result.value;
    }

    void write(T v) {
      long stamp = lastStamp + 1;
      r_value.set(stamp,v);
      lastStamp = stamp;
    }
};
