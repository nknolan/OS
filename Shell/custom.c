#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int cd (char ** args, int numArgs) {
    int dir = 0;
    if(numArgs != 2) {
        fprintf(stderr, "usage: cd [directory]\n");
        return dir;
    } else {
        dir = chdir(args[1]);
        if(dir != 0) {
            fprintf(stderr, "cd %s: No such file or directory\n", args[1]);
        }
    }
    return dir;
}

void datetime(int numArgs) {
    if(numArgs > 1) {
        fprintf(stderr, "usage: datetime (no arguments)\n");
        exit(EXIT_FAILURE);
    }
    const char * days[] = {"Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"};
    const char * months[] = {"January", "February", "March", "April", "May",
        "June", "July", "August", "September", "October", "November", "December"};
    const char * suffixes[] = {"", "st", "nd", "rd", "th"};
    time_t raw;
    struct tm * current;
    time(&raw);
    current = localtime(&raw);
    int ordinal = 0;
    char period[5] = "a.m.";
    int hour;

    if(current->tm_mday == 1 || current->tm_mday == 21 || current->tm_mday == 31) {
        ordinal = 1;
    } else if(current->tm_mday == 2 || current->tm_mday == 22) {
        ordinal = 2;
    } else if(current->tm_mday == 3 || current->tm_mday == 23) {
        ordinal = 3;
    } else {
        ordinal = 4;
    }

    if(current->tm_hour == 0) {
        hour = 12;
    } else if(current->tm_hour > 12) {
        hour = current->tm_hour % 12;
        strcpy(period, "p.m.");
    } else {
        hour = current->tm_hour;
    }

    printf("Today is %s, %s %d%s, %d\n", days[current->tm_wday], months[current->tm_mon],
        current->tm_mday, suffixes[ordinal], current->tm_year + 1900);
    printf("%02d:%02d %s\n", hour, current->tm_min, period);
    exit(EXIT_SUCCESS);
}

void gcd(char ** args, int numArgs) {
    int aType = 0;
    int bType = 0;
    if(numArgs != 3) {
        fprintf(stderr, "usage: %s number1 number2\n", args[0]);
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < strlen(args[1]); i++) {
        if(isxdigit(args[1][i]) && !isdigit(args[1][i])) {
            aType++;
        }
        if(!isxdigit(args[1][i]) && !isdigit(args[1][i])) {
            aType = -1;
            break;
        }
    }
    for(int i = 0; i < strlen(args[2]); i++) {
        if(isxdigit(args[2][i]) && !isdigit(args[2][i])) {
            bType++;
        }
        if(!isxdigit(args[2][i]) && !isdigit(args[2][i])) {
            bType = -1;
            break;
        }
    }
    if(aType == -1 || bType == -1) {
        fprintf(stderr, "usage: %s number1 number2\n", args[0]);
        exit(EXIT_FAILURE);
    }
    long a = 0;
    long b = 0;
    if(aType > 0) {
        a = strtol(args[1], NULL, 16);
    } else {
        a = strtol(args[1], NULL, 10);
    }
    if(bType > 0) {
        b = strtol(args[2], NULL, 16);
    } else {
        b = strtol(args[2], NULL, 10);
    }
    while(a != b) {
        if(a > b) {
            a = a - b;
        } else {
            b = b - a;
        }
    }
    printf("GCD(%s, %s) = %ld\n", args[1], args[2], a);
    exit(EXIT_SUCCESS);
}

void printArgs(char ** args, int numArgs) {
    printf("argc = %d, args = ", numArgs - 1);
    for(int i = 1; i < numArgs; i++) {
        printf("%s", args[i]);
        if(i != numArgs - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
    exit(EXIT_SUCCESS);
}

void signalReturn(int signal) {
    wait(NULL);
}
