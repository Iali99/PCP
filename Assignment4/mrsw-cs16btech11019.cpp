#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include <random>
#include <stdio.h>
#include <atomic>
#include "MRSWSnapshot.cpp"

using namespace std;

template<class T>
class CopyableAtomic : public std::atomic<T>
{
public:
    //defaultinitializes value
    CopyableAtomic() :
        std::atomic<T>(T{})
    {}

    constexpr CopyableAtomic(T desired) :
        std::atomic<T>(desired)
    {}

    constexpr CopyableAtomic(const CopyableAtomic<T>& other) :
        CopyableAtomic(other.load(std::memory_order_relaxed))
    {}

    CopyableAtomic& operator=(const CopyableAtomic<T>& other) {
        this->store(other.load(std::memory_order_relaxed), std::memory_order_relaxed);
        return *this;
    }
};

//Global Variables
WFSnapshotMRSW<CopyableAtomic<int> > snapshotObj;
// atomic<int> *a_table;
atomic<bool> term;
int n,k,l1,l2;
double t1,t2;
std::chrono::time_point<std::chrono::system_clock> start;
FILE * output;

struct ThreadArgs {
  int id;
};
//Thread Functions
void* writer(void* args);
void* snapshot(void* args);

int main(){
  cin >> n;
  cin >> l1;
  cin >> l2;
  cin >> k;

  std::default_random_engine generator1;
  std::exponential_distribution<double> distribution1(l1);
  t1 = distribution1(generator1);

  std::default_random_engine generator2;
  std::exponential_distribution<double> distribution2(l2);
  t2 = distribution2(generator2);

  output = fopen("Log-MRSW.txt","w");
  // a_table = new atomic<int>[n];
  // for(int i = 0;i<n;i++)
  //   a_table[i] = 0;
  snapshotObj.init(n,0);

  start = std::chrono::system_clock::now();

  pthread_t *writerThreads = (pthread_t *)malloc(sizeof(pthread_t)*n);
  for(int i =0;i<n;i++){
    struct ThreadArgs *args = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
    args->id = i;
    pthread_create(&writerThreads[i],NULL,writer,(void *)args);
  }

  pthread_t snapshotThread;
  pthread_create(&snapshotThread,NULL,snapshot,(void *)1);

  pthread_join(snapshotThread,NULL);
}

void* writer(void *args){
  int id = ((struct ThreadArgs *) args) -> id ;
  int v = 1;
  double time;
  while(!term){
    v = v*(id+1) + 1;
    auto current = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::nanoseconds>(current - start).count();
    fprintf(output,"Thread %d starting to write value %d at time %lf\n",id,v,time );
    // a_table[id].store(v);
    snapshotObj.update(id,v);
    current = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::nanoseconds>(current - start).count();
    fprintf(output,"Thread %d finished writing value %d at time %lf\n",id,v,time );

    usleep(t1);
  }
}

void* snapshot(void* args){
  int i = 0;

  while(i<k){
    auto beginCollect = std::chrono::system_clock::now();
    fprintf(output,"Starting to collect %dth snapshot\n",i );
    CopyableAtomic<int> *temp = snapshotObj.scan();
    fprintf(output,"Finished collecting %dth snapshot\n",i );
    auto endCollect = std::chrono::system_clock::now();
    fprintf(output,"%dth snapshot = ",i );
    for(int j = 0;j<n;j++)
      fprintf(output,"%d ",temp[j].load() );
    fprintf(output,"\n" );
    usleep(t2);
    i++;
  }
  term = true;

}
