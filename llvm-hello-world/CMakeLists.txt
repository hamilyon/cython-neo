cmake_minimum_required(VERSION 3.13)
project(llvm-hello-world)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add LLVM include and link directories
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Set C++ standard to 17 (LLVM 21 requires it)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create the IR generator executable
add_executable(llvm-hello-gen src/hello.cpp)

# Link against LLVM libraries
target_link_libraries(llvm-hello-gen LLVM)

# Set output directory
set_target_properties(llvm-hello-gen PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Custom target to run the generator
add_custom_target(generate
    COMMAND llvm-hello-gen
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS llvm-hello-gen
    COMMENT "Generating LLVM IR..."
)

# Custom target to compile IR to assembly
add_custom_target(compile
    COMMAND llc -o hello_world.s hello_world.ll
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS generate
    COMMENT "Compiling LLVM IR to assembly..."
)

# Custom target to assemble to object file
add_custom_target(assemble
    COMMAND as -o hello_world.o hello_world.s
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS compile
    COMMENT "Assembling to object file..."
)

# Custom target to link to executable
add_custom_target(link
    COMMAND gcc -o hello_world hello_world.o
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS assemble
    COMMENT "Linking to executable..."
)

# Custom target for complete build
add_custom_target(build-all
    DEPENDS link
    COMMENT "Full build complete!"
)
