#include <atomic>

using namespace std;

class TTASLock {
  private:
    atomic<bool> state;
  public:
    void lock() {
      while (true) {
        while (state.load()) {};
        if (!state.exchange(true))
        return;
      }
    }
    void unlock() {
      state.store(false);
    }
};
