/**
 * Flush+Reload tools.
 *
 * Originally called rb_tools, but becuase reload buffer is not a widespread term, we can
 * call it flush+reload
 */

#include <compiler.h>
#include <lib.h>
#include <toolkit/cache/lib.h>
#include <usermode.h>

/** L3 miss */
#ifndef CACHE_MISS_THRES
#define CACHE_MISS_THRES 120
#endif

#ifndef RB_PTR
// arbitrary ptr
#define RB_PTR 0xff1f200000ul
#endif

#ifndef RB_STRIDE
#define RB_STRIDE (1UL << 12)
#endif

#ifndef RB_SLOTS
#define RB_SLOTS 8
#endif

#ifndef SECRET
#define SECRET 6
#endif

#ifndef RB_HIST
// arbitrary ptr
#define RB_HIST _ptr(0xfffffff000222000)
#endif

static __always_inline void reload_range(long base, long stride, unsigned n,
                                         u64 *results) {
    mb();
#pragma GCC unroll 888
    for (u64 k = 0; k < n; ++k) {
        u64 c = (k * 13 + 9) & (n - 1);
        u64 dt = cache_read_access_time((u8 *) base + (stride * c));
        if (dt < CACHE_MISS_THRES)
            results[c]++;
    }
}

static __always_inline void flush_range(long start, long stride, unsigned n) {
    mb();
    for (u64 k = 0; k < n; ++k) {
        volatile void *p = (u8 *) start + k * stride;
        asm volatile("clflushopt (%0)\n" ::"r"(p));
        asm volatile("clflushopt (%0)\n" ::"r"(p));
    }
    rmb();
}

static __always_inline void rb_print() {
    u64 *rb_hist = _ptr(RB_HIST);
    for (int i = 0; i < RB_SLOTS; ++i) {
        printk("%04ld ", rb_hist[i]);
    }
    printk("\n");
}

static __always_inline __user_text void rb_print_user() {
    u64 *rb_hist = _ptr(RB_HIST);
    for (int i = 0; i < RB_SLOTS; ++i) {
        printf("%04ld ", rb_hist[i]);
    }
    printf("\n");
}

#define rb_reset()  memset(_ptr(RB_HIST), 0, RB_SLOTS * sizeof(u64))
#define rb_flush()  flush_range(_ul(RB_PTR), RB_STRIDE, RB_SLOTS);
#define rb_reload() reload_range(_ul(RB_PTR), RB_STRIDE, RB_SLOTS, RB_HIST);
