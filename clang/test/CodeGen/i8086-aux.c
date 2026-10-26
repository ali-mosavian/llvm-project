// RUN: %clang_cc1 -triple i386-pc-none-code16 -fborland-extensions -emit-llvm -o - %s | FileCheck %s
// `#pragma aux NAME = "code" parm [..] value [..] modify [..]`: a call is the
// code as inline assembly, its registers the constraints.

#pragma aux add2 = "add al,bl" parm [al] [bl] value [al] modify exact [al]
char add2(char, char);
#pragma aux split parm [ax] [cx] value [dx ax] = "cwd" "idiv cx"
long split(int, int);
#pragma aux nop = 0x90
void nop(void);

// CHECK-LABEL: define {{.*}}i8 @a(
// CHECK: call addrspace(0) i16 asm sideeffect inteldialect "add al,bl\0A", "={ax},{ax},{bx},~{memory}"(i16 %{{.*}}, i16 %{{.*}})
char a(char x, char y) { return add2(x, y); }

// CHECK-LABEL: define {{.*}}i32 @s(
// CHECK: call addrspace(0) { i16, i16 } asm sideeffect inteldialect "cwd\0Aidiv cx\0A", "={dx},={ax},{ax},{cx},~{cx},~{memory}"(i16 %{{.*}}, i16 %{{.*}})
long s(int x, int y) { return split(x, y); }

// CHECK-LABEL: define {{.*}}void @n(
// CHECK: call addrspace(0) void asm sideeffect inteldialect ".byte 0x90\0A", "~{memory}"()
void n(void) { nop(); }
