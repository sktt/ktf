// clang-format off
// clobbers rax
#define IRET(reg, pfx)                          \
    "movq   " #pfx "%ss, " #pfx "%rax     \n\t" \
    "pushq  " #pfx "%rax                  \n\t" \
    "lea    8(" #pfx "%rsp), " #pfx "%rax \n\t" \
    "pushq  " #pfx "%rax                  \n\t" \
    "pushf                                \n\t" \
    "movq   " #pfx "%cs, " #pfx "%rax     \n\t" \
    "pushq  " #pfx "%rax                  \n\t" \
    "pushq  " #pfx "%" #reg "             \n\t" \
    "iretq                                \n\t"
// clang-format on

#define BHB_RAND                                                                         \
    asm volatile(".align 0x10\n\t"                                                       \
                 "rdrand %%eax\n"                                                        \
                 "rdrand %%ecx\n"                                                        \
                 "test $0x0000001, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000002, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000004, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000008, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000010, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000020, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000040, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000080, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000100, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000200, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000400, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000800, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0001000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0002000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0004000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0008000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0010000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0020000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0040000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0080000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0100000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0200000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0400000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0800000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x1000000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x2000000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x4000000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x8000000, %%eax\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000001, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000002, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000004, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000008, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000010, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000020, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000040, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000080, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000100, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000200, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000400, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0000800, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0001000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0002000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0004000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0008000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0010000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0020000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0040000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0080000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0100000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0200000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0400000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x0800000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x1000000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x2000000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x4000000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t"         \
                 "test $0x8000000, %%ecx\n\tjz 1f\n\t .skip 0x8, 0x90\n\t1:\n\t" ::      \
                     : "cc", "memory", "rax", "rcx");

// Alder and later (11x6) + ..
#define BHB_CLEAR_POST_ALDER(pfx)                                                        \
    "      mov $12, " #pfx "%ecx  \n\t"                                                  \
    "      call 801f              \n\t"                                                  \
    "      jmp 805f               \n\t"                                                  \
    "      .align 64              \n\t"                                                  \
    "801:  call 802f              \n\t"                                                  \
    "      ret                    \n\t"                                                  \
    "      .align 64              \n\t"                                                  \
    "802:  movl $7, " #pfx "%eax  \n\t"                                                  \
    "803:  jmp 804f               \n\t"                                                  \
    "      nop                    \n\t"                                                  \
    "804:  sub $1, " #pfx "%eax   \n\t"                                                  \
    "      jnz 803b               \n\t"                                                  \
    "      sub $1, " #pfx "%ecx   \n\t"                                                  \
    "      jnz 801b               \n\t"                                                  \
    "      ret                    \n\t"                                                  \
    "805:  lfence                 \n\t"
// eax, ecx

// Before Alder (4x4)
#define BHB_CLEAR_PRE_ALDER(pfx)                                                         \
    "      mov $5, " #pfx "%ecx         \n\t"                                            \
    "      call 801f                    \n\t"                                            \
    "      jmp 805f                     \n\t"                                            \
    "      .align 64                    \n\t"                                            \
    "801:  call 802f                    \n\t"                                            \
    "      ret                          \n\t"                                            \
    "      .align 64                    \n\t"                                            \
    "802:  movl $5, " #pfx "%eax        \n\t"                                            \
    "803:  jmp 804f                     \n\t"                                            \
    "      nop                          \n\t"                                            \
    "804:  sub $1, " #pfx "%eax         \n\t"                                            \
    "      jnz 803b                     \n\t"                                            \
    "      sub $1, " #pfx "%ecx         \n\t"                                            \
    "      jnz 801b                     \n\t"                                            \
    "      ret                          \n\t"                                            \
    "805:  lfence                       \n\t" // eax, ecx
