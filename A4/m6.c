/*
 * Name: Darion Kwasnitza
 * ID: 3122890
 * File: m6.c
 */

#include "m6.h"

/* function to do matrix multiplication */
/* init at void* for threads */
void* multiply(void* arg) {

	Data *data = (Data *)arg;
	int size = data->size;

	/* i = r index m1; j = c index m2; k = c index m1*/
	for (int i = data->start_row; i < data->end_row; ++i) {
    		for (int j = 0; j < size; ++j) {
        		int sum = 0;
        		for (int k = 0; k < size; ++k) {
            			sum += data->matrix1[i * size + k] * data->matrix2[k * size + j];
        		}
			/* store the result in sum */
        		data->result[i * size + j] = sum;
    		}	
	}
	return NULL;
}

/* function to map matrix into memory */
void map(const char *filename, int **matrix, int *size) {
	/* open file for reading */
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
    		perror("Error: opening file");
    		exit(EXIT_FAILURE);
	}
	
	/* get the size of the file */
	struct stat st;
	if (fstat(fd, &st) != 0) {
    		perror("Error: file size");
    		close(fd);
    		exit(EXIT_FAILURE);
	}

	/* calculate the matrix dimensions, must be square! */
	*size = (int)sqrt(st.st_size / sizeof(int));

	/* if size * size * int != sqrt size then not square */
	/* -1 adjusted for padding */
	if ((*size) * (*size) * sizeof(int) != st.st_size-1) {
    		fprintf(stderr, "Error: not a square matrix\n");
    		close(fd);
    		exit(EXIT_FAILURE);
	}

	/* map the file to memory */
	/* addr, len, prot, flag, fildes, off */
	*matrix = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (*matrix == MAP_FAILED) {
    		perror("Error: mapping file failed");
    		close(fd);
    		exit(EXIT_FAILURE);
	}
	
	/* close file descriptor */
	close(fd);
}


void output_to_file(const char *filename, int *matrix, int size) {
	/* open the file to write */
	FILE *file = fopen(filename, "w");
	if (!file) {
		perror("Error: opening output file");
		exit(EXIT_FAILURE);
	}
	
	/* for rows and coloums, print as integers */
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			fprintf(file, "%d ", matrix[i* size + j]);
		}
		fprintf(file,"\n");
	}

	/* close the file */
	fclose(file);
}

int main(int argc, char *argv[]) {
	/* default of 1 thread */
	int n_threads = 1;

	/* default to no output file */
	const char *output_file = NULL;

	/* get cmd options using getopt */
	int opt; 
	while ((opt = getopt(argc, argv, "t:o:")) != -1) {
    		switch (opt) {
			/* if t, set number of threads */
        		case 't':
            			n_threads = atoi(optarg);
            			break;
			/* if o, set output file */
        		case 'o':
            			output_file = optarg;
            			break;
			/* if invalid option show usage message */
        		default:
            			fprintf(stderr, "Usage: %s matrix_file1 matrix_file2 [-t number_of_threads] [-o output_file]\n", argv[0]);
            			exit(EXIT_FAILURE);
    		}
	}
	
	/* if two inputs aren't provided, usage message */
	if (argc - optind < 2) {
    		fprintf(stderr, "Usage: %s matrix_file1 matrix_file2 [-t number_of_threads] [-o output_file]\n", argv[0]);
    		exit(EXIT_FAILURE);
	}
	
	/* get both files */
	const char *file1 = argv[optind]; 
	const char *file2 = argv[optind + 1];

	int *matrix1; 
	int *matrix2;
	int size1 = 0;
	int size2 = 0;
	
	/* map the input from both files */
	map(file1, &matrix1, &size1);
	map(file2, &matrix2, &size2);

	

	/* compare size */
	if (size1 != size2) {
    		fprintf(stderr, "Error: the arrays are not the same size\n");
    		exit(EXIT_FAILURE);
	}
 
	int size = size1;
	printf("Multiplying arrays of dimension %i\n", size);
	
	/* allocate memory for resulting matrix, calloc initializes memory to 0 and dynamically updates */
	int *result = calloc(size * size, sizeof(int));

	/* assign rows per number of threads equally */
	pthread_t threads[n_threads];
	Data t_data[n_threads];

	/* divide size by number of threads to get rows per thread*/
	int rows_per_thread = size / n_threads;

	/* init everything in struct for each thread */
	for (int i = 0; i < n_threads; ++i) {
    		t_data[i].matrix1 = matrix1;
    		t_data[i].matrix2 = matrix2;
    		t_data[i].result = result;
    		t_data[i].size = size;
    		t_data[i].start_row = i * rows_per_thread;
    		t_data[i].end_row = (i == n_threads - 1) ? size : (i + 1) * rows_per_thread;
		
		/* start thread */
    		pthread_create(&threads[i], NULL, multiply, &t_data[i]);
	}

	/* wait for threads to finish*/
	for (int i = 0; i < n_threads; ++i) {
    		pthread_join(threads[i], NULL);
	}
	
	/* if there is an output file, write to it */
	if (output_file) {
    		output_to_file(output_file, result, size);
	}
	
	/* clean up and unmap */
	free(result);
	munmap(matrix1, size * size * sizeof(int));
	munmap(matrix2, size * size * sizeof(int));

	return 0;
}


