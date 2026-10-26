// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -emit-llvm -o - %s | FileCheck %s
// RUN: not %clang_cc1 -triple x86_64-linux -fborland-extensions -fsyntax-only %s 2>&1 | FileCheck %s --check-prefix=OTHER

// CHECK: @p = {{.*}}global ptr addrspace(1)
char __far__ *p;
// CHECK: @q = {{.*}}global ptr addrspace(1)
char *__far__ q;
// CHECK: @h = {{.*}}global ptr addrspace(3)
int __huge__ *h;
// CHECK: @n = {{.*}}global i16
int __near__ n;
// CHECK: define {{.*}}void @f() addrspace(1)
void __far__ f(void) {}

// __emit__ puts constant bytes into the code stream; Borland's dos.h declares it.
void __emit__(unsigned char byte, ...);
// CHECK: define {{.*}}void @e()
// CHECK: call addrspace(0) void asm sideeffect ".byte 144,205,33"
void e(void) { __emit__(0x90, 0xCD, 0x21); }

// OTHER: error: '__far__' is only available when targeting i8086

// A huge object lives in a far segment; only its pointers are huge, and two of
// them are subtracted by llrm's intrinsic, not by their packed bits.
// CHECK: @big = {{.*}}addrspace(1) global [100 x i8]
char __huge__ big[100];
// CHECK: define {{.*}}i32 @hd(
// CHECK: call addrspace(0) i32 @llrm.ia16.ptrdiff.i32.p3(ptr addrspace(3)
long hd(char __huge__ *p, char __huge__ *q) { return q - p; }
