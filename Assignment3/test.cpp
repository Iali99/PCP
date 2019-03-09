#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include <random>
#include <stdio.h>

#include "AtomicMRMW.cpp"

using namespace std;

struct ThreadArgs {
  int id;
};

void *testAtomic(void * args);

AtomicMRMWRegister<int> shVar;
int n,k,p;
std::chrono::time_point<std::chrono::system_clock> start;


int main(){
  cin >> n;
  cin >> k;
  cin >> p;

  shVar.init(n,0);

  start = std::chrono::system_clock::now();

  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t)*n);
  for(int i =0;i<n;i++){
    struct ThreadArgs *args = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
    args->id = i;
    pthread_create(&threads[i],NULL,testAtomic,(void *)args);
  }
  for(int i =0;i<n;i++) {
    pthread_join(threads[i],NULL);
  }

  return 0;
}

void * testAtomic(void * args){
  int id = ((struct ThreadArgs *) args) -> id ;
  int localVar;
  int action = 0;

  double time;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(p);

  for(int i= 0;i<k;i++){

    // action = d(gen);

    auto current = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(current - start).count();
    printf("%d th Action requested at %lf seconds by thread %d\n",i,time,id );

    if(action == 0) { // Read Action
      localVar = shVar.read(id);
      printf("Value Read by thread %d = %d\n",id,localVar );
    }
    else { // Write Action
      localVar = k * (id+1);
      shVar.write(localVar,id);
      printf("Value Written by thread %d = %d\n",id,localVar );
    }

    current = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(current - start).count();
    printf("%d th Action completed at %lf seconds by thread %d\n",i,time,id );

    action = 1 - action;
    usleep(20000);
  }
}
