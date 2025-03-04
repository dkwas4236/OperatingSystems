/*
 * Darion Kwasnitza
 * 3122890
 * macD.h
 */

#ifndef MACD_H
#define MACD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define MAX_CMD_LEN 256
#define MAX_PROCESSES 100

/* struct stores process information */
struct process {
	pid_t pid;
	char *cmd;
	double cpu_usage;
	double mem_usage;
	/* 0 running, 1 exited, 2 terminated */
	int status;
};

void read_config(const char *config_file);
int launch();
void monitor();
void check_usage(pid_t pid, int index);
void print_report(int f);
void handle_signal(int signal);
void terminate();
void cleanup();

#endif


