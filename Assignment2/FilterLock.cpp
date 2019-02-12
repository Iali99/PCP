class FilterLock {
   int n;
   int *level;
   int *victim;


  public :
  FilterLock() {
    this->n = 0;
  }
  
  void setDefaults(int n) {
     this->n = n;
     level = (int *)malloc(sizeof(int)*n);
     victim = (int *)malloc(sizeof(int)*n);
     for(int i =0;i<n;i++){
       level[i] = 0;
     }
   }

   void lock(int threadID) {
     for(int i = 1;i<n;i++) {
       level[threadID] = i;
       victim[i] = threadID;

       bool flag = true;
       while(flag) {
         flag = false;
         for(int k =0;k<n;k++)
           if(k!=threadID && level[k] >= i && victim[i] == threadID){
             flag = true;
             break;
           }
       }
     }
   }

   void unlock(int threadID) {
     level[threadID] = 0;
   }
};
