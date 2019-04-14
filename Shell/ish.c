/* Nicholas Nolan
 * 0914780
 * February 2019
 * CIS*3110
 */
#define _XOPEN_SOURCE 500
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "custom.h"

int main() {
    bool isBackground = false;
    int isRead, isWrite = 0;
    const char fwdslash = '/';
    char ** args;
    char directory[4096];
    char * currentDirectory;
    char hostname[256];
	gethostname(hostname, 256);
	char * username = getenv("USER");
	int exitFlag = 0;
    int numArgs = 0;
    int status = 0;
    pid_t childPID;

    do {
		getcwd(directory, 4096);
        currentDirectory = strrchr(directory, fwdslash) + 1;
        printf("[%s@%s] %s%c ", username, hostname, currentDirectory, getuid() == 0 ? '#' : '$');
		args = getln();
        if(args[0] == NULL) {
            continue;
        }
        numArgs = 0;
        isRead = 0;
        isWrite = 0;
        isBackground = false;
        while(args[numArgs] != NULL) {
            if(!strcmp(args[numArgs], "<")) {
                isRead = numArgs;
            }
            if(!strcmp(args[numArgs], ">")) {
                isWrite = numArgs;
            }
            numArgs++;
        }
        if(!strcmp(args[numArgs - 1], "&")) {
            isBackground = true;
        }

		if(args[0] == NULL) {
			continue;//No input
		} else if(!strcmp(args[0], "exit")) {
			exitFlag = 1;
        } else if(!strcmp(args[0], "cd")) {
                cd(args, numArgs);
        } else {
            childPID = fork();

            if(!isBackground && childPID > 0) {
                //Parent process
                waitpid(childPID, &status, WUNTRACED);
                if(WIFEXITED(status) && WEXITSTATUS(status)) {
                    fprintf(stderr, "Child process exited abnormally\n");
                } else if(WIFSIGNALED(status)) {
                    fprintf(stderr, "Child process was killed\n");
                }
            } else if(isBackground && childPID > 0) {
                sigset(childPID, signalReturn);
            } else if(childPID == 0){
                //Child process
                if(isBackground) {
                    args[numArgs - 1] = NULL;
                }

                if(isRead) {
                    freopen(args[isRead + 1], "r", stdin);
                    args[isRead] = NULL;
                }
                if(isWrite) {
                    freopen(args[isWrite + 1], "w", stdout);
                    args[isWrite] = NULL;
                }

               if(!strcmp(args[0], "gcd")) {
                   gcd(args, numArgs);
               } else if(!strcmp(args[0], "args")) {
                   printArgs(args, numArgs);
               } else if(!strcmp(args[0], "datetime")) {
                   datetime(numArgs);
               } else if(execvp(args[0], args) < 0) {
                    fprintf(stderr, "%s: command not found\n", args[0]);
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            } else if (childPID < 0){
                fprintf(stderr, "Error launching child process\n");
            }
        }

    } while(!exitFlag);

	exit(0);
}
