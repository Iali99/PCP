#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <pthread.h>

void *ThreadFunc(void * args);
bool isPrime(int n);
int m,n;

using namespace std;

struct ThreadArgs {
  int id;
};

int main() {

  cin >> n;
  cin >> m;
  ThreadArgs *args;

  pthread_t *threadid = (pthread_t *)malloc(sizeof(pthread_t)*m);
  for(int i =0;i<m;i++) {
    //printf("i = %d \n", i);
    args = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
    args->id = i;
    pthread_create(&threadid[i],NULL,ThreadFunc,(void *)args);
  }
  for(int i =0;i<m;i++) {
    pthread_join(threadid[i],NULL);
  }
  pthread_exit(NULL);
  return 0;

}

void *ThreadFunc(void * args) {
  int id = ((struct ThreadArgs *) args) -> id + 1;
  free(args);
  //printf("MY id is %d\n",id );
  long int a = id ;
  while(a < n) {
    if(isPrime(a)) {
      printf("%ld by thread %d\n",a,id );
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
