#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    int fdP2C[2], fdC2P[2];
    char c = 0;
    int i;

    if(pipe(fdP2C) != 0 || pipe(fdC2P)) {
        perror("Could not create pipes");
        return 1;
    }

    if(fork() != 0) {
        //parent
        close(fdP2C[0]);
        close(fdC2P[1]);
        c = 'a';
        for(i=0; i<10; i++) {
            write(fdP2C[1], &c, sizeof(c));
            read(fdC2P[0], &c, sizeof(c));
            printf("Parent: %c\n", c);
            c++;
        }
        close(fdP2C[1]);
        close(fdC2P[0]);
        wait(NULL);
    } else {
        //child
        close(fdP2C[1]);
        close(fdC2P[0]);
        for(i=0; i<10; i++) {
            read(fdP2C[0], &c, sizeof(c));
            printf("Child: %c\n", c);
            c++;
            write(fdC2P[1], &c, sizeof(c));
        }
        close(fdP2C[0]);
        close(fdC2P[1]);
    }

    return 0;
}
