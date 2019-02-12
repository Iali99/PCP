#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <chrono>

#include "BakeryLock.cpp"

using namespace std;

struct ThreadArgs {
  int id;
};

void* threadFunction(void * args);
BakeryLock bkLock;
int n,k;
std::chrono::time_point<std::chrono::system_clock> start;

int main() {

  cin >> n;
  cin >> k;
  bkLock.setDefaults(n);
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
  return 0;
}

void * threadFunction(void * args) {
  int id = ((struct ThreadArgs *) args) -> id ;
  std::chrono::time_point<std::chrono::system_clock> current;
  std::chrono::duration<double> time;
  for(int i =0;i<k;i++) {
    current = std::chrono::system_clock::now();
    time = (current - start);
    printf("%dth CS Request at %lf seconds by thread %d\n",i,time,id );
    bkLock.lock(id);
    current = std::chrono::system_clock::now();
    time = (current - start);
    printf("%dth CS Entry at %lf seconds by thread %d\n",i,time,id );
    usleep(10000);
    current = std::chrono::system_clock::now();
    time = (current - start);
    printf("%dth CS Exit at %lf seconds by thread %d\n",i,time,id );
    bkLock.unlock(id);

    usleep(20000);
  }
}
