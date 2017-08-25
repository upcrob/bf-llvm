# bf-llvm

This is a simple implementation of a [Bf](https://en.wikipedia.org/wiki/Brainfuck) to [LLVM](http://llvm.org/) IR compiler.  It has no practical purpose beyond providing a sample project for playing with LLVM.

This compiler was built and tested with LLVM 4.0.1.

## Building

This project can be built with the included Makefile.  The executable is called, `bfc`.

## Usage

Running Hello World:

	cat examples/hello.bf | ./bfc > hello.ll
	lli hello.ll

Running the IO Example (this will print "ABC"):

	cat examples/io.bf | ./bfc > io.ll
	echo "ABC" | lli io.ll

## Compiling to a Binary

Alternatively, these examples can be compiled to binary files with `llc` and `clang`:

	cat examples/hello.bf | ./bfc > hello.ll
	llc hello.ll
	clang -s hello.s
	./a.out
