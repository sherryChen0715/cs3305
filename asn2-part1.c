#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>




int x = 10, y= 20, z = 0; //initialize the variables

/**
 * print method to print thread msg
 * @return 0
 */
void *thread_prints_msg()
{
    z = x+y;
    printf("x+y is :%d",z);

    return 0;
}


/**
 * main method
 * @return
 */
int main ()
{
    pid_t pid;
    int fd[2];
    pthread_t thread;

    if (pipe(fd) < 0){ //pipe failed
        perror("pipe error");
        exit(0);
    }



    pid=fork();

    if (pid <0) // fork unsuccessful
    {
        printf("fork unsuccessful");
        exit(1);
    }

    if (pid>0) { //parent process
        wait(NULL);
        read (fd[0],&z,1); //read from the pipe
        printf("x+y is: %d\n",z);
        printf("From main: Going to create Thread...\n");
        pthread_create(&thread, NULL, thread_prints_msg, ""); //create thread
        pthread_join(thread, NULL);
        wait(NULL); //wait until thread finish
        printf("\nAll thread terminated...\n");
    }



    if (pid==0) { //child process
        z = x + y;
        write(fd[1],&z,4); //write to the pipe
    }



}
