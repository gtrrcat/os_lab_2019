#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <windows.h>
#include <sys/wait.h>

int main() {
    

    pid_t pid;

   
    if ((pid = fork()) == 0) {
        
        printf("Child process (PID: %d) has finished.\n", getpid());
        exit(0);  // Terminate the child process
    }
    else if (pid > 0) {
       
        printf("Parent process (PID: %d) is sleeping for 10 seconds...\n", getpid());
        sleep(5);  // Wait to make the child process a zombie
        printf("Parent process has finished.\n");
    }
    else {
        
        printf("Fork error");
        return 1;
    }

    return 0;
}