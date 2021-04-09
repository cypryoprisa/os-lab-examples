#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd;
    off_t size;
    
    if(argc != 2){
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
        perror("Could not open input file");
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    printf("The file size is %ld.\n", size);

    close(fd);

    return 0;
}