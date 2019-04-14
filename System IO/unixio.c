#include <ctype.h>
#include <fcntl.h> // open
#include <unistd.h> // read
#include <sys/types.h> // read
#include <sys/uio.h> // read
#include <stdio.h> // fopen, fread
#include <stdlib.h>
#include <string.h>
#include <sys/time.h> // gettimeofday

struct timeval start, end; // maintain starting and finishing wall time.

void startTimer( ) { // memorize the starting time
    printf("start");
    gettimeofday( &start, NULL );
}

void stopTimer( char *str ) { // checking the finishing time and computes the elapsed time
    gettimeofday( &end, NULL );
    printf("%s's elapsed time\t= %ld\n",str, ( end.tv_sec - start.tv_sec ) * 1000000 + (end.tv_usec - start.tv_usec ));
}

int main( int argc, char *argv[] ) {

    char filename[256];
    char * result;
    int typeofcalls;
    int readsize;
    int badArgument = 0;
    int readreturn = 0;
    // validate arguments
    if(argc != 4) {
        badArgument = 1;
    } else if(!strcmp(argv[3], "0") && !strcmp(argv[3], "1")) {
        badArgument = 1;
    } else {
        strncpy(filename, argv[1], 255);
        readsize = atoi(argv[2]);
        typeofcalls = atoi(argv[3]);
        if(readsize == 0) {
            badArgument = 1;
        }
    }
    if(badArgument == 1) {
        fprintf(stderr, "usage: %s filename bytes typeofcalls\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char * buffer = (char*)malloc(sizeof(char) * readsize);


    if (typeofcalls == 1) {
        // Uix I/O system calls
        printf("Using Unix I/O system calls to read a file by %d bytes per read\n", readsize);
        startTimer();
        int file = open(filename, O_RDONLY);
        if(file < 0) {
            perror("Error");
            exit(EXIT_FAILURE);
        }
        for(;;) {
            readreturn = read(file, buffer, readsize);
            if(readreturn != readsize) {
                close(file);
                break;
            }
        }
        stopTimer("Unix read");
    } else if (typeofcalls == 0) {
        // Standard I/O
        printf("Using C functions to read a file by %d bytes per fread\n", readsize);
        startTimer();
        FILE * fp = fopen(filename, "r");
        if(fp == NULL) {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        if(readsize == 1) {
            //fgetc to read one char at a time
            int c;
            for(;;) {
                c = fgetc(fp);
                if(feof(fp)) {
                    break;
                }
            }
            stopTimer("C fgetc");
        } else {
            //fread for > 1 at a time
            while(!feof(fp)) {
                fread(buffer, readsize, 1, fp);
            }
            stopTimer("C fread");
        }
    }

    free(buffer);
    exit(EXIT_SUCCESS);
}
