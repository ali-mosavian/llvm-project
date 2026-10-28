// RUN: %clang_cc1 -triple i386-unknown-linux-gnu -std=gnu89 -fdefault-calling-conv=regcall -emit-llvm -o - %s | FileCheck %s

// With regcall as the default, a prototyped function is regcall and an unprototyped one (K&R
// definition or declaration) keeps the C convention: a regcall function needs a prototype.

// CHECK-LABEL: define {{.*}}@__regcall3__proto(
int proto(int a) { return a; }

// CHECK-LABEL: define {{.*}}@knr(
int knr(a) int a; { return a; }

int undeclared();
int use(void) { return undeclared(1); }
