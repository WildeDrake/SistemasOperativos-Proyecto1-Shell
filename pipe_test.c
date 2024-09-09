#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void test1() {
	int fd1[2], fd2[2];
	pipe(fd1);
	pipe(fd2);

	if (fork() == 0) {
		dup2(fd1[1], STDOUT_FILENO);
		char *args[] = {"ps", "-aux",
							 NULL}; // "|", "sort", "-nr", "-k", "4", "|" ,"head", "-20", NULL};
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		execvp(args[0], args);
	} else if (fork() == 0) {
		dup2(fd1[0], STDIN_FILENO);
		dup2(fd2[1], STDOUT_FILENO);
		char *args[] = {"sort", "-nr", "-k", "4", NULL};
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		execvp(args[0], args);
	} else {
		dup2(fd2[0], STDIN_FILENO);
		char *args[] = {"head", "-20", NULL};
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		execvp(args[0], args);
	}
}

void test2() {

	int n = 2;

	char *C[][5] = {{"ps", "-aux", NULL}, {"sort", "-nr", "-k", "4", NULL}, {"head", "-20", NULL}};

	int FDs[2 * n];
	for (int i = 0; i <= n; i++) {
		if (i < n) {
			pipe(FDs + 2 * i);
		}
		if (fork() == 0) {
			if (i > 0) {
				dup2(FDs[2 * i - 2], STDIN_FILENO);
				close(FDs[2 * i - 2]);
				close(FDs[2 * i - 1]);
			}
			if (i < n) {
				dup2(FDs[2 * i + 1], STDOUT_FILENO);
				close(FDs[2 * i]);
				close(FDs[2 * i + 1]);
			}
			execvp(C[i][0], C[i]);
		}
		if (i > 0) {
			close(FDs[2 * i - 2]);
			close(FDs[2 * i - 1]);
		}
	}
}

int main() {

	test2();

	return 0;
}