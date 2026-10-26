// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -mcmodel=small -DWANT_SMALL -DCODE=2 -DDATA=2 -DDIFF=2 -emit-llvm -o - %s | FileCheck %s --check-prefix=SMALL
// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -mcmodel=medium -DWANT_MEDIUM -DCODE=4 -DDATA=2 -DDIFF=2 -emit-llvm -o - %s | FileCheck %s --check-prefix=MEDIUM
// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -mcmodel=compact -DWANT_COMPACT -DCODE=2 -DDATA=4 -DDIFF=4 -emit-llvm -o - %s | FileCheck %s --check-prefix=COMPACT
// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -mcmodel=large -DWANT_LARGE -DCODE=4 -DDATA=4 -DDIFF=4 -emit-llvm -o - %s | FileCheck %s --check-prefix=LARGE
// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -mcmodel=huge -DWANT_HUGE -DCODE=4 -DDATA=4 -DDIFF=4 -emit-llvm -o - %s | FileCheck %s --check-prefix=LARGE
// Memory models: the model sets the default distance of code and of data; __near__,
// __far__ and __huge__ on a data pointer override it. A function pointer follows the
// code distance.

// SMALL: target datalayout = "e-p:16:16-p1:
// MEDIUM: target datalayout = "e-P1-p:16:16-p1:
// COMPACT: target datalayout = "e-p:16:16-p1:
// LARGE: target datalayout = "e-P1-p:16:16-p1:

char *np;
char __far__ *fp;
char __huge__ *hp;
char __near__ *nr;
char **pp;
int (*nf)(void);
struct S { char *p; int (*f)(void); int k; };

// SMALL: @np = {{.*}}global ptr null
// MEDIUM: @np = {{.*}}global ptr null
// COMPACT: @np = {{.*}}global ptr addrspace(1) null
// LARGE: @np = {{.*}}global ptr addrspace(1) null
// SMALL: @nr = {{.*}}global ptr null
// COMPACT: @nr = {{.*}}global ptr null
// COMPACT: @pp = {{.*}}global ptr addrspace(1) null
// MEDIUM: @nf = {{.*}}global ptr addrspace(1) null
// COMPACT: @nf = {{.*}}global ptr null

_Static_assert(sizeof(np) == DATA, "default data pointer");
_Static_assert(sizeof(fp) == 4 && sizeof(hp) == 4, "far and huge pointers");
_Static_assert(sizeof(nr) == 2, "near data pointer");
_Static_assert(sizeof(nf) == CODE, "function pointer follows the code distance");
_Static_assert(sizeof(np - np) == DIFF, "ptrdiff_t");
_Static_assert(sizeof(struct S) == DATA + CODE + 2, "record of a pointer, a function pointer and an int");

// A declaration list shares one type specifier: a pointer in it qualifies no neighbour.
char buf[4], *p, **ppp, c;
_Static_assert(sizeof(buf) == 4 && sizeof(c) == 1, "list members");

// Arithmetic and loads follow the pointer's own distance.
// COMPACT: define {{.*}}i16 @near_load(ptr noundef
// COMPACT: load i8, ptr %
int near_load(char __near__ *q) { return *q; }
// COMPACT: define {{.*}}i16 @near_var(ptr noundef
// COMPACT: load i8, ptr %
int near_var(char *__near__ q) { return *q; }
// LARGE: define {{.*}}i16 @far_load(ptr addrspace(1)
// LARGE: load i8, ptr addrspace(1)
int far_load(char *q) { return *q; }

// The model's name as a macro, as Borland and Watcom state it.
#if defined(__SMALL__) + defined(__MEDIUM__) + defined(__COMPACT__) + defined(__LARGE__) + defined(__HUGE__) != 1
#error "exactly one model macro"
#endif
#if defined(WANT_SMALL) && !defined(__SMALL__) || defined(WANT_MEDIUM) && !defined(__MEDIUM__) ||                       \
    defined(WANT_COMPACT) && !defined(__COMPACT__) || defined(WANT_LARGE) && !defined(__LARGE__) ||                      \
    defined(WANT_HUGE) && !defined(__HUGE__)
#error "the model macro is not the model's"
#endif
