#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 64

int main()
{
    int fd1 = -1, fd2 = -1;
    ssize_t size = 0;
    int err = 0;
    unsigned char buff[BUFF_SIZE];

    fd1 = open("/usr/include/stdlib.h", O_RDONLY);
    if(fd1 == -1) {
        perror("Could not open input file");
        return 1;
    }
    fd2 = creat("copy_stdlib.h", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd2 == -1) {
        perror("Could not open output file");
        close(fd1);
        return 1;
    }

    while(1){
        size = read(fd1, buff, BUFF_SIZE);
        if(size < 0) {
            //read error
            perror("Could not read from input file");
            err = 1;
            break;
        } else if(size == 0) {
            //end of file
            break;
        } else {
            //size may be smaller than BUFF_SIZE
            size = write(fd2, buff, size);
            if(size <= 0){
                perror("Could not write to output file");
                err = 2;
                break;
            }
        }
    }

    close(fd1);
    close(fd2);

    if(err == 0) {
        printf("File copied successfully.\n");
    } else {
        printf("Some error occured while copying.\n");
    }

    return 0;
}