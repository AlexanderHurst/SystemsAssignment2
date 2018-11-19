#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#define READFILEACCESSERR 10
#define WRITEFILEACCESSERR 11

void sig_usr(int signo){
    if (signo == READFILEACCESSERR)
    printf("Error: \tCould not read source file\n\tFile or read Permissions missing");
    if (signo == WRITEFILEACCESSERR)
    printf("Error: \tCould not write to target file\n\tCheck File Permissions");    
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

        // catch the child signals if there are complications
        signal(READFILEACCESSERR, sig_usr);
        signal(WRITEFILEACCESSERR, sig_usr);
        
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
        FILE *fpr, *fpw;
        fpr = fopen(source, "r");
        fpw = fopen(target, "w");
        
        // wait for the parent to finish setting up
        usleep(1000);

        // signal the parent if the input file does not exist
        // or permission is denied
        if (fpr == NULL){
            kill(ppid, READFILEACCESSERR);
            return(1);
        }

        // signal the parent if write access denied
        if (fpw == NULL){
            kill(ppid, WRITEFILEACCESSERR);
            return(1);
        }

        // grab each character and write it to the target file
        char c = fgetc(fpr);

        while(c != EOF){
            fputc(c, fpw);
            c = fgetc(fpr);
        }

        fclose(fpw);
        fclose(fpr);
        
        //usleep(2000000);

        return(0);
    }
}