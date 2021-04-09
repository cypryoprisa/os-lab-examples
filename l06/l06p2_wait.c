#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid1=-1, pid2=-1, pid3=-1;
    int x=5, y=3, z=0;
    int status1=0, status2=0;
    srand(time(NULL));

    pid1 = fork();
    if(pid1 == 0) {
        //1st child
        
        printf("%ld\n", random() % 500000);
        z = x + y;
        exit(z);
    }
    pid2 = fork();
    if(pid2 == 0) {
        //2nd child
        printf("%ld\n", random() % 500000);
        z = x - y;
        exit(z);
    }
    pid3 = fork();
    if(pid3 == 0) {
        //3rd child
        printf("%ld\n", random() % 500000);
        z = x * y;
        exit(z);
    }
    waitpid(pid2, &status1, 0);
    printf("The result of the subtraction is %d.\n", WEXITSTATUS(status1));
    wait(&status1);
    wait(&status2);
    printf("The result of other two operations are %d and %d.\n", 
        WEXITSTATUS(status1), WEXITSTATUS(status2));

    return 0;
}
