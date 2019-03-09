template <class T>
class StampedValue {

  public:
    long stamp;
    T value;

    StampedValue(){
      stamp = 0;
    }

    StampedValue(T i) {
      stamp = 0;
      value = i;
    }

    StampedValue(long s,T i) {
      stamp = s;
      value = i;
    }

    void init(T i) {
      stamp = 0;
      value = i;
    }

    void set(long s, T i) {
      stamp = s;
      value = i;
    }

    static StampedValue max(StampedValue x, StampedValue y){
      if(x.stamp > y.stamp)
        return x;
      else
        return y;
    }

    bool operator >(const StampedValue& b) {
      if(this->stamp > b.stamp)
        return true;
      else
        return false;
    }
    // static StampedValue MIN_VALUE =
};
