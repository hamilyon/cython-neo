#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>

using namespace llvm;

// Generate a simple "hello world" LLVM IR module
std::unique_ptr<Module> generateHelloWorld(LLVMContext &context) {
    // Create a new module
    auto module = std::make_unique<Module>("hello_world", context);

    // Get the data layout and target triple for proper IR generation
    module->setDataLayout("e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128");

    // Declare the puts function: i32 @puts(i8*)
    // Use pointer type without explicit i8Ptr type (LLVM 21 uses opaque pointers)
    auto i32Type = Type::getInt32Ty(context);
    auto ptrType = Type::getInt8Ty(context)->getPointerTo();
    
    auto putsFuncType = FunctionType::get(i32Type, {ptrType}, false);
    auto putsFunc = Function::Create(
        putsFuncType,
        Function::ExternalLinkage,
        "puts",
        module.get()
    );

    // Create the main function: i32 @main()
    auto mainFuncType = FunctionType::get(i32Type, {}, false);
    auto mainFunc = Function::Create(
        mainFuncType,
        Function::ExternalLinkage,
        "main",
        module.get()
    );

    // Create entry basic block
    auto entryBB = BasicBlock::Create(context, "entry", mainFunc);
    IRBuilder<> builder(entryBB);

    // Create a global string constant "Hello, World!"
    auto str = builder.CreateGlobalString("Hello, World!\n", "hello_str");
    auto strPtr = builder.CreatePointerCast(str, ptrType, "str_ptr");

    // Call puts with our string
    builder.CreateCall(putsFunc, {strPtr});

    // Return 0 from main
    builder.CreateRet(ConstantInt::get(i32Type, 0));

    return module;
}

int main() {
    // Initialize LLVM
    LLVMContext context;

    // Generate the module
    auto module = generateHelloWorld(context);

    // Verify the module
    std::string errorMsg;
    raw_string_ostream errorStream(errorMsg);
    if (verifyModule(*module, &errorStream)) {
        errs() << "Module verification failed:\n" << errorMsg;
        return 1;
    }

    // Output LLVM IR to file
    std::error_code ec;
    raw_fd_ostream outFile("hello_world.ll", ec, sys::fs::OF_Text);
    if (ec) {
        errs() << "Error opening file: " << ec.message() << "\n";
        return 1;
    }

    module->print(outFile, nullptr);
    outFile.close();

    // Also print to stdout for visibility
    outs() << "Generated LLVM IR:\n";
    outs() << "==================\n";
    module->print(outs(), nullptr);
    outs() << "==================\n\n";
    outs() << "LLVM IR written to hello_world.ll\n";

    return 0;
}
