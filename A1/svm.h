// name: darion kwasnitza
// id: 3122890
// file: svm.h
//
#ifndef SVM_H
#define SVM_H

#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 32768

/* Opcodes for 80d201 processor */
#define HALT 0x31
#define LOAD 0x60
#define LOADI 0x61
#define STORE 0x62
#define STOREI 0x63
#define JMP 0x64
#define JMPZ 0x65
#define JMPN 0x66
#define JMPO 0x67
#define ADD 0x68
#define ADDR 0x69
#define SUB 0x6A
#define SUBR 0x6B
#define OUT 0x6C
#define OUTC 0x6D
#define OUTR 0x6E
#define OUTRC 0x6F
#define OUTI 0x70
#define OUTIC 0x71

/* Registers for 80d201 processor */
#define A1 3
#define A2 2
#define R1 1
#define R2 0
#define NUM_REG 4

extern unsigned char memory[MEMORY_SIZE];
extern int registers[NUM_REG];
extern int pc;  // Program counter

// Flags
extern int Z;
extern int N;
extern int O;

// Function prototypes
void update_flags(int);
void decode_and_execute(unsigned char, unsigned char);

#endif


