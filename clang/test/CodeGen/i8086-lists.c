// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -emit-llvm -o - %s | FileCheck %s
// A segment keyword opening a later declarator of a list is that declarator's alone.

// CHECK: @l1 = {{.*}}global ptr null
// CHECK: @l2 = {{.*}}global ptr addrspace(1) null
// CHECK: @l3 = {{.*}}global ptr addrspace(3) null
// CHECK: @l4 = {{.*}}global ptr null
char *l1, __far__ *l2, __huge__ *l3, *l4;
