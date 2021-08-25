#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stddef.h>

//writer process 1

struct msg_queue
{
    long msg_type;
    char msg_text[100];
} message;

key_t key;
int msqid;

void tokenize(char arr[])
{
    int i = 0;
    char *ptr = strtok(arr, " ");
    char *str;

    while(ptr!=NULL)
    {
        strcpy(message.msg_text, ptr);
        if(msgsnd(msqid, &message, sizeof(message), 0) == -1)
        {
            perror("msgsnd");
        }
        //printf(message.msg_text);
        ptr = strtok(NULL, " ");
    }
}

int main()
{
    key = ftok("q2c.c", 'b');
    if(key==-1)
    {
        perror("ftok");
        exit(1);
    }

    msqid  = msgget(key, 0644 | IPC_CREAT);
    if(msqid == -1)
    {
        perror("msgget");
        exit(1);
    }

    message.msg_type = 1;

    char buff[501]; //reads at most 500 characters at a time.
    FILE *fileptr;
    int f;

    fileptr = fopen("para1.txt", "r");
    if(fileptr == NULL)
    {
        perror("file");
        exit(1);
    }

    while(fgets(buff, 501, fileptr))
    {
        //printf("%s", buff);
        tokenize(buff);
    }


    fclose(fileptr);


    //strcpy(message.msg_text, "\n\n");
    //msgsnd(msqid, &message, sizeof(message), 0);
    
    return 0;
}
