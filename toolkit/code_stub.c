#include <compiler.h>
#include <console.h>
#include <mm/pmm.h>
#include <pagetable.h>
#include <string.h>
#include <toolkit/code_stub.h>
#include <usermode.h>

void _init_stub(code_stub_t *code_stub, void *ptr, void *stub, long sz) {
    int pgs = 1 + (_ul(ptr + sz) >> 12) - (_ul(ptr) >> 12);
    code_stub->ptr = ptr;
    code_stub->code_sz = sz;
    code_stub->map_sz = pgs * 0x1000;
    code_stub->tmpl = stub;
    code_stub->map_base = _ptr(_ul(ptr) & ~0xffful);

    for (int i = 0; i < pgs; ++i) {
        void *pg = code_stub->map_base + i * 0x1000;
        if (vmap_kern_4k(pg, get_free_frame()->mfn, L1_PROT) != pg)
            BUG();
    }

    memcpy(ptr, stub, sz);
}

__user_text void _init_stub_user(code_stub_t *code_stub, void *ptr, void *stub, long sz) {
    int pgs = 1 + (_ul(ptr + sz) >> 12) - (_ul(ptr) >> 12);
    code_stub->ptr = ptr;
    code_stub->code_sz = sz;
    code_stub->map_sz = pgs * 0x1000;
    code_stub->tmpl = stub;
    code_stub->map_base = _ptr(_ul(ptr) & ~0xffful);

    for (int i = 0; i < pgs; ++i) {
        void *pg = code_stub->map_base + i * 0x1000;
        if (mmap(pg, PAGE_ORDER_4K) != pg)
            USER_BUG();
    }

    memcpy(ptr, stub, sz);
}

void free_stub(code_stub_t *code_stub) {
    void *ptr = code_stub->ptr;
    unsigned long sz = code_stub->code_sz;
    int pgs = 1 + (_ul(ptr + sz) >> 12) - (_ul(ptr) >> 12);
    for (int i = 0; i < pgs; ++i) {
        void *pg = _ptr((_ul(ptr) & ~_ul(0xfff))) + i * 0x1000;
        vunmap_kern(pg, NULL, PAGE_ORDER_4K);
    }
    memset(code_stub, 0, sizeof(*code_stub));
}

__user_text void free_stub_user(code_stub_t *code_stub) {
    void *ptr = code_stub->ptr;
    unsigned long sz = code_stub->code_sz;
    int pgs = 1 + (_ul(ptr + sz) >> 12) - (_ul(ptr) >> 12);
    for (int i = 0; i < pgs; ++i) {
        void *pg = _ptr((_ul(ptr) & ~_ul(0xfff))) + i * 0x1000;
        munmap(pg);
    }
    memset(code_stub, 0, sizeof(*code_stub));
}
