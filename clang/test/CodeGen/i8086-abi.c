// RUN: %clang_cc1 -triple i386-unknown-unknown-code16 -fpack-struct=1 -emit-llvm -o - %s | FileCheck %s
// Sizes and address spaces of the i8086 (llrm x86-m16) target.
#define far __attribute__((address_space(1)))
#define huge __attribute__((address_space(3)))
_Static_assert(sizeof(int) == 2 && sizeof(long) == 4, "integers");
_Static_assert(sizeof(int *) == 2 && sizeof(int far *) == 4 && sizeof(int huge *) == 4, "pointers");
_Static_assert(sizeof(void (*)(void)) == 4, "function pointers are far");
typedef struct { void (*f)(int); int x; char c; } Record;
_Static_assert(sizeof(Record) == 7, "byte-packed");

// A near pointer converts to a far one, and a far one to a near one, as Borland does.
// CHECK-LABEL: define {{.*}}@widen(
// CHECK: addrspacecast ptr %{{.*}} to ptr addrspace(1)
int far *widen(int *p) { return p; }
// CHECK-LABEL: define {{.*}}@narrow(
// CHECK: addrspacecast ptr addrspace(1) %{{.*}} to ptr
int *narrow(int far *p) { return p; }

// `far` beside a pointer declarator makes the pointer far, and a void result is not far.
// CHECK-LABEL: define {{.*}}@takes(
void far takes(char *far *pp, int far *q) {}

// Pascal is llrm's BASIC convention, as cc232.
// CHECK: declare {{.*}}cc232 {{.*}}@pascal_fn
int __attribute__((pascal)) pascal_fn(int);
int call_pascal(void) { return pascal_fn(1); }
