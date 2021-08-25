
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main()
{
    //use two pipes and store two ends of each pipe.
    int pipe1[2], pipe2[2];
    pid_t pid;
    char str[501];

    fgets(str, 501, stdin);

    if(pipe(pipe1) == -1)
    {
        perror("pipe 1");
        exit(EXIT_FAILURE);
    }

    if(pipe(pipe2) == -1)
    {
        perror("pipe 2");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(pid == 0)
    {
        //child process
        char ch1[1];
        int i=0;
        close(pipe1[1]); //close the writing end of pipe 1
        close(pipe2[0]);

        for(int i=0; i<strlen(str); i++)
        {
            read(pipe1[0], ch1, 1); //read from the parent process  
            char ret = toupper(ch1[0]);
            write(pipe2[1], &ret, 1); //send the modified character to the parent process
        }
        
    }

    else
    {
        //parent process
        char ch1[1];

        close(pipe1[0]); //close the reading end of pipe1.

        for(int i=0; i<strlen(str); i++)
        {
            write(pipe1[1], &str[i], 1);
        }
        close(pipe1[1]); //close the writing end of pipe1

        int w = waitpid(pid, NULL, 0);

        close(pipe2[1]); //close the writing end of pipe2
        for(int i=0; i<strlen(str); i++)
        {
            read(pipe2[0], ch1, 1); //read from the child process
            printf("%c", ch1[0]);
        }
        
        close(pipe2[0]);  //close the reading end of pipe2
    }
    
    return 0;
}
