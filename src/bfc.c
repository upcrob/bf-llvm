/*
 * Simple BF -> LLVM IR compiler implementation for LLVM practice and reference.
 */

#include <stdio.h>

// amount of memory to allocate for the virtual tape
#define TAPE_SIZE 10000

int main(void)
{
	// emit llvm setup code
	printf("declare i8 @putchar(i8)\n");
	printf("declare i8 @getchar()\n");

	printf("define i32 @main() {\n");

	printf("\t%%mem = alloca i8, i32 %d\n", TAPE_SIZE);
	printf("\t%%offsetPtr = alloca i32, i32 1\n");
	printf("\tstore i32 1, i32* %%offsetPtr\n\n");

	// memory zeroing code
	printf("\t%%countRef = alloca i32, i32 1\n");
	printf("\tstore i32 0, i32* %%countRef\n");
	printf("\t%%icond = icmp eq i32 %d, 0\n", TAPE_SIZE);
	printf("\tbr i1 %%icond, label %%doneinit, label %%initloop\n");
	printf("\tinitloop:\n");
	printf("\t\t%%countval = load i32, i32* %%countRef\n");
	printf("\t\t%%countvalInc = add i32 %%countval, 1\n");
	printf("\t\t%%byteRef = getelementptr i8, i8* %%mem, i32 %%countval\n");
	printf("\t\tstore i8 0, i8* %%byteRef\n");
	printf("\t\tstore i32 %%countvalInc, i32* %%countRef\n");
	printf("\t\t%%dcond = icmp eq i32 %d, %%countvalInc\n", TAPE_SIZE);
	printf("\t\tbr i1 %%dcond, label %%doneinit, label %%initloop\n");
	printf("\t\tdoneinit:\n\n");

	// compile all instructions
	int r = 0;
	int label = 0;
	int c;
	do {
		c = getchar();
		switch (c) {
			case '>':
				// increment data pointer
				printf("\t; increment data pointer\n");
				printf("\t%%offset%d = load i32, i32* %%offsetPtr\n", r);
				printf("\t%%offsetInc%d = add i32 %%offset%d, 1\n", r, r);
				printf("\tstore i32 %%offsetInc%d, i32* %%offsetPtr\n", r);
				break;
			case '<':
				// decrement data pointer
				printf("\t; decrement data pointer\n");
				printf("\t%%offset%d = load i32, i32* %%offsetPtr\n", r);
				printf("\t%%offsetDec%d = sub i32 %%offset%d, 1\n", r, r);
				printf("\tstore i32 %%offsetDec%d, i32* %%offsetPtr\n", r);
				break;
			case '+':
				// increment the byte at the data pointer
				printf("\t; increment byte\n");
				printf("\t%%offset%d = load i32, i32* %%offsetPtr\n", r);
				printf("\t%%valuePtr%d = getelementptr i8, i8* %%mem, i32 %%offset%d\n", r, r);
				printf("\t%%value%d = load i8, i8* %%valuePtr%d\n", r, r);
				printf("\t%%valueInc%d = add i8 1, %%value%d\n", r, r);
				printf("\tstore i8 %%valueInc%d, i8* %%valuePtr%d\n", r, r);
				break;
			case '-':
				// decrement the byte at the data pointer
				printf("\t; decrement byte\n");
				printf("\t%%offset%d = load i32, i32* %%offsetPtr\n", r);
				printf("\t%%valuePtr%d = getelementptr i8, i8* %%mem, i32 %%offset%d\n", r, r);
				printf("\t%%value%d = load i8, i8* %%valuePtr%d\n", r, r);
				printf("\t%%valueDec%d = sub i8 %%value%d, 1\n", r, r);
				printf("\tstore i8 %%valueDec%d, i8* %%valuePtr%d\n", r, r);
				break;
			case '.':
				// output the byte at the data pointer
				printf("\t; output byte\n");
				printf("\t%%offset%d = load i32, i32* %%offsetPtr\n", r);
				printf("\t%%valuePtr%d = getelementptr i8, i8* %%mem, i32 %%offset%d\n", r, r);
				printf("\t%%value%d = load i8, i8* %%valuePtr%d\n", r, r);
				printf("\tcall i8 (i8) @putchar(i8 %%value%d)\n", r);
				break;
			case ',':
				// input a byte from standard in to the data pointer
				printf("\t%%input%d = call i8 () @getchar()\n", r);
				printf("\t%%offset%d = load i32, i32* %%offsetPtr\n", r);
				printf("\t%%valuePtr%d = getelementptr i8, i8* %%mem, i32 %%offset%d\n", r, r);
				printf("\tstore i8 %%input%d, i8* %%valuePtr%d\n", r, r);
				break;
			case '[':
				// if byte at data pointer is zero, jump to command after matching ']'
				label++;
				printf("\t; begin loop %d\n", label);
				printf("\t%%offset%d = load i32, i32* %%offsetPtr\n", r);
				printf("\t%%valuePtr%d = getelementptr i8, i8* %%mem, i32 %%offset%d\n", r, r);
				printf("\t%%value%d = load i8, i8* %%valuePtr%d\n", r, r);
				printf("\t%%cond%d = icmp eq i8 0, %%value%d\n", r, r);
				printf("\tbr i1 %%cond%d, label %%iszero%d, label %%isnonzero%d\n", r, label, label);
				printf("\tisnonzero%d:\n", label);
				break;
			case ']':
				// if byte at data pointer is non-zero, jump to command after matching '['
				printf("\t; end loop %d\n", label);
				printf("\t%%offset%d = load i32, i32* %%offsetPtr\n", r);
				printf("\t%%valuePtr%d = getelementptr i8, i8* %%mem, i32 %%offset%d\n", r, r);
				printf("\t%%value%d = load i8, i8* %%valuePtr%d\n", r, r);
				printf("\t%%cond%d = icmp eq i8 0, %%value%d\n", r, r);
				printf("\tbr i1 %%cond%d, label %%iszero%d, label %%isnonzero%d\n", r, label, label);
				printf("\tiszero%d:\n", label);
				label--;
				break;
			default:
				// skip characters
				break;
		}
		r++;
	} while (c > 0);

	// emit final llvm code
	printf("\tret i32 0\n");
	printf("}\n");
}
