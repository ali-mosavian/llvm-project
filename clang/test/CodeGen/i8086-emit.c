// RUN: %clang_cc1 -triple i386-pc-none-code16 -ffreestanding -fborland-extensions -std=gnu89 -emit-llvm -o - %s | FileCheck %s
// __emit__ puts constant bytes in the code stream, in a freestanding program and
// with no declaration in scope (QCport's sys_time.c calls it that way); it was a
// call to an undefined `___emit__` there.

// CHECK: define {{.*}}void @rdtsc()
// CHECK: call addrspace(0) void asm sideeffect ".byte 15,49"
// CHECK-NOT: @__emit__
void rdtsc(void) { __emit__(0x0f, 0x31); }
