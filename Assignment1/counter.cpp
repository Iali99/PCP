#include <mutex>

using namespace std;

class Counter {
  private :
  long value;
  mutex mutexLock;

  public :
  Counter(int i) {
    value = i;
  }

  long getAndIncrement() {
    long temp;
    mutexLock.lock();
    temp = value;
    value++;
    mutexLock.unlock();
    return temp;
  }
};
