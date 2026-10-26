//===--- I8086.h - Declare I8086 target feature support ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Real-mode 8086 family target (llrm x86-m16): 16-bit int and near pointers,
// 32-bit long, address_space(1) far, (2) segment, (3) huge, (4) fixed.
// Keyed on the i386-unknown-unknown-code16 triple.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_I8086_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_I8086_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"

namespace clang {
namespace targets {

class LLVM_LIBRARY_VISIBILITY I8086TargetInfo : public TargetInfo {
public:
  I8086TargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple) {
    TLSSupported = false;
    IntWidth = IntAlign = 16;
    LongWidth = 32;
    LongLongWidth = 64;
    LongAlign = LongLongAlign = 16;
    FloatWidth = 32;
    FloatAlign = 16;
    DoubleWidth = 64;
    DoubleAlign = 16;
    LongDoubleWidth = 80;
    LongDoubleAlign = 16;
    LongDoubleFormat = &llvm::APFloat::x87DoubleExtended();
    PointerWidth = PointerAlign = 16;
    SuitableAlign = 16;
    SizeType = UnsignedInt;
    IntMaxType = SignedLongLong;
    IntPtrType = SignedInt;
    PtrDiffType = SignedInt;
    SigAtomicType = SignedInt;
    MaxAtomicPromoteWidth = MaxAtomicInlineWidth = 0;
    resetDataLayout("e-P1-p:16:16-p1:32:16:16:16-p2:16:16-p3:32:16:16:32-"
                    "p4:32:16:16:16-i32:16-i64:16-n8:16:32");
  }

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  llvm::SmallVector<Builtin::InfosShard> getTargetBuiltins() const override {
    return {};
  }

  // Far and huge pointers carry a segment: 32 bits. The segment alone is 16.
  uint64_t getPointerWidthV(LangAS AddrSpace) const override {
    switch (toTargetAddressSpace(AddrSpace)) {
    case 1: // far
    case 3: // huge
    case 4: // fixed
      return 32;
    default:
      return 16;
    }
  }
  // A near pointer converts to a far one, and either to a huge one, as in C
  // for real mode; the reverse needs a cast.
  bool isAddressSpaceSupersetOf(LangAS A, LangAS B) const override {
    auto Number = [](LangAS AS) {
      return AS == LangAS::Default ? 0u : toTargetAddressSpace(AS);
    };
    unsigned To = Number(A), From = Number(B);
    return To == From || ((To == 1 || To == 3) && From == 0) ||
           (To == 3 && From == 1);
  }
  uint64_t getPointerAlignV(LangAS AddrSpace) const override { return 16; }

  // The driver names an x86 CPU for this triple; llrm-llc takes the real one.
  bool setCPU(StringRef Name) override { return true; }

  CallingConvCheckResult checkCallingConvention(CallingConv CC) const override {
    return CC == CC_C || CC == CC_X86Pascal ? CCCR_OK : CCCR_Warning;
  }

  bool allowsLargerPreferedTypeAlignment() const override { return false; }

  bool hasFeature(StringRef Feature) const override {
    return Feature == "i8086";
  }

  ArrayRef<const char *> getGCCRegNames() const override { return {}; }
  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override {
    return {};
  }
  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &Info) const override {
    return false;
  }
  std::string_view getClobbers() const override { return ""; }

  BuiltinVaListKind getBuiltinVaListKind() const override {
    return TargetInfo::CharPtrBuiltinVaList;
  }
};

} // namespace targets
} // namespace clang
#endif // LLVM_CLANG_LIB_BASIC_TARGETS_I8086_H
