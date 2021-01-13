//
//  RXAtomic.h
//  rivenx
//

#if !defined(RX_ATOMIC_H)
#define RX_ATOMIC_H

#include "RXBase.h"
#include <atomic>

__BEGIN_DECLS

#if __LP64__
typedef int64_t atomic_int_t;
#else
typedef int32_t atomic_int_t;
#endif

#if __has_builtin(__sync_swap)
#define rx_atomic_xchg(p, n) ((__typeof__(*(p)))__sync_swap((p), (n)))
#else
#define rx_atomic_xchg(p, n) ((__typeof__(*(p)))__sync_lock_test_and_set((p), (n)))
#endif

RX_INLINE bool RX_compare_and_swap(atomic_int_t oldvalue, atomic_int_t newvalue, atomic_int_t* pvalue)
{
#if __LP64__
  return std::atomic_compare_exchange_strong_explicit((volatile std::atomic_int64_t*)pvalue, &oldvalue, newvalue, std::memory_order_seq_cst, std::memory_order_relaxed);
#else
  return OSAtomicCompareAndSwap32Barrier(oldvalue, newvalue, pvalue);
#endif
}

__END_DECLS

#endif // RX_ATOMIC_H
