#define _POSIX_SOURCE
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYSCALL_ERROR 2

void gen_signal_handler(int signum)
{
    write(STDOUT_FILENO, "GEN TERMINATED\n",strlen("GEN TERMINATED\n"));
    _exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){

    int pipefd[2];
    pid_t gen_pid, nsd_pid;

    // Create pipe
    if (pipe(pipefd) == -1) {
        return SYSCALL_ERROR;
    }

    // Create GEN process
    gen_pid = fork();
    if (gen_pid == -1) {
        return SYSCALL_ERROR;
    }

    // GEN main work
    if (gen_pid == 0) {
        // Cathching signal
        signal(SIGTERM, gen_signal_handler);

        // Redirect stdout to pipe
        if(dup2(pipefd[1],STDOUT_FILENO) == -1){
            return SYSCALL_ERROR;
        }
        
        //Close pipe for GEN
        if(close(pipefd[0]) == -1 || close(pipefd[1]) == -1){
            return SYSCALL_ERROR;
        }

        // GEN work
        while (1)
        {
            // Printed text redirected to pipe
            printf("%d %d\n", rand() % 4096, rand() % 4096);
            fflush(stdout);
            sleep(1);
        }
    }

    nsd_pid = fork();
    if (nsd_pid == -1) {
        return SYSCALL_ERROR;
    }

    if(nsd_pid == 0){

        // Get input from pipe
        if(dup2(pipefd[0],STDIN_FILENO) == -1){
            return SYSCALL_ERROR;
        }
        
        //Close pipe for NSD
        if(close(pipefd[0]) == -1 || close(pipefd[1]) == -1){
            return SYSCALL_ERROR;
        }
        
        char *nsd_name = "nsd";

        // Execute nsd
        if(execl(nsd_name,nsd_name,NULL,NULL) == -1){
            return SYSCALL_ERROR;
        }        
    }

    // Close pipe for MAIN
    if(close(pipefd[0]) == -1 || close(pipefd[1]) == -1){
        return SYSCALL_ERROR;
    }
    sleep(5);

    if(kill(gen_pid, SIGTERM) == -1){
        return SYSCALL_ERROR;
    }
    
    // Checking return value of children processes
    int gen_stat, nsd_stat;
    if(wait(&gen_stat) == -1 || wait(&nsd_stat) == -1){
        return SYSCALL_ERROR;
    }
    
    // Exit MAIN process
    if(gen_stat == EXIT_SUCCESS && nsd_stat == EXIT_SUCCESS){
        printf("OK\n");
        return EXIT_SUCCESS;
    }
    else{
        printf("ERROR\n");
        return EXIT_FAILURE;
    }
}