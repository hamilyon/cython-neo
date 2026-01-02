.PHONY: help configure build generate compile assemble link run clean distclean

help:
	@echo "LLVM Hello World Project"
	@echo "======================="
	@echo ""
	@echo "Available targets:"
	@echo "  make configure - Configure the build system (run first)"
	@echo "  make build     - Build the IR generator"
	@echo "  make generate  - Generate LLVM IR code"
	@echo "  make compile   - Compile IR to assembly"
	@echo "  make assemble  - Assemble to object file"
	@echo "  make link      - Link to executable"
	@echo "  make run       - Run the hello world program"
	@echo "  make all       - Configure, build, and generate (default)"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make distclean - Remove all generated files"
	@echo ""

configure:
	mkdir -p build
	cd build && cmake ..

build: configure
	cd build && make

generate: build
	cd build && make generate

compile: generate
	cd build && make compile

assemble: compile
	cd build && make assemble

link: assemble
	cd build && make link

run: link
	./build/hello_world

all: build generate

clean:
	cd build && make clean

distclean:
	rm -rf build/
	rm -f hello_world.ll hello_world.s hello_world.o hello_world
