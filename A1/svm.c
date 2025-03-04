// name: darion kwasnitza
// id: 3122890
// file: svm.c
#include "svm.h" 

unsigned char memory[MEMORY_SIZE];
int registers[NUM_REG] = {0};
int pc = 0;  // init program counter to 0

// zero flag, neg flag, overflow flag
int Z = 0; 
int N = 0; 
int O = 0; 

/* Function: update_flags(int)
 * Purpose: updates flags based on result*/
void update_flags(int result) {
	// ? : if condition is true set to _
	Z = (result == 0) ? 1 : 0;  // if result is 0 set z
	N = (result < 0) ? 1 : 0;   // if result is - set n
	O = (result > 32767 || result < -32768) ? 1 : 0; // if overflow set O
}

/* Function: decode_and_execute(unsigned char, unsigned char)
 * Purpose: to decode instructions and then execute them */
void decode_and_execute(unsigned char opcode, unsigned char reg) {
	int immSRC;  // immediate source for instructions
	int result;  
	unsigned char reg2; // for addr

	switch (opcode) {
    	case HALT:
        	printf("HALT instruction executed. Exiting program.\n");
        	exit(0); 
    	case LOAD:
        	immSRC = memory[pc + 2];
        	registers[reg] = immSRC; 
        	update_flags(registers[reg]);
        	break;
    	case LOADI:
        	registers[reg] = memory[registers[reg]];
        	update_flags(registers[reg]);
        	break;
    	case STORE:
        	immSRC = memory[pc + 2];
        	memory[immSRC] = registers[reg]; 
        	break;
    	case STOREI:
        	memory[registers[reg]] = registers[reg];
        	break;
    	case JMP:
        	pc = (reg << 8) | memory[pc + 2]; // jump to address
        	return; // skip PC
    	case JMPZ:
        	if (Z) {
            	pc = (reg << 8) | memory[pc + 2]; // jump if Z flag is set
            	return; // skip PC
        	}
        	break;
    	case JMPN:
        	if (N) {
            	pc = (reg << 8) | memory[pc + 2]; // jump if N flag is set
            	return; // skip PC
        	}
        	break;
    	case JMPO:
        	if (O) {
            	pc = (reg << 8) | memory[pc + 2]; // jump if O flag is set
            	return; // skip PC
        	}
        	break;
    	case ADD:
        	immSRC = memory[pc + 2];
        	result = registers[reg] + immSRC; // perform addition
        	registers[reg] = result; 
        	update_flags(result); 
        	break;
    	case ADDR:
		reg2 = memory[pc+2];
        	result = registers[reg] + registers[reg2];
        	registers[reg] = result;
        	update_flags(result); 
        	break;
    	case SUB:
        	immSRC = memory[pc + 2];
        	result = registers[reg] - immSRC; // perform subtraction
        	registers[reg] = result; 
        	update_flags(result);
        	break;
    	case SUBR:
        	result = registers[reg] - registers[pc + 2];
        	registers[reg] = result;
        	update_flags(result); 
        	break;
    	case OUT:
        	immSRC = memory[pc + 2]; // output immediate value
        	printf("%d\n", immSRC);
        	break;
    	case OUTC:
        	immSRC = memory[pc + 2]; // output immediate character value
        	printf("%c\n", (char)immSRC);
        	break;
    	case OUTR:
        	printf("%d: %d\n", reg, registers[reg]);
        	break;
    	case OUTRC:
        	printf("%d: %c\n", reg, (char)registers[reg]);
        	break;
    	case OUTI:
        	printf("%d\n", memory[registers[reg]]);
        	break;
    	case OUTIC:
        	printf("%c\n", (char)memory[registers[reg]]);
        	break;
    	default:
        	printf("Unknown instruction: 0x%x (opcode: 0x%x, reg: 0x%x)\n", opcode, opcode, reg);
        	break;
	}
}


int main() {
	// init memory with data from stdin
	for (int i = 0; i < MEMORY_SIZE; i++) {
    	int byte = getchar();
    	if (byte == EOF) {
        	break;
    	}
    	memory[i] = (unsigned char)byte;
	}

	// read instructions from memory until HALT
	while (pc < MEMORY_SIZE) {
    	unsigned char opcode = memory[pc + 0];
    	unsigned char reg = memory[pc + 1];	

    	// decode and execute the instruction
    	decode_and_execute(opcode, reg);

    	// determine how to skip additional bytes based on the opcode
    	if (opcode == LOAD || opcode == JMP || opcode == JMPZ || opcode == STORE || opcode == ADD || opcode == SUB || opcode == OUTC) {
        	pc += 4; // increment PC by 4 for 4-byte instructions
    	} else {
        	pc += 2; // increment PC by 2 for 2-byte instructions
    	}
    }
	return 0;
}

