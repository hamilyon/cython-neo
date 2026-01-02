# Quick Start Guide

## One-Time Setup

```bash
make configure
```

## Generate and Run

```bash
make link
./build/hello_world
```

Output:
```
Hello, World!
```

## View the Pipeline

See the generated files:
- `build/hello_world.ll` - LLVM IR
- `build/hello_world.s` - Assembly code
- `build/hello_world.o` - Object file
- `build/hello_world` - Final executable

```bash
# View LLVM IR
cat build/hello_world.ll

# View assembly
cat build/hello_world.s

# View all help
make help
```

## Modify and Extend

Edit `src/hello.cpp` to:
- Change the greeting message
- Add more functions
- Generate different LLVM IR constructs

Then rebuild:
```bash
make clean
make link
./build/hello_world
```

## Key Targets

| Command | What it does |
|---------|-------------|
| `make configure` | Initialize build system (run first) |
| `make build` | Compile the IR generator |
| `make generate` | Generate LLVM IR code |
| `make compile` | Convert IR to assembly |
| `make link` | Create final executable |
| `make run` | Execute the program |
| `make clean` | Remove build artifacts |

## Full Documentation

See `README.md` for comprehensive documentation.
