#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#define FILEDOESNOTEXIST 10

void sig_usr(int signo){
    if (signo == FILEDOESNOTEXIST)
    printf("Error: \tCould not find source file\n\tFile or read Permissions missing");    
    return;
    
}

int main(int argc, char *argv[]){
    
    // validate the number of arguments
    if(argc < 3){
        printf("Usage: duplicate \"source file\" \"target file\"");
        return(2);
    }

    int pid;
    int ppid = getpid();
    pid = fork();
    
    // parent
    // the parent keeps track of the child and will inform the user that the 
    // operation is still in execution every second until the child exits
    if (pid != 0) {
        int *status;

        // catch the child signal if the file does not exist
        signal(FILEDOESNOTEXIST, sig_usr);

        // wait a second for the child
        sleep(1);
        
        // every secord check if the child has exited
        // if not inform the user the operation is pending completion
        while(!(waitpid(pid, status, WNOHANG))){
           printf("duplicate: still duplicating\n");
           sleep(1);
        }
    }

    // child 
    // performs the copy operation
    else {
        char *source = argv[1];
        char *target = argv[2];
        FILE *fpr;
        FILE *fpw;

        fpr = fopen(source, "r");
        fpw = fopen(target, "w");
        
        // wait for the parent to finish setting up
        usleep(1000);

        // signal the parent if the input file does not exist
        if (fpr == NULL){
            kill(ppid, FILEDOESNOTEXIST);
            return(1);
        }

        usleep(10000000);

        return(0);
    }
}