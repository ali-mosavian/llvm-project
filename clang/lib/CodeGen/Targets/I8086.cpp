//===- I8086.cpp ----------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ABIInfoImpl.h"
#include "TargetInfo.h"

using namespace clang;
using namespace clang::CodeGen;

//===----------------------------------------------------------------------===//
// I8086 (llrm x86-m16) ABI: cdecl on the stack, 2-byte slots. Scalars are
// passed as they are (narrow integers extended to int); aggregates by value
// on the stack (byval) and returned through a hidden pointer.
//===----------------------------------------------------------------------===//

namespace {

class I8086ABIInfo : public DefaultABIInfo {
public:
  I8086ABIInfo(CodeGenTypes &CGT) : DefaultABIInfo(CGT) {}

  ABIArgInfo classifyReturnType(QualType RetTy) const {
    if (isAggregateTypeForABI(RetTy))
      return getNaturalAlignIndirect(RetTy, getDataLayout().getAllocaAddrSpace());
    return DefaultABIInfo::classifyReturnType(RetTy);
  }

  ABIArgInfo classifyArgumentType(QualType Ty) const {
    if (isAggregateTypeForABI(Ty))
      return ABIArgInfo::getIndirect(CharUnits::fromQuantity(2),
                                     getDataLayout().getAllocaAddrSpace(),
                                     /*ByVal=*/true, /*Realign=*/false);
    return DefaultABIInfo::classifyArgumentType(Ty);
  }

  void computeInfo(CGFunctionInfo &FI) const override {
    if (!getCXXABI().classifyReturnType(FI))
      FI.getReturnInfo() = classifyReturnType(FI.getReturnType());
    for (auto &I : FI.arguments())
      I.info = classifyArgumentType(I.type);
  }

  RValue EmitVAArg(CodeGenFunction &CGF, Address VAListAddr, QualType Ty,
                   AggValueSlot Slot) const override {
    return emitVoidPtrVAArg(CGF, VAListAddr, Ty, /*IsIndirect=*/false,
                            getContext().getTypeInfoInChars(Ty),
                            CharUnits::fromQuantity(2),
                            /*AllowHigherAlign=*/true, Slot);
  }
};

class I8086TargetCodeGenInfo : public TargetCodeGenInfo {
public:
  I8086TargetCodeGenInfo(CodeGenTypes &CGT)
      : TargetCodeGenInfo(std::make_unique<I8086ABIInfo>(CGT)) {}
};

} // namespace

std::unique_ptr<TargetCodeGenInfo>
CodeGen::createI8086TargetCodeGenInfo(CodeGenModule &CGM) {
  return std::make_unique<I8086TargetCodeGenInfo>(CGM.getTypes());
}
