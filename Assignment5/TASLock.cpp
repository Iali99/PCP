#include <atomic>

using namespace std;

class TASLock {
  private:
    atomic<bool> state;

  public:
    TASLock(){
      state.store(false);
    }

    void lock() {
      while(state.exchange(true)){}
    }

    void unlock() {
      state.store(false);
    }
};
