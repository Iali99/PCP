#include "StampedValue.cpp"

// template <typename T>
class AtomicSRSWRegister {
  public :
    thread_local long lastStamp;
    thread_local StampedValue<int> lastRead;
    StampedValue<int> r_value; // regular SRSW timestamp-value pair

    void init(int i) {
      r_value.init(i);
      lastStamp = 0;
      lastRead.init(i);
    }

    int read() {
      StampedValue value = r_value;
      StampedValue last = lastRead;
      StampedValue result = StampedValue::max(value,last);
      lastRead.set(result.stamp,result.value);
      return result.value;
    }

    void write(int v) {
      long stamp = lastStamp + 1;
      r_value.set(stamp,v);
      lastStamp = stamp;
    }
}
