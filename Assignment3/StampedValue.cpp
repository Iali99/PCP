class StampedValue {

  public:
    long stamp;
    int value;
    
    StampedValue(int i) {
      stamp = 0;
      value = i;
    }

    StampedValue(long s,int i) {
      stamp = s;
      value = i;
    }

    void set(long s, int i) {
      stamp = s;
      value = i;
    }

    static StampedValue max(StampedValue x, StampedValue y){
      if(x.stamp > y.stamp)
        return x;
      else
        return y;
    }

    // static StampedValue MIN_VALUE =
};
