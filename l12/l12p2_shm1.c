#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int shmFd;
    char *sharedChar = NULL;

    shmFd = shm_open("/l12_myshm", O_CREAT | O_RDWR, 0600);
    if(shmFd < 0) {
        perror("Could not aquire shm");
        return 1;
    }
    ftruncate(shmFd, sizeof(char));
    sharedChar = (char*)mmap(0, sizeof(char), PROT_READ | PROT_WRITE, 
        MAP_SHARED, shmFd, 0);
    if(sharedChar == (void*)-1) {
        perror("Could not map the shared memory");
        return 1;
    }

    *sharedChar = 'A';
    while(*sharedChar == 'A') {
        printf("sharedChar: %c\n", *sharedChar);
        sleep(1);
    }
    printf("sharedChar new value: %c\n", *sharedChar);

    munmap(sharedChar, sizeof(char));
    sharedChar = NULL;
    shm_unlink("/l12_myshm");

    return 0;
}
