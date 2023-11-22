#include <compiler.h>
// may be assert its size. but its good to have this value at compile time.
#define CODE_STUB_DESC_SZ 0x28

struct code_stub {
    union {
        unsigned char *ptr;
        unsigned long ptr_u64;
        void (*fptr)();
    };
    void *map_base;
    unsigned long map_sz;
    unsigned long code_sz;
    // map_base           map_base+map_sz
    // |----------------------|
    //        <-------->
    //        ptr     ptr+code_sz
    void *tmpl; // often we build allocations from a template stub of code
};
typedef struct code_stub code_stub_t;

// clang-format off
#define mk_stub(name, str)                      \
    extern char name##__stub[];                 \
    extern char e_##name##__stub[];             \
    asm(".pushsection .text.user \n"            \
        ".align 0x1000           \n"            \
        #name "__stub:            \n"           \
        str                                     \
        "\ne_"#name"__stub: nop")
// clang-format on

#define stub_sz(name) (unsigned long) (e_##name##__stub - name##__stub)

#define initc_stub(VA, stub) memcpy(_ptr(VA), stub##__stub, stub_sz(stub))

void _init_stub(code_stub_t *code_stub, void *ptr, void *stub, long sz);
__user_text void free_stub_user(code_stub_t *code_stub);
void free_stub(code_stub_t *code_stub);
__user_text void _init_stub_user(code_stub_t *code_stub, void *ptr, void *stub, long sz);

#define init_stub(code_stub, ptr, stub)                                                  \
    _init_stub(code_stub, ptr, stub##__stub, stub_sz(stub))
#define init_stub_user(code_stub, ptr, stub)                                             \
    _init_stub_user(code_stub, ptr, stub##__stub, stub_sz(stub))
