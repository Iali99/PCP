
#include "AtomicSRSW.cpp"

template <typename T>
class AtomicMRSWRegister {
  private:
    AtomicSRSWRegister<StampedValue<T>> **a_table;
    int length;

  public:
    long lastStamp;

    void init(T init, int readers){
      lastStamp = 0;
      length = readers;
      a_table = new AtomicSRSWRegister<StampedValue<T>> *[readers];

      for (int i=0; i<readers; i++)
          a_table[i] = new AtomicSRSWRegister<StampedValue<T>>[readers];

      for(int i =0;i<readers;i++)
        for(int j =0;j<readers;j++)
          a_table[i][j].init(init);

    }

    T read(int threadID) {
      StampedValue<T> value = a_table[threadID][threadID].read();
      for(int i =0; i<length; i++){
        auto x = a_table[i][threadID].read();
        value = (value > x) ? value : x;
      }

      for(int i =0; i<length; i++)
        a_table[threadID][i].write(value);
      return value.value;
    }

    void write(T v) {
      long stamp = lastStamp + 1;
      lastStamp = stamp;
      StampedValue<T> temp(stamp,v);
      
      for(int i =0; i<length; i++)
        a_table[i][i].write(temp);
    }

};
