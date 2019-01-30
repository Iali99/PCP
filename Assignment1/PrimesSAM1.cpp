#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <bits/stdc++.h>
#include <fstream>
#include <pthread.h>

void *ThreadFunc(void * args);
bool isPrime(int n);
int m,n;
long int upperLimit;
std::vector<long int> result;
std::mutex mutexLock;
std::fstream output,times;

using namespace std;

struct ThreadArgs {
  int id;
};

int main() {

  cin >> n;
  upperLimit = pow(10,n);
  cin >> m;
  ThreadArgs *args;
  output.open("Primes-SAM1.txt",fstream::out);
  times.open("Times.txt",fstream::app);

  clock_t begin,end;
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
  times << "SAM1 : n = " << n << ", m = " << m << ", time = " << elapsed_secs << " seconds\n";
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
  long int a = id ;
  while(a < upperLimit) {
    if(isPrime(a)) {
      mutexLock.lock();
      result.push_back(a);
      mutexLock.unlock();
    }
    a += m;
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
