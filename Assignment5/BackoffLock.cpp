#include <atomic>
#include <stdlib.h>
#include<time.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))

using namespace std;

class Backoff{
  private:
    int minDelay, maxDelay;
    int limit;

  public:
    Backoff(int min, int max){
      minDelay = min;
      maxDelay = max;
      srand(time(0));
      limit = minDelay;
    }

    void backoff(){
      int delay = rand()%limit;
      limit = MIN(maxDelay,2*limit);
      usleep(delay);
    }
};

class BackoffLock {
  private:
    atomic<bool> state;
    int MIN_DELAY;
    int MAX_DELAY;

  public:
    BackoffLock(){}

    void init(int min, int max){
      state.store(false);
      this->MIN_DELAY = min;
      this->MAX_DELAY = max;
    }

    void lock(){
      Backoff backoff(this->MIN_DELAY,this->MAX_DELAY);
      while(true){
        while(state.load()) {};
        if(!state.exchange(true)){
          return;
        } else {
          backoff.backoff();
        }
      }
    }

    void unlock() {
      state.store(false);
    }
};
