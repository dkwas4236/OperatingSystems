#include "binscan.h"

int elf(FILE *file) {
	// 'magic number' for ELF = 0x7f E L F (four bytes)
	// use unsigned char for reading bytes (A1)
	unsigned char *magicELF;
	magicELF = malloc(4 * sizeof(unsigned char));
	if (!magicELF) {
    		fprintf(stderr, "ELF Memory Allocation Error\n");
    		return 0;
	}

	// fread(*store data, size read (bytes), # of elements read, *file)
	fread(magicELF, 1, 4, file);

	// return ELF FILE, true if matching
	return (magicELF[0] == 0x7f && magicELF[1] == 'E' && magicELF[2] == 'L' && magicELF[3] == 'F');

	// free pointer
	free(magicELF);
	return 0;
}

int shebang(FILE *file) {
	// buffer size for line will be 2048
	// buffer size is large, should be adequate to prevent buffer overflow
	size_t bSize = 2048;

	char *buffer; 
	buffer = malloc(bSize * sizeof(char));
	if (!buffer) {
    		fprintf(stderr, "Buffer Memory Allocation Error\n");
    		return 0;
	}

	// fgets(*store data, max char to read, *file) until NULL
	if (fgets(buffer, bSize, file) != NULL) {

    		// use strncmp to compare two strings for shebang
    		// strncmp (string1 (line), string2 (shebang), # of character comparison) (0 if =)
    		if (strncmp(buffer, "#!", 2) == 0) {
        		// strstr finds first occurence of string specified in given string
        		// check for python (python3?), perl, ruby, or shell
        		if (strstr(buffer, "python")) {
            			free(buffer);
            			return 1;
        		}
        		if (strstr(buffer, "perl")) {
            			free(buffer);
            			return 2;
        		}
        		if (strstr(buffer, "ruby")) {
            			free(buffer);
            			return 3;
        		}	
        		if (strstr(buffer, "bash") || strstr(buffer, "sh") || strstr(buffer, "zsh") || strstr(buffer, "csh")) {
            			free(buffer);
            			return 4;
        		}		
    		}
	}
		// free pointer if no shebang found
		free(buffer);
		return 0;
}

void directory(const char *givenDirectory) {
	int cElf= 0; int cPython = 0; int cPerl= 0; int cRuby= 0; int cShell= 0; int cUnknown= 0;

	DIR *directory = opendir(givenDirectory);

	// struct dirent holds info like d_name and d_type
	struct dirent *directoryEntry;

	// check if directory exists
	if (!directory) {
    		fprintf(stderr, "binscan: No such directory %s\n",givenDirectory);
    		exit(1);
	}

	// loop through directory to get files until NULL
	while ((directoryEntry = readdir(directory)) != NULL) {
    		// check if file is DT_REG, meaning it's a regular file
    		if (directoryEntry->d_type == DT_REG) {

        		// file path length = strlen(directory itself + the file name) + the null terminator '/0'
        		size_t filePathLength = strlen(givenDirectory) + strlen(directoryEntry->d_name) + 2;

        		char *filePath;
        		filePath = malloc(filePathLength * sizeof(char));
        		if (!filePath) {
            			fprintf(stderr, "File Path Memory Allocation Error\n");
            			closedir(directory);
            			exit(1);
        		}

        		// snprintf(* store data, max # of characters, format, arg1, arg2)
			snprintf(filePath, filePathLength, "%s/%s",givenDirectory, directoryEntry->d_name);

        		// open file from determined filepath, cannot open = unknown + 1
        		FILE *file = fopen(filePath, "rb");
        		if (!file) {
            			cUnknown++;
            			free(filePath);
            			continue;
        		}

        		// check if it's an ELF file
        		if (elf(file)) {
            			cElf++;
        		}		 
			else {
				// rewind will go back to start of file if its not ELF executable
            			rewind(file);
				
				// get result from shebang
            			int result = shebang(file);
            			if (result == 1){cPython++;}
            			else if (result == 2){cPerl++;}
            			else if (result == 3){cRuby++;}
            			else if (result == 4){cShell++;}
            			else {cUnknown++;}
        		}
        		// close file and free pointer
        		fclose(file);
        		free(filePath);
    		}
	}
	// close directory
	closedir(directory);

	// print counts (%8d will right justify and pad with spaces)
	printf("%8d ELF executable\n", cElf);
	printf("%8d Perl script\n", cPerl);
	printf("%8d Shell script\n", cShell);
	printf("%8d Python script\n", cPython);
	printf("%8d Ruby script\n", cRuby);
	printf("%8d Unknown programs\n", cUnknown);
}


int main(int argc, char *argv[]) {
	// if not two arguments
	if (argc != 2) {
    		fprintf(stderr, "USAGE: %s <directory>\n", argv[0]);
    		exit(1);
	}
    	// if valid input, open directory and begin process
	directory(argv[1]);
	exit(0);
}


