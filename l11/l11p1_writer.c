#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "l11_my_fifo"

int main(void)
{
    int fd = -1;
    int x = 42;

    //create fifo
    if(mkfifo(FIFO_NAME, 0600) != 0) {
        perror("Could not create pipe");
        return 1;
    }

    //open, write and close fifo
    fd = open(FIFO_NAME, O_WRONLY);
    if(fd == -1) {
        perror("Could not open FIFO for writing");
        return 1;
    }
    printf("Writing value %d to FIFO\n", x);
    write(fd, &x, sizeof(x));
    close(fd);

    //delete fifo
    unlink(FIFO_NAME);

    return 0;
}
