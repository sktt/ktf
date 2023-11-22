#include <lib.h>
#include <toolkit/bp_tools.h>

#ifndef BHB_MAX
#define BHB_MAX 512
#endif

struct bhb_bb {
    unsigned long start_adr;
    unsigned long end_adr;
    int br_width;
};

struct call_args {
    unsigned long rdi;
    unsigned long rsi;
    unsigned long rdx;
    unsigned long rcx;
};

static inline unsigned long bb_len(struct bhb_bb *bb) {
    return 1 + bb->end_adr - bb->start_adr;
}

static inline unsigned long bb_pgs(struct bhb_bb *bb) {
    return 1 + (((bb->start_adr & 0xfff) + bb_len(bb) - 1) >> 12);
}

static inline void bhb_shuffle(struct bhb_bb *bhb_path, int n) {
    struct bhb_bb temp;

    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        temp = bhb_path[i];
        bhb_path[i] = bhb_path[j];
        bhb_path[j] = temp;
    }
}

void alloc_bhb_path(struct bhb_bb *bhb, int n, void *edge, unsigned edge_sz,
                    bool user_access);

void bhb_print(struct bhb_bb *bhb, int n);

void bhb_tramp_finish();

void bhb_tramp(struct bhb_bb *bhb, int n, void *vict_br_src, struct call_args *args);

void bhb_tramp_user(struct bhb_bb *bhb, int n, void *vict_br_src, struct call_args *args);
