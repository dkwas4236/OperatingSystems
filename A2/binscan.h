#ifndef BINSCAN_H
#define BINSCAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

// Function: elf
// Purpose: to check if file is ELF executable
// Parameters: FIlE *file
int elf(FILE *file);


// Function: shebang
// Purpose: to determine if there is a shebang and if so what follows it
// Parameters: FILE *file
int shebang(FILE *file);

// Function: directory
// Purpose: to search given directory files and get total number of each file type
// Parameters: const char * givenDirectory (opendir requires const char*)
void directory(const char *givenDirectory);

#endif

