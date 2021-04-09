#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd;
    off_t size, crtPos, i;
    char c = 0;
    
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDWR);
    if(fd == -1) {
        perror("Could not open input file");
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    for(i=0; i<size; i++) {
        if(read(fd, &c, 1) != 1) {
            perror("Reading error");
            break;
        }
        if(c >= '0' && c <= '9') {
            crtPos = lseek(fd, 0, SEEK_CUR);
            lseek(fd, 0, SEEK_END);
            if(write(fd, &c, 1) != 1) {
                perror("Writing error");
                break;
            }
            lseek(fd, crtPos, SEEK_SET);
        }
    }

    close(fd);

    return 0;
}