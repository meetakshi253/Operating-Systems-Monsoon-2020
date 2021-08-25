#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int skt;
struct sockaddr_in server, client1, client2;

//server that prints data on the console

int main()
{
    int count = 0;
    skt = socket(AF_INET, SOCK_DGRAM, 0);       //socket file descriptor
    if(skt<0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    memset(&client1, 0, sizeof(client1));
    memset(&client2, 0, sizeof(client2));

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(skt, (const struct sockaddr *)&server, sizeof(server))<0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    int l1 = sizeof(client1);
    int l2 = sizeof(client2);

    while(1)
    {
        char word[50];
        int n = recvfrom(skt, (char *)word, sizeof(word), MSG_WAITALL, (struct sockaddr *) &client1, &l1);
        if(n<0)
        {
            perror("recvfrom");
        }
         if(strcmp(word, "\n\n")==0)
        {
            count++;
            if(count==2)
                break;
            continue;
        }
        printf("%s ",word);
        memset(word, 0, sizeof(word));
    }

    close(skt);
    return 0;
}


