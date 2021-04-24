#define _POSIX_SOURCE
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include "codeGenContext.h"
#include "annetaBuilder.h"
#include "annetaTypes.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/Host.h"
#include "llvm/CodeGen/CommandFlags.h"

void genContext::initContext() {
IRContext = std::make_unique<llvm::LLVMContext>(); 
CurModule = std::make_unique<llvm::Module>("Module",*IRContext); 
Builder = std::make_unique<llvm::IRBuilder<>>(*IRContext); 
types = std::make_unique<typeTable>();
initPrimativeTypes();
initLibaryFunctions();
}

void genContext::create_printf()
{
llvm::FunctionType *ft = llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*IRContext),llvm::Type::getInt8Ty(*IRContext)->getPointerTo(),true);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,"printf",CurModule.get());
pushFunction("printf",func,std::vector<const AstType*>{types->getExpandTypes("char",1)},&intType,true);
std::cerr << "Generated: " << "printf." << std::endl;
}

void genContext::create_scanf()
{
llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(*IRContext),llvm::Type::getInt8Ty(*IRContext)->getPointerTo(),true);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,"scanf",CurModule.get());
pushFunction("scanf",func,std::vector<const AstType*>{types->getExpandTypes("char",1)},&intType,true);
std::cerr << "Generated: " << "scanf." << std::endl;
}

void genContext::create_atoi()
{
llvm::FunctionType *ft = llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*IRContext),llvm::Type::getInt8Ty(*IRContext)->getPointerTo(),false);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,"atoi",CurModule.get());
pushFunction("atoi",func,std::vector<const AstType*>{types->getExpandTypes("char",1)},&intType);
std::cerr << "Generated: " << "atoi." << std::endl;
}

void genContext::create_atof()
{
llvm::FunctionType *ft = llvm::FunctionType::get(llvm::IntegerType::getDoubleTy(*IRContext),llvm::Type::getInt8Ty(*IRContext)->getPointerTo(),false);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,"atof",CurModule.get());
pushFunction("atoi",func,std::vector<const AstType*>{types->getExpandTypes("char",1)},&floatType);
std::cerr << "Generated: " << "atof." << std::endl;
}

void genContext::create_sprintf()
{
llvm::FunctionType *ft = llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*IRContext),{llvm::Type::getInt8Ty(*IRContext)->getPointerTo(),llvm::Type::getInt8Ty(*IRContext)->getPointerTo()},true);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,"sprintf",CurModule.get());
pushFunction("sprintf",func,std::vector<const AstType*>{types->getExpandTypes("char",1)},&intType,true);
std::cerr << "Generated: " << "sprintf." << std::endl;
}

void genContext::initLibaryFunctions() {
create_printf();
create_scanf();
create_atoi();
create_atof();
create_sprintf();
}

void genContext::initPrimativeTypes()
{
types->createTypeElement(&intType);
types->createTypeElement(&floatType);
types->createTypeElement(&boolType);
types->createTypeElement(&charType);
types->getExpandTypes("char",1);
}

void genContext::createStart()
{
llvm::Type* voidTy = llvm::Type::getVoidTy(*IRContext);
std::vector<llvm::Type*> argTypes;
llvm::FunctionType *ft = llvm::FunctionType::get(voidTy,argTypes,false);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,"_start",CurModule.get());
llvm::BasicBlock *BB = llvm::BasicBlock::Create(*IRContext,"_start",func);
Builder->SetInsertPoint(BB);

functionInformation mainFunctionValue = functionLookUp("main");
std::vector <llvm::Value*> argsV;
Builder->CreateCall(mainFunctionValue.function,argsV,"functionCall",nullptr);

Builder->CreateRet(llvm::UndefValue::get(llvm::Type::getVoidTy(*IRContext)));
}

void genContext::pushBlock(AstBlockPtr& block)
{
blockList.push_back(std::move(block));
}

void genContext::pushBlock(AstBlock* blockPtr)
{
blockList.emplace_back(blockPtr);
}

void genContext::popBlock()
{
blockList.back().reset();
blockList.pop_back();
}

void genContext::pushVariable(std::string name, llvm::AllocaInst* varPointer,const AstType* type)
{
blockList.back()->variableMap.insert(std::make_pair(name,variableInformation{varPointer,type}));
}

void genContext::pushArrayVariable(std::string name, llvm::AllocaInst* varPointer,const AstType* type, int size)
{
blockList.back()->arrayMap.insert(std::make_pair(name,arrayInformation{varPointer,type,size}));
}

void genContext::pushFunction(std::string name,llvm::Function* function,std::vector<const AstType*> types,const AstType* returnType,bool varArg)
{
functionMap.insert(std::make_pair(name,functionInformation{function,types,returnType,varArg}));
}

void genContext::pushGlobalVariable(std::string name,llvm::GlobalVariable* var,const AstType* type)
{
globalVariableMap.insert(std::make_pair(name,globalVariableInformation{var,type}));
}

functionInformation genContext::functionLookUp(std::string name)
{
std::map<std::string,functionInformation>::iterator mapIt = functionMap.find(name);
if (mapIt != functionMap.end())
{
return (mapIt->second);
}
std::cerr<< "Couldn't find function: ";
std::cerr<< name << std::endl;
exit(1);
}

globalVariableInformation genContext::globalVariableLookUp (std::string name)
{
std::map<std::string,globalVariableInformation>::iterator mapIt = globalVariableMap.find(name);
if (mapIt != globalVariableMap.end())
{
return (mapIt->second);
}
return {nullptr,nullptr};
}

void genContext::pushAstNode(AstNode* node)
{
codeObjects.push(AstNodePtr(node));
}

void genContext::pushAstNode(AstNodePtr& node)
{
codeObjects.push(std::move(node));
}

AstNode* genContext::popAndGetAstNode()
{
return codeObjects.front().release();
}

variableInformation genContext::varLookUp (std::string name)
{
for (std::vector<AstBlockPtr>::reverse_iterator it = blockList.rbegin(); it != blockList.rend(); ++it)
{
std::map<std::string,variableInformation>::iterator mapIt = (*it) -> variableMap.find(name);
if (mapIt != ((*it) -> variableMap.end()))
{
return mapIt->second;
}
}
return {nullptr,nullptr};
}


arrayInformation genContext::arrayLookUp (std::string name)
{
for (std::vector<AstBlockPtr>::reverse_iterator it = blockList.rbegin(); it != blockList.rend(); ++it)
{
std::map<std::string,arrayInformation>::iterator mapIt = (*it) -> arrayMap.find(name);
if (mapIt != ((*it) -> arrayMap.end()))
{
return mapIt->second;
}
}
return {nullptr,nullptr,0};
}

void genContext::printBitCode(std::string outputFileName)
{
std::error_code errorCode;
llvm::raw_fd_ostream OStream(outputFileName,errorCode);
llvm::WriteBitcodeToFile(*CurModule,OStream);
}

void genContext::printObjectCode(std::string outputFileName)
{
char tmpFileName [L_tmpnam+3];
tmpnam(tmpFileName);
strncat(tmpFileName,".ll",4);
std::error_code errorCode;
llvm::raw_fd_ostream OStream(tmpFileName,errorCode,llvm::sys::fs::CreationDisposition::CD_CreateNew);
CurModule->print(OStream,nullptr);

/*char tmpFileName[] = "/tmp/llvmTemp.XXXXXX";
int tmpFileDescriptor;
if ((tmpFileDescriptor = mkstemp(tmpFileName))==-1) {
std::cerr << "Cannot create tmp file:" << tmpFileName << std::endl;	
exit(1);
}
llvm::raw_fd_ostream TmpOStream(tmpFileDescriptor,true);
std::error_code errorCode;
llvm::raw_fd_ostream OStream(outputFileName,errorCode);
if (targetMachine->addPassesToEmitFile(pass,OStream,nullptr,llvm::CodeGenFileType::CGFT_ObjectFile))
{
llvm::errs() << "TargetMachine can't emit file of this type.";
return;
}
pass.run(*CurModule);
OStream.flush();*/

std::string systemCommand = "clang";//"lld --entry=main ";
systemCommand += " -o ";
systemCommand += outputFileName;
systemCommand += " -c ";
systemCommand += tmpFileName;
system(systemCommand.c_str());
if (remove(tmpFileName) != 0 )
{
std::cerr << "Error deleting file: " << tmpFileName << std::endl;
}
}

void genContext::printExeCode(std::string outputFileName)
{

char tmpFileName [L_tmpnam+3];
tmpnam(tmpFileName);
strncat(tmpFileName,".ll",4);
std::error_code errorCode;
llvm::raw_fd_ostream OStream(tmpFileName,errorCode,llvm::sys::fs::CreationDisposition::CD_CreateNew);
CurModule->print(OStream,nullptr);

/*char tmpFileName[] = "/tmp/llvmTemp.XXXXXX";
int tmpFileDescriptor;
if ((tmpFileDescriptor = mkstemp(tmpFileName))==-1) {
std::cerr << "Cannot create tmp file:" << tmpFileName << std::endl;	
exit(1);
}
llvm::raw_fd_ostream TmpOStream(tmpFileDescriptor,true);
if (targetMachine->addPassesToEmitFile(pass,TmpOStream,nullptr,llvm::CodeGenFileType::CGFT_ObjectFile))
{
llvm::errs() << "TargetMachine can't emit file of this type.";
return;
}
pass.run(*CurModule);
TmpOStream.flush();*/

std::string systemCommand = "clang ";//"lld --entry=main ";
systemCommand += tmpFileName;
systemCommand += " -o ";
systemCommand += outputFileName;

system(systemCommand.c_str());
if( remove(tmpFileName) != 0 )
{
std::cerr << "Error deleting file: " << tmpFileName << std::endl;
}
}

void genContext::printLLVMCode(std::string outputFileName)
{
std::error_code errorCode;
llvm::raw_fd_ostream OStream(outputFileName,errorCode);
CurModule->print(OStream,nullptr);
}

void genContext::initializeTarget(const std::string& triple,std::string& errorString,llvm::StringRef CPU)
{
llvm::Triple tripleObj(triple);
target = llvm::TargetRegistry::lookupTarget(triple,errorString);
auto reloc = llvm::Optional<llvm::Reloc::Model>();
llvm::TargetOptions options; // = llvm::codegen::InitTargetOptionsFromCodeGenFlags();
std::string features = "";
targetMachine = target->createTargetMachine(triple,CPU,features,options,reloc /*llvm::codegen::getRelocModel()*/ );
CurModule->setDataLayout(targetMachine->createDataLayout());
CurModule->setTargetTriple(triple);
}

void genContext::initializeTargetList()
{
llvm::InitializeAllTargetInfos();
llvm::InitializeAllTargets();
llvm::InitializeAllTargetMCs();
llvm::InitializeAllAsmPrinters();
llvm::InitializeAllAsmParsers();
}

void genContext::initializeNativeTargetList()
{
llvm::InitializeNativeTarget();
llvm::InitializeNativeTargetAsmPrinter();
llvm::InitializeNativeTargetAsmParser();
}

void genContext::codeGen()
{
while (!codeObjects.empty())
{
codeObjects.front() -> codeGen(*this);
codeObjects.front().reset();
codeObjects.pop();
}
}

AstBlockPtr& genContext::backBlock()
{
return blockList.back();
}

genContext currentContext;

