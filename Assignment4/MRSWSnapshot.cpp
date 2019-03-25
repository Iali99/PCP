#include "StampedSnap.cpp"

template <typename T>
class WFSnapshotMRSW {
  private:
    StampedSnap<T> * a_table;
    int lenght;

    StampedSnap<T> * collect() {
      StampedSnap<T> * copy = new StampedSnap<T>[lenght];
      for(int i =0;i<lenght;i++){
        copy[i] = a_table[i];
      }
      return copy;
    }

  public:
    WFSnapshotMRSW(){}

    WFSnapshotMRSW(int capacity, T init){
      this->lenght = capacity;
      a_table = new StampedSnap<T>[capacity];
      for(int i =0;i<capacity;i++){
        StampedSnap<T> temp(init);
        a_table[i] = temp;
      }
    }

    void init(int capacity, T init){
      this->lenght = capacity;
      a_table = new StampedSnap<T>[capacity];
      for(int i =0;i<capacity;i++){
        StampedSnap<T> temp(init);
        a_table[i] = temp;
      }
    }
    void update(int threadid, T value){
      T* snap = scan();
      StampedSnap<T> oldValue = a_table[threadid];
      StampedSnap<T> temp(oldValue.stamp +1, value, snap);
      StampedSnap<T> newValue = temp;
      a_table[threadid] = newValue;
    }

    T* scan() {
      StampedSnap<T>* oldCopy;
      StampedSnap<T>* newCopy;
      bool * moved = new bool[lenght];
      oldCopy = collect();

      collect:
        while(true) {
          newCopy = collect();
          for(int j = 0;j< lenght;j++){
            if(oldCopy[j].stamp != newCopy[j].stamp){
              if(moved[j]){
                return oldCopy[j].snap;
              }
              else{
                moved[j] = true;
                oldCopy = newCopy;
                goto collect;
              }
            }
          }
          T* result = new T[lenght];
          for(int j=0;j<lenght;j++)
            result[j] = newCopy[j].value;
          return result;
        }
    }
};
