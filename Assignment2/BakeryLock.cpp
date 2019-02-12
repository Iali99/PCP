class BakeryLock {
  int n;
  bool * flag;
  long long int *label;

  public:
  BakeryLock(){
    this->n = 0;
  }

  void setDefaults(int n) {
    this->n = n;
    flag = (bool *)malloc(sizeof(bool)*n);
    label = (long long int *)malloc(sizeof(long long int)*n);
    for(int i =0;i<n;i++){
      flag[i] = false;
      label[i] = 0;
    }
  }

  void lock(int threadID) {
    flag[threadID] = true;
    for(int i =0;i<n;i++){
      if(label[threadID] > label[i])
        label[threadID] = label[i];
    }
    label[threadID] += 1;

    bool f = true;
    while(f) {
      f = false;
      for(int k =0;k<n;k++)
        if(k!=threadID && flag[k] && (label[k] < label[threadID] || (label[k] == label[threadID] && k < threadID))){
          f = true;
          break;
        }
    }
  }

  void unlock(int threadID) {
    flag[threadID] = false;
  }
};
