#include <set>
using namespace std;

template <typename T>
class REGtype {
  public:
    T value;
    int thread;
    int sn;

    REGtype(){}

    void set(T v, int i, int sn){
      this->value = v;
      this->thread = i;
      this->sn = sn;
    }

    void operator =(const REGtype &t) {
      value = t.value;
      thread = t.thread;
      sn = t.sn;
    }

   bool operator !=(const REGtype &t){
     if(value != t.value || thread != t.thread || sn != t.sn)
      return true;
     return false;
   }
};

template <typename T>
class WFSnapshotMRMW {
  private :
    int * sn;
    REGtype<T> * reg;
    T ** helpSnap;
    int length;

  public:
    WFSnapshotMRMW(){
    }

    void init(int capacity, T init){
      this->length = capacity;
      sn = new int[capacity];
      reg = new REGtype<T>[capacity];
      helpSnap = new T*[capacity];
      for(int i=0;i<capacity;i++)
        helpSnap[i] = new T[capacity];
      for(int i =0;i<capacity;i++){
        sn[i] = 0;
        reg[i].set(init,-1,0);
        for(int j = 0;j<capacity;j++)
          helpSnap[i][j] = init;
      }
    }

    void update(int threadid, T v){
      sn[threadid]++;
      reg[threadid].set(v,threadid,sn[threadid]);
      helpSnap[threadid] = scan();
    }

    T* scan(){
      set<int> can_help;
      REGtype<T> * aa = new REGtype<T>[length];
      for(int i = 0;i<length;i++)
        aa[i] = reg[i];
      while(true){
        REGtype<T> * bb = new REGtype<T>[length];
        for(int i = 0;i<length;i++)
          bb[i] = reg[i];
        bool flag = true;
        for(int i =0;i<length;i++){
          if(aa[i] != bb[i]){
            flag = false;
            break;
          }
        }
        if(flag == true){
          T* temp = new T[length];
          for(int i =0;i<length;i++)
            temp[i] = bb[i].value;
          return temp;
        }
        for(int i = 0;i<length;i++){
          if(bb[i] != aa[i]){
            int w = bb[i].thread;
            if(can_help.count(w))
              return helpSnap[w];
            else
              can_help.insert(w);
          }
        }
        for(int i =0;i<length;i++)
            aa[i] = bb[i];
      }
    }
};
