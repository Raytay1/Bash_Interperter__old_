#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "cmdparse.h"


int main(){
    char inputB[101];
    char value[50];
    long lForkPid, lForkPid2;
    long lWaitPid;
    int iExitStatus = 0;
    int fdM[2];
    int iReadFd, iWriteFd;  // File Descriptors for reader and writer


    while(1){
        CMD com;
        printf("$ ");
        fgets(inputB, sizeof(inputB), stdin);
        sscanf(inputB, "%s", value);
        if(strcmp(value,"exit") == 0)
            exit(0);
        cmdparse(&inputB, &com);


        if(com.background){
            if(com.pipelining){//pipelining and backgrounding
                // create the pipe
                if (pipe(fdM) == -1){
                    fprintf( stderr, "pipe not created\n"); //prints to stderr
                    exit(1);
                }
                lForkPid = fork();    // create a child process
                // Both the parent and first child continue here
                switch(lForkPid) {
                    case -1:
                        fprintf( stderr, "fork failed\n"); //prints to stderr
                        exit(1);
                    case 0:  // child process
                        if ( dup2(fdM[1], STDOUT_FILENO) == -1){
                            fprintf( stderr, "Failed to redirect stdout for first command\n"); //prints to stderr
                            exit(1);
                        }
                        close(fdM[0]); close(fdM[1]);
                        execvp(com.argv1[0], com.argv1);
                        fprintf( stderr, "parse error\n"); //prints to stderr
                        exit(1);
                    default: // parent process
                        // create the other child
                        lForkPid2 = fork();    // create a child process
                        // Both the parent and second child continue here
                        switch(lForkPid2) {
                            case -1:
                                fprintf( stderr, "fork of second child failed\n"); //prints to stderr
                                exit(1);
                            case 0:  // child process - sort -k5 -n
                                if ( dup2(fdM[0], STDIN_FILENO) == -1) {
                                    fprintf(stderr, "Failed to redirect stdin for first command\n"); //prints to stderr
                                    exit(1);
                                }
                                close(fdM[0]); close(fdM[1]);
                                execvp(com.argv2[0], com.argv2);
                                fprintf( stderr, "parse error\n"); //prints to stderr
                                exit(1);
                            default:  // parent process
                                close(fdM[0]); close(fdM[1]);
                        }
                }

            }else{//no pipelining with backgrounding
                // creates a child process
                lForkPid = fork();
                // Both the parent and child continue here
                switch(lForkPid){
                    case -1://fork failed to execute
                        fprintf( stderr, "fork failed\n"); //prints to stderr
                        exit(1);
                    case 0:  // child process
                        execvp(com.argv1[0], com.argv1);
                        fprintf( stderr, "parse error\n"); //prints to stderr
                        exit(1); //execvp failed if it got to this point
                    default: // parent process
                        break;
                }//end of switch statement
            }

        }else{//for non background commands
            if(com.pipelining){//pipelining with no backgrounding

                // create the pipe
                if (pipe(fdM) == -1){
                    fprintf( stderr, "pipe not created\n"); //prints to stderr
                    exit(1);
                }
                lForkPid = fork();    // create a child process
                // Both the parent and first child continue here
                switch(lForkPid) {
                    case -1:
                        fprintf( stderr, "fork failed\n"); //prints to stderr
                        exit(1);
                    case 0:  // child process
                        if ( dup2(fdM[1], STDOUT_FILENO) == -1){
                            fprintf( stderr, "Failed to redirect stdout for first command\n"); //prints to stderr
                            exit(1);
                        }
                        close(fdM[0]); close(fdM[1]);
                        execvp(com.argv1[0], com.argv1);
                        fprintf( stderr, "parse error\n"); //prints to stderr
                        exit(1);
                    default: // parent process
                        // create the other child
                        lForkPid2 = fork();    // create a child process
                        // Both the parent and second child continue here
                        switch(lForkPid2) {
                            case -1:
                                fprintf( stderr, "fork of second child failed\n"); //prints to stderr
                                exit(1);
                            case 0:  // child process - sort -k5 -n
                                if ( dup2(fdM[0], STDIN_FILENO) == -1) {
                                    fprintf(stderr, "Failed to redirect stdin for first command\n"); //prints to stderr
                                    exit(1);
                                }
                                close(fdM[0]); close(fdM[1]);
                                execvp(com.argv2[0], com.argv2);
                                fprintf( stderr, "parse error\n"); //prints to stderr
                                exit(1);
                            default:  // parent process
                                lWaitPid = wait (&iExitStatus);
                                if (lWaitPid == -1){
                                    fprintf( stderr, "wait error\n"); //prints to stderr
                                    exit(1);
                                }
                                close(fdM[0]); close(fdM[1]);
                        }
                }
            }else{//no pipelining and no backgrounding
                // creates a child process
                lForkPid = fork();
                // Both the parent and child continue here
                switch(lForkPid){
                    case -1://fork failed to execute
                        fprintf( stderr, "fork failed\n"); //prints to stderr
                        exit(1);
                    case 0:  // child process
                        execvp(com.argv1[0], com.argv1);
                        fprintf( stderr, "parse error\n"); //prints to stderr
                        exit(1); //execvp failed if it got to this point
                    default: // parent process
                        lWaitPid = wait (&iExitStatus);
                        if (lWaitPid == -1){
                            fprintf( stderr, "wait error\n"); //prints to stderr
                            exit(1);
                        }
                        continue;
                }//end of switch statement
            }//end of else no pipelining statement

        }//end of else no background statement


    }//end of while loop
}

