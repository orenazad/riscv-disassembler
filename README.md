# riscv-disassembler

## What is a disassembler? 

A disassembler is a computer program that translates machine language into assembly language—the inverse operation to that of an assembler. Here are some sample RISC-V Instructions represented in assembly and machine language. 

| Assembly Language | Machine Language                   | Instruction Description                             |
|-------------------|------------------------------------|-----------------------------------------------------|
| `lui a2, 44`      | `00000000000000101100011000110111` | Loads the 20 upper-bits of 44 into register a2.     |
| `blt a4, a5, 8`   | `00000000111101110100010001100011` | Skips the next instruction if R[a4] < R[a5].        |
| `addi sp, sp, -4` | `11111111110000010000000100010011` | Decrements the stack pointer by one word (4 bytes). |
| `sw s0, 0(sp)`    | `00000000100000010010000000100011` | Stores the value at R[s0] into the stack.           |
| `jalr x0, ra, 0`  | `00000000000000001000000001100111` | Jumps to the address specified in R[ra].            |

## Output

Run the executable on a text file containing machine language to dissassemble and return all instructions in the input file.

```console
$ ./r5dis sample_input.txt
0:      blt a5, s7, -68
4:      sh s2, 49(s6)
8:      lui a2, 44
c:      ebreak 
10:     ecall 
14:     blt a4, a5, 8
18:     jalr x0, ra, 0
1c:     addi sp, sp, -4
20:     sw s0, 0(sp)
```

If run with no arguments, the executable will accept user input and return the disassembled instruction. 

```console
$ ./r5dis
Please enter a 32-bit binary string:
11111111111100101000010000010011
addi s0, t0, -1
```

## Build Instructions

Just run `make` in order to build the executable!
```console
$ make
```
