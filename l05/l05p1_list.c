#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int listDir(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        perror("Could not open directory");
        return -1;
    }
    while((entry = readdir(dir)) != NULL) {
        snprintf(filePath, 512, "%s/%s", path, entry->d_name);
        if(lstat(filePath, &statbuf) == 0) {
            if(S_ISREG(statbuf.st_mode)) {
                printf("[reg] ");
            } else if(S_ISDIR(statbuf.st_mode)) {
                printf("[dir] ");
            } else if(S_ISLNK(statbuf.st_mode)) {
                printf("[lnk] ");
            } else {
                printf("[unk] ");
            }
            printf("[%o] ", statbuf.st_mode & 0777);
        } else {
            printf("[err] ");
        }
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return 0;
}

int main(int argc, char **argv)
{
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <dir_name>\n", argv[0]);
        return 1;
    }
    if(listDir(argv[1]) == 0) {
        printf("The directory was listed successfully.\n");
    } else {
        printf("The were some erorrs listing the directory.\n");
    }
    return 0;
}
