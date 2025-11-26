#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

pid_t Fork(void) {

    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("Fork error");
        exit(1);
    }

    return pid;
}

int main() {
    pid_t pid;
    int x = 1;

    pid = Fork(); 

    if (pid == 0) { // branch condition for Child 
        
        printf("child : x=%d\n", ++x); 
        exit(0);
    }
    
    // Parent 
    printf("parent: x=%d\n", --x); 

    exit(0);
}
