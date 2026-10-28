// RUN: %clang_cc1 -triple i386-unknown-linux-gnu -emit-llvm -o - %s | FileCheck %s

// A small struct passed to a regcall function stays one argument on the stack
// (byval): expanded into scalars, a backend that gives scalars registers
// could not tell them from the struct (Open Watcom's rule is the stack).
// Other conventions still expand it.

struct S { int a, b, c; };

// CHECK-LABEL: define {{.*}}@__regcall3__regcall_by_value(
// CHECK-SAME: ptr {{.*}}byval(%struct.S)
int __attribute__((regcall)) regcall_by_value(struct S s, int x) { return s.a + s.b + s.c + x; }

// CHECK-LABEL: define {{.*}}@cdecl_by_value(
// CHECK-SAME: i32 {{.*}}, i32 {{.*}}, i32 {{.*}}, i32
int __attribute__((cdecl)) cdecl_by_value(struct S s, int x) { return s.a + s.b + s.c + x; }
