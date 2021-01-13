//
//  osspinlock.h
//

#pragma once

#import "Base/cxx_policies.h"
#include <os/lock.h>

namespace rx {

class OSSpinlockGuard : public noncopyable
{
public:
	explicit OSSpinlockGuard(os_unfair_lock* lock) : lock_(lock) { os_unfair_lock_lock(lock_); }
	~OSSpinlockGuard() { os_unfair_lock_unlock(lock_); }
private:
  os_unfair_lock* lock_;
};

class OSSpinlock : public noncopyable
{
public:
  typedef os_unfair_lock_t native_handle_type;

  constexpr OSSpinlock() noexcept : lock_(OS_UNFAIR_LOCK_INIT) {}
  ~OSSpinlock() noexcept {}

  void lock() noexcept { os_unfair_lock_lock(&lock_); }
  bool try_lock() noexcept { return os_unfair_lock_trylock(&lock_); }
  void unlock() noexcept { os_unfair_lock_unlock(&lock_); }

  native_handle_type native_handle() { return &lock_; }

private:
  os_unfair_lock lock_;
};

} // namespace rx
