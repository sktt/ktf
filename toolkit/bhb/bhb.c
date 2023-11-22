#include "../../tests/log.h"
#include <compiler.h>
#include <console.h>
#include <ktf.h>
#include <lib.h>
#include <page.h>
#include <pagetable.h>
#include <test.h>
#include <toolkit/bhb/bhb.h>
#include <usermode.h>

#define PG_MASK  _ul(~0xfff)
#define BHB_BITS 31ul // how many lower bits the bhb uses?
#define BB_MASK  ((1ul << BHB_BITS) - 1)

bool is_mapped(void *va) {
    pte_t *pte;
    bool mapped;
    map_pagetables(&cr3, NULL);
    pte = get_pte(va);
    mapped = pte->mfn != 0 && !mfn_invalid(pte->mfn);
    unmap_pagetables(&cr3, NULL);
    return mapped;
}

bool bb_overlap(struct bhb_bb *our, struct bhb_bb *their) {
    if (their->start_adr < our->start_adr) {
        // their start before our, hence
        // overlap if their end after our start
        return their->end_adr > our->start_adr;
    }

    // their starts before our ends, but they start after our starts.
    return their->start_adr < our->end_adr;
}

// does bb at bb_i collide with any previous bb?
bool bb_avail(struct bhb_bb *bhb_path, int bb_i) {
    struct bhb_bb *our = &bhb_path[bb_i];
    for (int i = 0; i < bb_i; ++i) {
        struct bhb_bb *their = &bhb_path[i];
        if (bb_overlap(our, their)) {
            return false;
        }
    }
    return true;
}

/* 41 58                   pop    %r8            */
/* 8c d0                   mov    %ss,%eax       */
/* 50                      push   %rax           */
/* 48 8d 44 24 08          lea    0x8(%rsp),%rax */
/* 50                      push   %rax           */
/* 9c                      pushf                 */
/* 8c c8                   mov    %cs,%eax       */
/* 50                      push   %rax           */
/* 41 50                   push   %r8            */
/* 48 cf                   iretq                 */
#define IRET_STUB_LEN 19 // can be shorter for non-prefixed regs

void alloc_bhb_path(struct bhb_bb *bhb_path, int n, void *edge, unsigned edge_sz,
                    bool user_access) {
    for (int i = 0; i < n; ++i) {
        struct bhb_bb *bb = &bhb_path[i];
        if (bb->start_adr == 0) {
            KINFO("Missing BB at %d\n", i);
            break;
        }

        // keep just the bhb-index bits
        /* bb->start_adr &= BB_MASK; */
        /* bb->end_adr &= BB_MASK; */
        if (bb->end_adr < bb->start_adr) {
            // The BB shouldn't end after the start
            bb->end_adr += 1ul << BHB_BITS;
            KINFO("End before start. can't we just IRET?\n");
        }

        if (bb_len(bb) < edge_sz) {
            KINFO("bhb[%d]: The bb is too small, or the edge is too big.", i);
            BUG();
        }

        int skip_block = 0;
        while (!bb_avail(bhb_path, i)) {
            printk("skip unavil\n");
            skip_block += 1;
            bb->start_adr += skip_block * (1ul << BHB_BITS);
            bb->end_adr += skip_block * (1ul << BHB_BITS);
        }

        /* if (bb->start_adr >> 48 != BB_AREA >> 48) { */
        /*     KINFO("overflow\n");                    */
        /*     BUG();                                  */
        /* }                                           */
        // we could map it actually.
        long npgs = bb_pgs(bb);
        unsigned long map_va = bb->start_adr & ~0xffful;

        for (int p = 0; p < npgs; ++p) {
            if (npgs > 10) {
                KINFO("npgs=%d\n", npgs);
                BUG();
            }
            void *ptr = _ptr(map_va + p * 4096);
            if (!is_mapped(ptr)) {
                // assume if not mapped, that it may need to be mapped for user too.
                frame_t *f = get_free_frame();
                if (vmap_kern_4k(ptr, f->mfn, L1_PROT_USER) != ptr) {
                    KINFO("failed: %lx\n", ptr);
                    BUG();
                }
                if (vmap_user_4k(ptr, f->mfn, L1_PROT_USER) != ptr) {
                    KINFO("failed: %lx\n", ptr);
                    BUG();
                }
            }
        }
        memset(_ptr(bb->start_adr), 0x90, bb_len(bb) - edge_sz);
        memcpy(_ptr(bb->end_adr - edge_sz + 1), edge, edge_sz);
    }
}

// to maximize fun, we switch stack when applying the bhb.
__user_data __aligned(0x1000) static long bhb_stack[0x1000]; // 24kb alot..
__user_text void bhb_tramp_user(struct bhb_bb *bhb, int n, void *vict_br_src,
                                struct call_args *args) {
    if (n + 2 > BHB_MAX) {
        // We will append the targets array with victim branch and the final branch to us
        // return here where restore the stack and whatever state needs to be restored.
        USER_BUG();
    }

    // let's leave some space on top of the stack so we can push the IRET frame
    static long *stack = &bhb_stack[0x40];
    for (int i = 0; i < n; ++i) {
        stack[i] = bhb[i].start_adr;
    }
    stack[n] = (long) vict_br_src;
    stack[n + 1] = (long) bhb_tramp_finish;

    // clang-format off
    asm(
        "mov %%rsp, %%r15\n" // hahaha no.
        "lea 8(%0), %%rsp\n" // stack following bbs.
        "mov (%0), %%r8\n"   // the first bb via IRET
        IRET(r8, %)
        "bhb_tramp_finish:\n"
        "mov %%r15, %%rsp\n"
        ::"r"(stack),
        "D"(args->rdi), "S"(args->rsi), "d"(args->rdx), "c"(args->rcx)
        :"r8", "r15", "rax");
     //clang-format on
}

void bhb_tramp(struct bhb_bb *bhb, int n, void *vict_br_src, struct call_args *args) {
    unsigned long t = (unsigned long) bhb_tramp_user;
    asm("jmp *%0" :: "r"(t));
}

void bhb_print(struct bhb_bb *bhb, int n) {
    for (int i = 0; i < n; ++i) {
        printk("[%00d] 0x%lx--0x%lx\n", i, bhb[i].start_adr, bhb[i].end_adr);
    }
}
