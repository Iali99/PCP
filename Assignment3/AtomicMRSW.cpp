
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
      // AtomicSRSWRegister in(i);
      printf("ba\n" );
      // a_table = (AtomicSRSWRegister<StampedValue<T>> **)malloc(readers * sizeof(AtomicSRSWRegister<StampedValue<T>> *));
      a_table = new AtomicSRSWRegister<StampedValue<T>> *[readers];
      printf("b\n" );
      for (int i=0; i<readers; i++)
          a_table[i] = new AtomicSRSWRegister<StampedValue<T>>[readers];
         // a_table[i] = (AtomicSRSWRegister<StampedValue<T>> *)malloc(readers * sizeof(AtomicSRSWRegister<StampedValue<T>> *));
      printf("c\n" );
      // StampedValue<AtomicSRSWRegister> value(in);
      // AtomicSRSWRegister<StampedValue<T>> value(i);
      printf("a\n" );
      for(int i =0;i<readers;i++)
        for(int j =0;j<readers;j++)
          a_table[i][j].init(init);
      printf("a\n" );
    }

    T read(int threadID) {
      // printf("b\n" );
      StampedValue<T> value = a_table[threadID][threadID].read();
      for(int i =0; i<length; i++){
        // printf("b\n" );
        auto x = a_table[i][threadID].read();
        value = (value > x) ? value : x;
        // value = StampedValue<T>::max(value,x);
      }

        // value = StampedValue<AtomicSRSWRegister>::max(value,a_table[i][threadID]);
      for(int i =0; i<length; i++)
        a_table[threadID][i].write(value);
      return value.value;
    }

    void write(T v) {
      long stamp = lastStamp + 1;
      lastStamp = stamp;
      // AtomicSRSWRegister r(v);
      // StampedValue<AtomicSRSWRegister> value(stamp,r);
      for(int i =0; i<length; i++)
        a_table[i][i].write(v);
    }

};
