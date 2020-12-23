#include "mutex.h"

namespace zoo {
namespace kangaroon {
void Mutex::lock() { pthread_mutex_lock(&mutex_); }
void Mutex::unlock() { pthread_mutex_unlock(&mutex_); }

MutexGuard::MutexGuard(Mutex& mutex) : mutex_(mutex) { mutex_.lock(); }
MutexGuard::~MutexGuard() { mutex_.unlock(); }
};  // namespace kangaroon
}  // namespace zoo