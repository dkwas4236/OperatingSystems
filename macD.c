/*
 * Darion Kwasnitza
 * 3122890
 * macD.c
 */

#include "macD.h"

/* initiate structure and variables*/
struct process *processes = NULL;
int num_p = 0, time_limit = 0, terminated = 0;
time_t start_time;

/* function to read given config file*/
void read_config(const char *config_file) {
	/* open the file and error check*/
	FILE *fp = fopen(config_file, "r");
	if (!fp) {
    		perror("error opening configuration file\n");
    		exit(EXIT_FAILURE);
	}

	/* read the time limit */
	fscanf(fp, "timelimit %d\n", &time_limit);

	/* dynamically allocate memory for processes and error check*/
	processes = malloc(MAX_PROCESSES * sizeof(struct process));
	if (!processes) {
    		perror("memory allocation failed");
    		exit(EXIT_FAILURE);
	}
	/* line will be dynamically allocated and reallocated*/
	char line[MAX_CMD_LEN];
	while (fgets(line, sizeof(line), fp)) {
    		/* remove newline */
    		line[strcspn(line, "\n")] = '\0';

    	 	/* dynamically allocate for commands and error check */
    		char *cmd = malloc(MAX_CMD_LEN * sizeof(char));
    		if (!cmd) {
        		perror("memory allocation error");
        		fclose(fp);
        		exit(EXIT_FAILURE);
    		}
		/* copy string to cmd and adjust null */
    		strncpy(cmd, line, MAX_CMD_LEN - 1);
    		cmd[MAX_CMD_LEN - 1] = '\0';

    		/* reallocation and error check */
    		processes = realloc(processes, (num_p + 1) * sizeof(struct process));
    		if (!processes) {
        		perror("memory allocation error");
        		free(cmd);
        		fclose(fp);
        		exit(EXIT_FAILURE);
    		}

    		/* initialize attributes of struct for each process */
    		processes[num_p].cmd = cmd;
    		processes[num_p].pid = -1;
    		processes[num_p].cpu_usage = 0.0;
    		processes[num_p].mem_usage = 0.0;
    		processes[num_p].status = 0;

		/* increment process number */
    		num_p++;
	}
	fclose(fp);
}

int launch() {
	/* initialize waitpid counter so process is recognized as not executing */
	int waitpid_counter = 0;

	for (int i = 0; i < num_p; i++) {
    		pid_t pid = fork();
		/* child */
    		if (pid == 0) {
        		/* execute the command and error check */
        		char *args[] = {"/bin/bash", "-c", processes[i].cmd, NULL};
        		if (execvp(args[0], args) == -1) {
            			perror("macD: execvp failed");
            			exit(EXIT_FAILURE); 
        		}	
    		}
		/* parent */
    		else if (pid > 0) {
			/* had to have small delay to give child process time to attempt execution */
        		sleep(1);
			waitpid_counter++;
        		/* waitpid with WNOHANG to check if the child process terminated after sleep */
        		int status;
        		if (waitpid(pid, &status, WNOHANG) > 0 && WIFEXITED(status)) {
            			printf("[%d] %s, failed to start\n", i, processes[i].cmd);
            			processes[i].pid = -1;
        		} else {
            			processes[i].pid = pid;
            			printf("[%d] %s, started successfully (pid: %d)\n", i, processes[i].cmd, pid);
        		}
    		}
		/* else the fork fails */
    		else {
        		printf("[%d] %s, fork error\n", i, processes[i].cmd);
    		}
	}
	/* return counter for later time adjustment */
	return waitpid_counter;
}

void check_usage(pid_t pid, int index) {
	char stat_file[256], statm_file[256];
	sprintf(stat_file, "/proc/%d/stat", pid);
	sprintf(statm_file, "/proc/%d/statm", pid);

	/* get cpu usage, open state file and get utime and stime */
	FILE *fp = fopen(stat_file, "r");
	if (fp) {
    		long unsigned utime, stime;
    		fscanf(fp, "%*d %*s %*c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %ld %ld", &utime, &stime);
    		fclose(fp);

    		/* calculate cpu usage */
    		time_t elapsed_time = time(NULL) - start_time;
    		if (elapsed_time > 0) {
			/* utime + stime / total time * 100 */
        		processes[index].cpu_usage = ((double)(utime + stime) / sysconf(_SC_CLK_TCK)) / elapsed_time * 100;
    		}
	}

	/* get memory usage with rss */
	fp = fopen(statm_file, "r");
	if (fp) {
    		long rss;
    		fscanf(fp, "%ld", &rss);
    		fclose(fp);

    		/* convert to MB */
    		processes[index].mem_usage = ((double)rss * sysconf(_SC_PAGESIZE)) / (1024 * 1024);
	}
}

void monitor() {
	/* set first flag for output */
	int f = 1;
	/* monitor time_limit */
	while (!terminated) {
    		time_t elapsed_time = time(NULL) - start_time;
    		if (elapsed_time >= time_limit) {
        		terminate();
        		break;
    		}

    		/* print information */
    		print_report(f);
		f = 0;
    		/* sleep 5 seconds */
    		sleep(5);
	}
}

void print_report(int first) {
	/* get time string now */
	time_t now = time(NULL);
	char time_str[100];
	strftime(time_str, sizeof(time_str), "%a %b %d %T %Y", localtime(&now));

	/* print time string */
	if ((first == 1)) {
		printf("Starting report, %s\n", time_str);
	} else {
		printf("Normal report, %s\n", time_str);
	}

	for (int i = 0; i < num_p; i++) {
    		if (processes[i].pid > 0) {
			/* 1 = exited, 2 = terminated, 0 = running */
        		check_usage(processes[i].pid, i);
        		if (processes[i].status == 1) {
            			printf("[%d] Exited\n", i);
        		} else if (processes[i].status == 2) {
            			printf("[%d] Terminated\n", i);
        		} else if (processes[i].status == 0) {
            			printf("[%d] Running, cpu usage: %.2f%%, mem usage: %.2f MB\n", i,
                   			processes[i].cpu_usage, processes[i].mem_usage);
        		}
    		} else {
			/* no pid, process has exited */
        		printf("[%d] Exited\n", i);
    		}
	}
}

void terminate() {
	/* get time string now */
        time_t now = time(NULL);
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%a %b %d %T %Y", localtime(&now));

	/* terminate all processes if signal given */
	for (int i = 0; i < num_p; i++) {
    		if (processes[i].pid > 0) {
        		kill(processes[i].pid, SIGTERM);
        		waitpid(processes[i].pid, NULL, 0);
        		printf("Terminating, %s\n", time_str);
			printf("[%d] Terminated\n", i);
    		} else {
        		printf("[%d] Exited\n", i);
    		}
	}

	/* set terminated flag */
	terminated = 1;
}

void handle_signal(int signal) {
	/* make sure signal is SIGINT */
	if (signal == SIGINT) {
    		printf("Signal Received - Terminating, %s\n", ctime(&start_time));
    		terminate();
    		exit(EXIT_SUCCESS);
	}
}

void cleanup() {
	/* free all processes */
	for (int i = 0; i < num_p; i++) {
    		free(processes[i].cmd);
	}
	free(processes);
}

int main(int argc, char *argv[]) {
	/* time adjustment for correction (int c) */
	int c;
	
	/* if argc != 3 print usage */
	if (argc != 3 || strcmp(argv[1], "-i") != 0) {
    		fprintf(stderr, "macD: Usage: ./macD -i <config_file>\n");
    		return EXIT_FAILURE;
	}

	/* handle SIGINT */
	signal(SIGINT, handle_signal);

	read_config(argv[2]);
	start_time = time(NULL);
	c = launch();
	monitor();
	cleanup();

	/* print total time */
	printf("Exiting (total time: %ld seconds)\n", (time(NULL) - start_time - c));
	return EXIT_SUCCESS;
}

