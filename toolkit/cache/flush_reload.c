#include <compiler.h>
#include <lib.h>
#include <mm/pmm.h>
#include <pagetable.h>
#include <string.h>
#include <toolkit/cache/flush_reload.h>

void rb_init() {
    frame_t *hist_frame = get_free_frame();
    if (vmap_kern_4k(RB_HIST, hist_frame->mfn, L1_PROT_USER) != _ptr(RB_HIST))
        BUG();
    if (vmap_user_4k(RB_HIST, hist_frame->mfn, L1_PROT_USER) != _ptr(RB_HIST))
        BUG();

    frame_t *rb_frame = get_free_frames(9);
    if (vmap_kern_2m(_ptr(RB_PTR), rb_frame->mfn, L1_PROT_USER) != _ptr(RB_PTR))
        BUG();

    if (vmap_user_2m(_ptr(RB_PTR), rb_frame->mfn, L1_PROT_USER) != _ptr(RB_PTR))
        BUG();

    for (int i = 0; i < RB_SLOTS; ++i) {
        memset(_ptr(RB_PTR) + i * RB_STRIDE, '0' + i, RB_STRIDE);
    }
}
