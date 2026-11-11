// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -emit-llvm -o - %s | FileCheck %s

// A Borland interrupt function takes the registers it saved as plain
// parameters and is in the x86 interrupt convention; a call of it is too.
unsigned g;
void __attribute__((interrupt)) isr(unsigned bp, unsigned ax) { g = bp + ax; }
void __attribute__((interrupt)) plain(void) { g = 1; }
void use(void) { plain(); }

// CHECK: define{{.*}} x86_intrcc void @isr(i16 {{.*}}, i16 {{.*}})
// CHECK: define{{.*}} x86_intrcc void @plain()
// CHECK: call x86_intrcc addrspace(1) void @plain()
