#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include <random>
#include <stdio.h>
#include <atomic>

#include "AtomicMRMW.cpp"

using namespace std;

struct ThreadArgs {
  int id;
};

void *testAtomic(void * args);
void * testAtomicInBuilt(void * args);

AtomicMRMWRegister<int> shVar;
atomic<int> inbuiltShVar;
int n,k,l1;
double p,t,timeImp, timeInbuilt;
double * avgTime;
double * inbuiltAvgTime;
FILE * output;
FILE * inbuiltOutput;


std::chrono::time_point<std::chrono::system_clock> start;


int main(){
  cin >> n;
  cin >> k;
  cin >> l1;
  cin >> p;

  output = fopen("Log-Output.txt","w");
  inbuiltOutput = fopen("inbuilt-Log-Output.txt","w");

  std::default_random_engine generator1;
  std::exponential_distribution<double> distribution(l1);
  t = distribution(generator1);

  shVar.init(n,0);
  inbuiltShVar.store(0);

  avgTime = (double *)malloc(sizeof(double)*n);
  inbuiltAvgTime = (double *)malloc(sizeof(double)*n);
  for(int i =0;i<n;i++)
  {
    avgTime[i] = 0;
    inbuiltAvgTime[i] =0;
  }

  timeImp = 0;
  timeInbuilt = 0;

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



  pthread_t *threads_at = (pthread_t *)malloc(sizeof(pthread_t)*n);
  for(int i =0;i<n;i++){
    struct ThreadArgs *args = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
    args->id = i;
    pthread_create(&threads_at[i],NULL,testAtomicInBuilt,(void *)args);
  }
  for(int i =0;i<n;i++) {
    pthread_join(threads_at[i],NULL);
  }

  for(int i = 0;i<n;i++){
    timeImp = avgTime[i];
    timeInbuilt = inbuiltAvgTime[i];
  }

  timeImp /= n*k;
  timeInbuilt /= n*k;
  // inbuiltAvgTime /= n*k;

  printf("Avg Time by my implementation = %lf\n",timeImp );
  printf("Avg Time by inbuilt implementation = %lf\n",timeInbuilt );
  fclose(output);
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

    action = d(gen);

    auto current = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::nanoseconds>(current - start).count();
    fprintf(output,"%d th Action requested at %lf seconds by thread %d\n",i,time,id );

    if(action == false) { // Read Action
      auto s = std::chrono::system_clock::now();
      localVar = shVar.read(id);
      auto e = std::chrono::system_clock::now();
      avgTime[id] += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();
      fprintf(output,"Value Read by thread %d = %d\n",id,localVar );
    }
    else { // Write Action

      localVar = i * (id+1);
      auto s = std::chrono::system_clock::now();
      shVar.write(localVar,id);
      auto e = std::chrono::system_clock::now();
      avgTime[id] += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();
      fprintf(output,"Value Written by thread %d = %d\n",id,localVar );
    }

    current = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::nanoseconds>(current - start).count();
    fprintf(output,"%d th Action completed at %lf seconds by thread %d\n",i,time,id );

    // action = 1 - action;
    usleep(t);
  }
}

void * testAtomicInBuilt(void * args){
  int id = ((struct ThreadArgs *) args) -> id ;
  int localVar;
  int action = 0;

  double time;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(p);

  for(int i= 0;i<k;i++){

    action = d(gen);

    auto current = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::nanoseconds>(current - start).count();
    fprintf(inbuiltOutput,"%d th Action requested at %lf seconds by thread %d\n",i,time,id );

    if(action == false) { // Read Action
      auto s = std::chrono::high_resolution_clock::now();
      localVar = inbuiltShVar.load();
      auto e = std::chrono::high_resolution_clock::now();

      inbuiltAvgTime[id] += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();
      fprintf(inbuiltOutput,"Value Read by thread %d = %d\n",id,localVar );
    }
    else { // Write Action
      localVar = i * (id+1);
      auto s = std::chrono::high_resolution_clock::now();
      inbuiltShVar.store(localVar);
      auto e = std::chrono::high_resolution_clock::now();

      inbuiltAvgTime[id] += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();
      fprintf(inbuiltOutput,"Value Written by thread %d = %d\n",id,localVar );
    }

    current = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::nanoseconds>(current - start).count();
    fprintf(inbuiltOutput,"%d th Action completed at %lf seconds by thread %d\n",i,time,id );

    // action = 1 - action;
    usleep(t);
  }
}
