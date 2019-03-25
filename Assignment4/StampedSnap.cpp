template <typename T>
class StampedSnap{
  public:
    long stamp;
    T value;
    T* snap;

    StampedSnap(){

    }

    StampedSnap(T value){
      stamp = 0;
      this->value = value;
      snap = NULL;
    }

    StampedSnap(long label, T value, T* snap){
      this->stamp = label;
      this->value = value;
      this->snap = snap;
    }

    void init(T value){
      stamp = 0;
      this->value = value;
      snap = NULL;
    }

    void set(long label, T value, T* snap){
      this->stamp = label;
      this->value = value;
      this->snap = snap;
    }
};
