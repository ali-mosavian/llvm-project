// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -emit-llvm -o - %s | FileCheck %s
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

// OTHER: error: '__far__' is only available when targeting i8086
