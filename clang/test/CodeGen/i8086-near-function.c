// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -emit-llvm -o - %s | FileCheck %s

// A `near` function is in code address space 0 and is called without a segment;
// a `far` one and an unmarked one stay in the program address space (far).
int __near__ nf(int a) { return a + 1; }
int __far__ ff(int a) { return a + 2; }
int uf(int a) { return a + 3; }
int __near__ declared(int);
char *__near__ pf(void) { return 0; }
int use(void) { return nf(1) + ff(2) + uf(3) + declared(4) + (pf() != 0); }
int __near__ declared(int a) { return a; }

// CHECK: define{{.*}} i16 @nf(i16 {{.*}}) addrspace(0)
// CHECK: define{{.*}} i16 @ff(i16 {{.*}}) addrspace(1)
// CHECK: define{{.*}} i16 @uf(i16 {{.*}}) addrspace(1)
// CHECK: define{{.*}} ptr @pf() addrspace(0)
// CHECK: call addrspace(0) i16 @nf(
// CHECK: call addrspace(1) i16 @ff(
// CHECK: call addrspace(1) i16 @uf(
// CHECK: call addrspace(0) i16 @declared(
// CHECK: call addrspace(0) ptr @pf(
// CHECK: define{{.*}} i16 @declared(i16 {{.*}}) addrspace(0)
