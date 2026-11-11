// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -std=gnu89 -emit-llvm -o - %s | FileCheck %s

// A Borland interrupt function takes the registers it saved as plain
// parameters and is in the x86 interrupt convention; a call of it is too.
unsigned g;
void __attribute__((interrupt)) isr(unsigned bp, unsigned ax) { g = bp + ax; }
void __attribute__((interrupt)) plain(void) { g = 1; }
void use(void) { plain(); }
void __attribute__((interrupt)) setax(unsigned ax) { ax = 0x7777; }

// CHECK: define{{.*}} x86_intrcc void @isr(ptr noundef byval(i16) align 2 %bp, ptr noundef byval(i16) align 2 %ax)
// CHECK: define{{.*}} x86_intrcc void @plain()
// CHECK: call x86_intrcc addrspace(1) void @plain()
// An assignment to a register parameter is a store through the in-place slot:
// CHECK: define{{.*}} x86_intrcc void @setax(ptr noundef byval(i16) align 2 %ax)
// CHECK: store i16 30583, ptr %ax
