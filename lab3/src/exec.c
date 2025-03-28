#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/wait.h>


int main() {

    pid_t pid = fork();


    if (pid < 0) {

        printf("fork failed");

        return 1;

    }


    if (pid == 0) {


        if (execl("./sequential_min_max", "sequential_min_max", \

            3, 50, NULL) == -1) {

            printf("execl failed");

            return 1;

        }

    }
    else {

        printf("Started sequential_min_max with PID: %d\n", pid);

        wait(NULL);

    }


    return 0;

}