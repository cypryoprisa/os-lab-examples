#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "l11_my_fifo"

int main(void)
{
    int fd = -1;
    int x = 0;

    //open, read and close fifo
    fd = open(FIFO_NAME, O_RDONLY);
    if(fd == -1) {
        perror("Could not open FIFO for reading");
        return 1;
    }
    read(fd, &x, sizeof(x));
    printf("The read value is: %d\n", x);
    close(fd);

    return 0;
}
