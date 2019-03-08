#include "StampedValue.cpp"
#include "AtomicSRSW.cpp"

class AtomicMRSWRegister {
  private:
    StampedValue<AtomicSRSWRegister> **a_table;
    int length;

  public:
    thread_local long lastStamp;

    void init(int i, int readers){
      lastStamp = 0;
      length = readers;
      AtomicSRSWRegister init;
      init.init(i);
      a_table = (StampedValue<AtomicSRSWRegister> **)malloc(readers * sizeof(StampedValue<AtomicSRSWRegister> *));
      for (i=0; i<readers; i++)
         a_table[i] = (StampedValue<AtomicSRSWRegister> *)malloc(readers * sizeof(StampedValue<AtomicSRSWRegister> *));
      StampedValue<AtomicSRSWRegister> value(init);
      for(int i =0;i<readers;i++)
        for(int j =0;j<readers;j++)
          a_table[i][j] = value;
    }

    int read(int threadID) {
      StampedValue<AtomicSRSWRegister> value = a_table[threadID][threadID];
      for(int i =0; i<length; i++)
        value = StampedValue::max(value,a_table[i][threadID]);
      for(int i =0; i<length; i++)
        a_table[threadID][i] = value;
      return value.value.read();
    }

    void write(int v) {
      long stamp = lastStamp + 1;
      lastStamp = stamp;
      AtomicSRSWRegister r;
      r.init(v);
      StampedValue<AtomicSRSWRegister> value(stamp,r);
      for(int i =0; i<length; i++)
        a_table[i][i] = value;
    }

}
