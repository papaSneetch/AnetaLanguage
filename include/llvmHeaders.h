#ifndef llvmHeaders
#define llvmHeaders

#include <llvm/IR/Value.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Bitcode/BitcodeWriter.h"

#include "llvm/Support/TargetSelect.h"

#include "llvm/Support/TargetRegistry.h"

/* #include "llvm/Config/llvm-config.h"
#include "llvm/Config/Targets.def"
#include "llvm/Config/AsmPrinters.def"
#include "llvm/Config/AsmParsers.def"
#include "llvm/Config/Disassemblers.def" */

#include "llvm/Support/Host.h"

#include "llvm/CodeGen/CommandFlags.h"

#endif
