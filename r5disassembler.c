#include "r5disassembler.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *registers[] = {"x0", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2", // name of each register in RISC-V
                           "s0", "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                           "a6", "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                           "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

int main(int argc, char *argv[]) {
  if (argc == 1) { // use stdin for user input
    char *Instruction32Bit = (char *)calloc(32, sizeof(char));
    int flag = 1;
    while (flag) {
      printf("Please enter a 32-bit binary string:\n");
      scanf("%s", Instruction32Bit);
      if (strlen(Instruction32Bit) == 32) {
        flag = 0;
      }
      else {
        printf("The entered string is not 32-bits long!\n");
      }
    }
    dissassembleInstruction(Instruction32Bit);
    free(Instruction32Bit);
    return 0;
  } else {
    readFile(argv[1]);
    return 0;
  }
}

void readFile(char *fileName) {
  FILE *filePointer = fopen(fileName, "r");
  if (!filePointer) {
    printf("Error opening input file.\n");
    exit(-1);
  }
  fseek(filePointer, 0, SEEK_END);  // Seek all the way to the end of the file
  size_t size = ftell(filePointer); // Calculate size by grabbing the current position of the file-pointer
  rewind(filePointer);              // Rewind the file pointer to read properly
  unsigned offset = 0;
  char *Instruction32Bit = (char *)calloc(32, sizeof(char));
  fscanf(filePointer, " "); // skip any whitespace at the beggining of the file
  while (offset + 32 <= size) {
    fgets(Instruction32Bit, 33, filePointer); 
    printf("%x%s", offset/8,":\t");
    dissassembleInstruction(Instruction32Bit);
    fscanf(filePointer, " "); // Skip present whitespace
    offset += 32;
  }
  free(Instruction32Bit);
}

void UJFormatRegisters(char *Instruction32Bit) {
  char rd[6]; // Grab the Destination Register
  memcpy(rd, &Instruction32Bit[20], 5);
  rd[5] = '\0';
  long rdInt =
      strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  char *registerName = registers[rdInt];
  printf("%s%s", registerName, ", "); // Print the Destination Register

  char imm[22];  // Reconstruct the Immediate
  imm[20] = '0'; // Append the Implicit 0.
  imm[21] = '\0';
  imm[0] = Instruction32Bit[0];
  memcpy(&imm[1], &Instruction32Bit[12], 8);
  imm[9] = Instruction32Bit[11];
  memcpy(&imm[10], &Instruction32Bit[1], 10);
  long immDec =
      strtol(imm, NULL, 2); // Convert from binary string to base10 decimal
  if (immDec > 1048575) {   // Convert using Two's Complement
    immDec -= 2097152;
  }
  printf("%ld%s", immDec, "\n"); // Print the Immediate
}

void UFormat(char *Instruction32Bit) {
  char rd[6]; // Grab the destination register
  memcpy(rd, &Instruction32Bit[20], 5);
  rd[5] = '\0';
  long rdInt =
      strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  char *registerName = registers[rdInt]; // Print the destination register
  printf("%s%s", registerName, ", ");
  char imm[21];
  memcpy(imm, Instruction32Bit, 20); // Grab the immediate
  imm[20] = '\0';
  long immDec =
      strtol(imm, NULL, 2);      // Convert from binary string to base10 decimal
  printf("%ld%s", immDec, "\n"); // Print the immediate
}

void SFormatRegisters(char *Instruction32Bit) {
  char rd[6]; // Grab rs1
  memcpy(rd, &Instruction32Bit[7], 5);
  rd[5] = '\0';
  long rdInt =
      strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  char *registerName = registers[rdInt];
  printf("%s%s", registerName, ", "); // print rs1

  char imm[13]; // grab the immediate
  memcpy(&imm[7], &Instruction32Bit[20], 5);
  imm[12] = '\0';
  memcpy(imm, &Instruction32Bit[0], 7);
  long immDec =
      strtol(imm, NULL, 2); // Convert from binary string to base10 decimal
  if (immDec > 2047) {
    immDec -= 4096;
  }
  printf("%ld", immDec); // print the immediate

  memcpy(rd, &Instruction32Bit[12], 5); // grab rs2
  rd[5] = '\0';
  rdInt = strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  registerName = registers[rdInt];
  printf("%s%s%s", "(", registerName,
         ")\n"); // print rs2 using proper formatting i.e "sw rs1 imm(rs2)"
}

void IFormatRegisters(char *Instruction32Bit) {
  char rd[6]; // Grab the destination register
  memcpy(rd, &Instruction32Bit[20], 5);
  rd[5] = '\0';
  long rdInt =
      strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  char *registerName = registers[rdInt];
  printf("%s%s", registerName, ", ");   // Print the destination register
  memcpy(rd, &Instruction32Bit[12], 5); // Grab rs1
  rd[5] = '\0';
  rdInt = strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  registerName = registers[rdInt];
  printf("%s%s", registerName, ", "); // Print rs1
}

void IFormat1Registers(char *Instruction32Bit) {
  char rd[6]; // Grab the destination register
  memcpy(rd, &Instruction32Bit[20], 5);
  rd[5] = '\0';
  long rdInt =
      strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  char *registerName = registers[rdInt];
  printf("%s%s", registerName, ", "); // Print the destination register

  char imm[13]; // Grab the immediate
  memcpy(imm, &Instruction32Bit[0], 12);
  imm[12] = '\0';
  long immDec =
      strtol(imm, NULL, 2); // Convert from binary string to base10 decimal
  if (immDec > 2047) {      // Convert using Two's Complement
    immDec -= 4096;
  }
  printf("%ld", immDec); // Print the immediate

  // print rs1
  memcpy(rd, &Instruction32Bit[12], 5);
  rd[5] = '\0';
  rdInt = strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  registerName = registers[rdInt];
  printf("%s%s%s", "(", registerName, ")\n");
}

void IFormatImmediate(char *Instruction32Bit) {
  char imm[13]; // Grab the immediate
  memcpy(imm, &Instruction32Bit[0], 12);
  imm[12] = '\0';
  long immDec =
      strtol(imm, NULL, 2); // Convert from binary string to base10 decimal
  if (immDec > 2047) {      // Convert using Two's Complement
    immDec -= 4096;
  }
  printf("%ld%s", immDec, "\n"); // Print the immediate
}

void IFormat5BitImmediate(char *Instruction32Bit) {
  char imm[6]; // Grab the 5-bit immediate
  memcpy(imm, &Instruction32Bit[7], 5);
  imm[5] = '\0';
  long immDec =
      strtol(imm, NULL, 2);      // Convert from binary string to base10 decimal
  printf("%ld%s", immDec, "\n"); // Print the 5-bit immediate
}

void SBFormatRegisters(char *Instruction32Bit) {
  char rd[6]; // Grab rs2
  memcpy(rd, &Instruction32Bit[12], 5);
  rd[5] = '\0';
  long rdInt =
      strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  char *registerName = registers[rdInt]; // Print rs2
  printf("%s%s", registerName, ", ");
  memcpy(rd, &Instruction32Bit[7], 5); // Grab rs1
  rd[5] = '\0';
  rdInt = strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  registerName = registers[rdInt];
  printf("%s%s", registerName, ", "); // Print rs1

  char imm[14]; // Reconstruct the Immediate
  imm[0] = Instruction32Bit[0];
  imm[1] = Instruction32Bit[24];
  memcpy(&imm[2], &Instruction32Bit[1], 6);
  memcpy(&imm[8], &Instruction32Bit[20], 4);
  imm[12] = '0'; // Append the implict 0.
  imm[13] = '\0';
  long immDec =
      strtol(imm, NULL, 2); // Convert from binary string to base10 decimal
  if (immDec > 4095) {      // Convert using Two's Complement
    immDec -= 8192;
  }
  printf("%ld%s", immDec, "\n"); // Print the Immediate
}

void RFormatRegisters(char *Instruction32Bit) {
  char rd[6];
  memcpy(rd, &Instruction32Bit[20], 5);
  rd[5] = '\0';
  long rdInt =
      strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  char *registerName = registers[rdInt];
  printf("%s%s", registerName, ", ");
  // print rs1
  memcpy(rd, &Instruction32Bit[12], 5);
  rd[5] = '\0';
  rdInt = strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  registerName = registers[rdInt];
  printf("%s%s", registerName, ", ");
  // print rs2
  memcpy(rd, &Instruction32Bit[7], 5);
  rd[5] = '\0';
  rdInt = strtol(rd, NULL, 2); // Convert from binary string to base10 decimal
  registerName = registers[rdInt];
  printf("%s%s", registerName, "\n");
}

void dissassembleInstruction(char *Instruction32Bit) {
  char opcode[8];
  memcpy(opcode, &Instruction32Bit[25], 7); // Grab the Opcode
  opcode[7] = '\0';
  int opcodeInt = atoi(opcode);
  switch (opcodeInt) {
  case 11:                      // Switch depending on the Opcode
    IFormat1(Instruction32Bit); // For instructions which load from memory
    break;
  case 10011:
    IFormat2(Instruction32Bit); // Common immediate instructions
    break;
  case 1100111: // jalr is the only instruction with opcode 1100011
    printf("%s", "jalr ");
    IFormatRegisters(Instruction32Bit);
    IFormatImmediate(Instruction32Bit);
    break;
  case 10111:
    printf("%s", "auipc "); // auipc is the only instruction with opcode 0010111
    UFormat(Instruction32Bit);
    break;
  case 110111:
    printf("%s", "lui "); // lui is the only instruction with opcode 0110111
    UFormat(Instruction32Bit);
  case 100011:
    SFormat(Instruction32Bit); // For instructions which store into memory
    break;
  case 110011:
    RFormat(Instruction32Bit); // Arithmetic Functions
    break;
  case 111011:
    RFormat2(Instruction32Bit); // Word versions (only operates on the rightmost
                                // 32 bits of 64-bit registers)
    break;
  case 1100011:
    SBFormat(Instruction32Bit); // Branch instructions
    break;
  case 1101111:
    printf("%s", "jal "); // jal is the only instruction with opcode 1101111.
                          // (Also the only opcode with UJ-format)
    UJFormatRegisters(Instruction32Bit);
    break;
  case 1110011: 
    if (Instruction32Bit[11] == '1') {
      printf("ebreak \n");
    }
    else if (Instruction32Bit[11] == '0'){
      printf("ecall \n");
    }
    break;
  }
}

void RFormat(char *Instruction32Bit) {
  char funct3[4]; // Grab funct3
  memcpy(funct3, &Instruction32Bit[17], 3);
  funct3[3] = '\0';
  int funct3Int = atoi(funct3);
  char funct7[8]; // Grab funct7
  memcpy(funct7, &Instruction32Bit[0], 7);
  funct7[7] = '\0';
  int funct7Int = atoi(funct7);
  switch (funct3Int) {
  case 0: // add and sub both have funct3 000
    switch (funct7Int) {
    case 0:
      printf("%s", "add ");
      break;
    case 100000:
      printf("%s", "sub ");
      break;
    }
    break;
  case 1:
    printf("%s", "sll ");
    break;
  case 10:
    printf("%s", "slt ");
    break;
  case 11:
    printf("%s", "sltu ");
    break;
  case 100:
    printf("%s", "xor ");
    break;
  case 101: // srl and sra both have funct3 101
    switch (funct7Int) {
    case 0:
      printf("%s", "srl ");
      break;
    case 100000:
      printf("%s", "sra ");
      break;
    }
    break;
  case 110:
    printf("%s", "or ");
    break;
  case 111:
    printf("%s", "and ");
    break;
  }
  RFormatRegisters(Instruction32Bit); // print rd, rs1, rs2
}

void RFormat2(char *Instruction32Bit) {
  char funct3[4]; // Grab funct3
  memcpy(funct3, &Instruction32Bit[17], 3);
  funct3[3] = '\0';
  int funct3Int = atoi(funct3);
  char funct7[8]; // Grab funct7
  memcpy(funct7, &Instruction32Bit[0], 7);
  funct7[7] = '\0';
  int funct7Int = atoi(funct7);
  switch (funct3Int) {
  case 0: // addw and subw both have funct3 000
    switch (funct7Int) {
    case 0:
      printf("%s", "addw ");
      break;
    case 100000:
      printf("%s", "subw ");
      break;
    }
    break;
  case 1:
    printf("%s", "sllw ");
    break;
  case 101: // srl and sra both have funct3 101
    switch (funct7Int) {
    case 0:
      printf("%s", "srlw ");
      break;
    case 100000:
      printf("%s", "sraw ");
      break;
    }
    break;
  }
  RFormatRegisters(Instruction32Bit); // Print rd, rs1, rs2
}

void SFormat(char *Instruction32Bit) {
  char funct3[4];
  memcpy(funct3, &Instruction32Bit[17], 3);
  funct3[3] = '\0';
  int funct3Int = atoi(funct3);
  switch (funct3Int) {
  case 0:
    printf("%s", "sb ");
    SFormatRegisters(Instruction32Bit);
    break;
  case 1:
    printf("%s", "sh ");
    SFormatRegisters(Instruction32Bit);
    break;
  case 10:
    printf("%s", "sw ");
    SFormatRegisters(Instruction32Bit);
    break;
  case 11:
    printf("%s", "sd ");
    SFormatRegisters(Instruction32Bit);
    break;
  }
}

void IFormat1(char *Instruction32Bit) {
  char funct3[4];
  memcpy(funct3, &Instruction32Bit[17], 3);
  funct3[3] = '\0';
  int funct3Int = atoi(funct3);
  switch (funct3Int) {
  case 0:
    printf("%s", "lb ");
    IFormat1Registers(Instruction32Bit);
    break;
  case 1:
    printf("%s", "lh ");
    IFormat1Registers(Instruction32Bit);
    break;
  case 10:
    printf("%s", "lw ");
    IFormat1Registers(Instruction32Bit);
    break;
  case 11:
    printf("%s", "ld ");
    IFormat1Registers(Instruction32Bit);
    break;
  case 100:
    printf("%s", "lbu ");
    IFormat1Registers(Instruction32Bit);
    break;
  case 101: // special case
    printf("%s", "lhu ");
    IFormat1Registers(Instruction32Bit);
    break;
  case 110:
    printf("%s", "lwu ");
    IFormat1Registers(Instruction32Bit);
    break;
  }
}

void IFormat2(char *Instruction32Bit) {
  char funct3[4];
  memcpy(funct3, &Instruction32Bit[17], 3);
  funct3[3] = '\0';
  int funct3Int = atoi(funct3);
  switch (funct3Int) {
  case 0:
    printf("%s", "addi ");
    IFormatRegisters(Instruction32Bit);
    IFormatImmediate(Instruction32Bit);
    break;
  case 1:
    printf("%s", "slli ");
    IFormatRegisters(Instruction32Bit);
    IFormatImmediate(Instruction32Bit);
    break;
  case 10:
    printf("%s", "slti ");
    IFormatRegisters(Instruction32Bit);
    IFormatImmediate(Instruction32Bit);
    break;
  case 11:
    printf("%s", "sltiu ");
    IFormatRegisters(Instruction32Bit);
    IFormatImmediate(Instruction32Bit);
    break;
  case 100:
    printf("%s", "xori ");
    IFormatRegisters(Instruction32Bit);
    IFormatImmediate(Instruction32Bit);
    break;
  case 101: // special case
    IFormat2Funct7(Instruction32Bit);
    break;
  case 110:
    printf("%s", "ori ");
    IFormatRegisters(Instruction32Bit);
    IFormatImmediate(Instruction32Bit);
    break;
  case 111:
    printf("%s", "andi ");
    IFormatRegisters(Instruction32Bit);
    IFormatImmediate(Instruction32Bit);
    break;
  }
}

void IFormat2Funct7(
    char *Instruction32Bit) { // for I-Format Instructions which use funct7
  char funct7[8];             // Grab funct7
  memcpy(funct7, &Instruction32Bit[0], 7);
  funct7[7] = '\0';
  int funct7Int = atoi(funct7);
  switch (funct7Int) {
  case 0:
    printf("%s", "srli ");
    break;
  case 100000:
    printf("%s", "srai ");
    break;
  }
  IFormatRegisters(Instruction32Bit);     // Print ra, rs1
  IFormat5BitImmediate(Instruction32Bit); // Print the immediate
}

void SBFormat(char *Instruction32Bit) {
  char funct3[4];
  memcpy(funct3, &Instruction32Bit[17], 3); // Grab funct3
  funct3[3] = '\0';
  int funct3Int = atoi(funct3);
  switch (funct3Int) { // Switch depending on funct3
  case 0:
    printf("%s", "beq ");
    SBFormatRegisters(Instruction32Bit);
    break;
  case 1:
    printf("%s", "bne ");
    SBFormatRegisters(Instruction32Bit);
    break;
  case 100:
    printf("%s", "blt ");
    SBFormatRegisters(Instruction32Bit);
    break;
  case 101:
    printf("%s", "bge ");
    SBFormatRegisters(Instruction32Bit);
    break;
  case 110:
    printf("%s", "bltu ");
    SBFormatRegisters(Instruction32Bit);
    break;
  case 111:
    printf("%s", "bgeu ");
    SBFormatRegisters(Instruction32Bit);
    break;
  }
}