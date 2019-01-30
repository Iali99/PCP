#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <pthread.h>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include "counter.cpp"

void *ThreadFunc(void * args);
bool isPrime(int n);
int m,n;
long int upperLimit;
vector<long int> result;
mutex mutexLock;
fstream output,times;
Counter c(1);

using namespace std;

struct ThreadArgs {
  int id;
};

int main() {

  cin >> n;
  upperLimit = pow(10,n);
  //cout << "N = " << upperLimit;
  cin >> m;
  ThreadArgs *args;
  clock_t begin,end;
  output.open("Primes-DAM.txt",fstream::out);
  times.open("Times.txt",fstream::app);
  pthread_t *threadid = (pthread_t *)malloc(sizeof(pthread_t)*m);
  begin = clock();
  for(int i =0;i<m;i++) {
    //printf("i = %d \n", i);
    args = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
    args->id = i;
    pthread_create(&threadid[i],NULL,ThreadFunc,(void *)args);
  }
  for(int i =0;i<m;i++) {
    pthread_join(threadid[i],NULL);
  }
  end = clock();
  double elapsed_secs = double(end - begin)/ CLOCKS_PER_SEC;
  times << "DAM : n = " << n << ", m = " << m << ", time = " << elapsed_secs << " seconds\n";
  sort(result.begin(),result.end());
  for(auto x: result) {
    output << x << " " ;
  }
  output.close();
  times.close();
  pthread_exit(NULL);
  return 0;

}

void *ThreadFunc(void * args) {
  int id = ((struct ThreadArgs *) args) -> id + 1;
  free(args);
  //printf("MY id is %d\n",id );
  long int i = c.getAndIncrement();;
  long int limit = upperLimit ;
  while(i < limit) {

    if(isPrime(i)) {
      //printf("%ld by thread %d\n",i,id );
      mutexLock.lock();
      result.push_back(i);
      mutexLock.unlock();
    }
    i = c.getAndIncrement();
  }
}

bool isPrime(int n)
{
  // Corner case
  if (n <= 1)
  return false;

  // Check from 2 to n-1
  for (int i = 2; i <= sqrt(n); i++)
  if (n % i == 0)
  return false;

  return true;
}
