/*
 * Name: Darion Kwasnitza
 * ID: 3122890
 * File: m6.h
 */
#ifndef M6_H
#define M6_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <math.h>

typedef struct {
        int* matrix1;
        int* matrix2;
        int* result;
        int size;
        int start_row;
        int end_row;
} Data;

void* multiply(void*);

void map(const char*, int**, int*);

void output_to_file(const char *, int*, int);

#endif
