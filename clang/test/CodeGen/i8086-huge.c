// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -emit-llvm -o - %s | FileCheck %s
// A huge object lives in a far segment; only its pointers are huge, and two of
// them are subtracted by llrm's intrinsic, not by their packed bits.

// CHECK: @big = {{.*}}addrspace(1) global [100 x i8]
char __huge__ big[100];
// CHECK: define {{.*}}i32 @hd(
// CHECK: call addrspace(0) i32 @llrm.ia16.ptrdiff.i32.p3(ptr addrspace(3)
long hd(char __huge__ *p, char __huge__ *q) { return q - p; }
