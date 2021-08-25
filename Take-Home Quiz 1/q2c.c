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

//server

struct msg_queue
{
    long msg_type;
    char msg_text[100];
} message;

key_t key;
int msqid;

int main()
{
    int count =0;
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

    while(1)
    {
        if(msgrcv(msqid, &message, sizeof(message), 1, 0)==-1)
        {
            perror("msgrcv");
            exit(1);
        }

        if(strcmp(message.msg_text, "\n\n")==0)
        {
            count++;
            if(count==2)
                break;
            continue;
        }

        printf("%s ", message.msg_text);
    }

    if(msgctl(msqid, IPC_RMID, NULL)==-1)
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
