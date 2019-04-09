#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include <random>
#include <stdio.h>


#include "TASLock.cpp"

using namespace std;

struct ThreadArgs {
  int id;
};

void* threadFunction(void * args);
TASLock taslock;
int n,k,l1,l2;
double t1,t2;
double avg;
FILE * output;
std::chrono::time_point<std::chrono::system_clock> start;

int main() {

  cin >> n;
  cin >> k;
  cin >> l1;
  cin >> l2;

  output = fopen("TAS-Output.txt","w");
  std::default_random_engine generator1;
  std::default_random_engine generator2;
  std::exponential_distribution<double> distribution(l1);
  t1 = distribution(generator1);
  // printf("t1 = %lf\n",t1 );
  std::exponential_distribution<double> distribution2(l2);
  t2 = distribution2(generator2);
  // printf("t2 = %lf\n",t2 );
  avg = 0;


  start = std::chrono::system_clock::now();
  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t)*n);
  for(int i =0;i<n;i++){
    struct ThreadArgs *args = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
    args->id = i;
    pthread_create(&threads[i],NULL,threadFunction,(void *)args);
  }
  for(int i =0;i<n;i++) {
    pthread_join(threads[i],NULL);
  }

  avg /= n*k;
  // printf("%lf\n",avg );


  return 0;
}

void * threadFunction(void * args) {
  int id = ((struct ThreadArgs *) args) -> id ;
  double time;
  for(int i =0;i<k;i++) {
    auto current1 = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(current1 - start).count();

    fprintf(output,"%dth CS Request at %lf seconds by thread %d\n",i,time,id );
    taslock.lock();
    auto current2 = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(current2 - start).count();


    time = std::chrono::duration_cast<std::chrono::microseconds>(current2 - current1).count();
    avg += time;

    auto current4 = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(current4 - start).count();
    fprintf(output,"%dth CS Entry at %lf seconds by thread %d\n",i,time,id );
    usleep(t1);
    auto current3 = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(current3 - start).count();
    fprintf(output,"%dth CS Exit at %lf seconds by thread %d\n",i,time,id );
    taslock.unlock();

    usleep(t2);
  }
}
