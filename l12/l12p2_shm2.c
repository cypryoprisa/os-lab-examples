#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
    int shmFd;
    char *sharedChar = NULL;

    shmFd = shm_open("/l12_myshm", O_RDWR, 0);
    if(shmFd < 0) {
        perror("Could not aquire shm");
        return 1;
    }
    sharedChar = (char*)mmap(0, sizeof(char), PROT_READ | PROT_WRITE, 
        MAP_SHARED, shmFd, 0);
    if(sharedChar == (void*)-1) {
        perror("Could not map the shared memory");
        return 1;
    }

    printf("found sharedChar: %c\n", *sharedChar);
    *sharedChar = 'X';

    munmap(sharedChar, sizeof(char));
    sharedChar = NULL;

    return 0;
}
