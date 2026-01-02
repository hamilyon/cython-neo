# LLVM Hello World Project

A beginner-friendly introduction to LLVM that demonstrates how to generate, compile, and execute LLVM Intermediate Representation (IR) code.

## Overview

This project shows how to:
- Generate LLVM IR code programmatically using the LLVM C++ API
- Verify and output LLVM IR
- Compile IR to assembly with `llc`
- Assemble and link to create an executable
- Execute the final program

The example creates a simple program that prints "Hello, World!" to the console.

## Project Structure

```
llvm-hello-world/
├── src/
│   └── hello.cpp          # LLVM IR generator (generates main function)
├── build/                 # Build directory (created by make)
├── CMakeLists.txt         # CMake build configuration
├── Makefile               # Convenient build targets
└── README.md              # This file
```

## Prerequisites

You need to have LLVM installed on your system. Installation instructions vary by platform:

### macOS
```bash
brew install llvm
```

### Ubuntu/Debian
```bash
sudo apt-get install llvm llvm-dev clang
```

### Fedora/RHEL
```bash
sudo dnf install llvm llvm-devel clang
```

### Building from Source (Universal)
```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang ../llvm
make -j$(nproc)
sudo make install
```

### Verify Installation
```bash
llvm-config --version
llc --version
```

## Quick Start

### Step 1: Configure the Build System
```bash
make configure
```

This runs CMake to detect LLVM on your system and generate build files.

### Step 2: Build the IR Generator
```bash
make build
```

This compiles `hello.cpp` into the `llvm-hello-gen` executable.

### Step 3: Generate LLVM IR
```bash
make generate
```

This runs the generator and produces `hello_world.ll` (the LLVM IR file).

### Step 4: Full Build Chain
```bash
make all
```

This runs configure and build in one command.

## Detailed Build Targets

| Target | Description |
|--------|-------------|
| `make help` | Show this help message |
| `make configure` | Run CMake to configure the build |
| `make build` | Compile the IR generator |
| `make generate` | Generate `hello_world.ll` IR file |
| `make compile` | Compile IR to assembly (`hello_world.s`) |
| `make assemble` | Assemble to object file (`hello_world.o`) |
| `make link` | Link to final executable (`hello_world`) |
| `make run` | Execute the hello world program |
| `make all` | Configure + build (default) |
| `make clean` | Remove build artifacts |
| `make distclean` | Remove all generated files |

## Complete Workflow Example

```bash
# One-time setup
make configure

# Generate IR
make generate

# View the generated LLVM IR
cat build/hello_world.ll

# Compile to assembly
make compile
cat build/hello_world.s

# Assemble and link
make assemble
make link

# Run the program
make run
# Output: Hello, World!
```

Or do everything in one command:
```bash
make link && make run
```

## Understanding the Code

### hello.cpp - IR Generator

The main program demonstrates:

1. **Creating an LLVM Module** - Container for all IR
   ```cpp
   auto module = std::make_unique<Module>("hello_world", context);
   ```

2. **Declaring External Functions** - Reference to `puts()`
   ```cpp
   auto putsFunc = Function::Create(putsFuncType, Function::ExternalLinkage, "puts", module.get());
   ```

3. **Defining a Function** - Create `main()` function
   ```cpp
   auto mainFunc = Function::Create(mainFuncType, Function::ExternalLinkage, "main", module.get());
   ```

4. **Building Code with IRBuilder** - Add instructions
   ```cpp
   IRBuilder<> builder(entryBB);
   auto helloStr = builder.CreateGlobalStringPtr("Hello, World!", "hello_str");
   builder.CreateCall(putsFunc, {helloStr});
   builder.CreateRet(ConstantInt::get(Type::getInt32Ty(context), 0));
   ```

5. **Verification** - Ensure IR is valid
   ```cpp
   verifyModule(*module, &errorStream);
   ```

6. **Output** - Write IR to file
   ```cpp
   module->print(outFile, nullptr);
   ```

## Generated LLVM IR

After running `make generate`, `hello_world.ll` will contain IR like:

```llvm
; ModuleID = 'hello_world'
source_filename = "hello_world"

@hello_str = private unnamed_addr constant [14 x i8] c"Hello, World!\00", align 1

declare i32 @puts(i8*)

define i32 @main() {
entry:
  %0 = call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @hello_str, i32 0, i32 0))
  ret i32 0
}
```

## Compilation Pipeline

```
hello.cpp (C++)
    ↓
[cmake + g++]
    ↓
llvm-hello-gen (executable)
    ↓
[./llvm-hello-gen]
    ↓
hello_world.ll (LLVM IR)
    ↓
[llc]
    ↓
hello_world.s (assembly)
    ↓
[as]
    ↓
hello_world.o (object file)
    ↓
[gcc]
    ↓
hello_world (executable)
    ↓
[./hello_world]
    ↓
Output: "Hello, World!"
```

## Learning Path

1. **Start Here**: Read this README to understand the overall concept
2. **Build It**: Run `make configure && make build && make generate`
3. **Examine Output**: Look at `build/hello_world.ll` to see the IR
4. **Read the Code**: Study `src/hello.cpp` and its comments
5. **Modify It**: Try changing the string or adding more functions
6. **Explore**: Check LLVM documentation for more features

## Next Steps

From here, you can:
- Add more functions to the IR generator
- Generate different types of LLVM IR constructs (loops, conditionals, etc.)
- Create an optimized IR compiler
- Build a simple language compiler using this as a foundation
- Explore LLVM optimization passes

## References

- [LLVM Official Documentation](https://llvm.org/docs/)
- [LLVM Language Reference Manual](https://llvm.org/docs/LangRef/)
- [LLVM Programmer's Manual](https://llvm.org/docs/ProgrammersManual/)
- [LLVM Tutorial](https://llvm.org/docs/tutorial/)

## Troubleshooting

### CMake can't find LLVM
Make sure LLVM is installed and in your PATH:
```bash
llvm-config --prefix
```

If needed, tell CMake where to find LLVM:
```bash
cmake -DLLVM_DIR=$(llvm-config --cmakedir) ..
```

### Missing dependencies
Install build essentials:
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake llvm llvm-dev

# macOS
brew install cmake llvm
```

### IR compilation fails
Make sure you have `llc` (LLVM compiler) installed:
```bash
which llc
llc --version
```

## License

This project is provided as educational material.
