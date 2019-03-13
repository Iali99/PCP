#include "StampedValue.cpp"

template <typename T>
class AtomicSRSWRegister {
  public :
    long lastStamp;
    StampedValue<T> lastRead;
    StampedValue<T> r_value; // regular SRSW timestamp-value pair

    AtomicSRSWRegister(){
      lastStamp = 0;
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
      StampedValue<T> value = r_value;
      StampedValue<T> last = lastRead;
      StampedValue<T> result = StampedValue<T>::max(value,last);

      lastRead.set(result.stamp,result.value);
      return result.value;
    }

    void write(T v) {
      long stamp = lastStamp + 1;
      r_value.set(stamp,v);
      lastStamp = stamp;
    }
};
