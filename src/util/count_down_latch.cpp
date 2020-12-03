#include "count_down_latch.h"

namespace zoo {
namespace kangaroon {
CountDownLatch::CountDownLatch(int count)
    : count_(count), mutex_(), condition_(mutex_) {}

void CountDownLatch::wait() {
    MutexGuard guard(mutex_);

    while (count_ > 0) {
        condition_.wait();
    }
}

void CountDownLatch::countDown() {
    MutexGuard guard(mutex_);

    count_--;
    if (count_ == 0) {
        condition_.notifyOne();
    }
}

}  // namespace kangaroon

}  // namespace zoo
