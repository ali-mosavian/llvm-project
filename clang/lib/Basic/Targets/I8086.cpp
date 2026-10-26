//===--- I8086.cpp - Implement I8086 target feature support ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "I8086.h"
#include "clang/Basic/MacroBuilder.h"

using namespace clang;
using namespace clang::targets;

void I8086TargetInfo::getTargetDefines(const LangOptions &Opts,
                                       MacroBuilder &Builder) const {
  Builder.defineMacro("__i8086__");
  Builder.defineMacro("__I86__");
  Builder.defineMacro("_M_I86");
  // Borland's and Watcom's names for the memory model.
  const char *Model = Huge ? "__HUGE__"
                      : FarCode ? (FarData ? "__LARGE__" : "__MEDIUM__")
                                : (FarData ? "__COMPACT__" : "__SMALL__");
  Builder.defineMacro(Model);
}
