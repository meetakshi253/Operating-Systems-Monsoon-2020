#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int skt;
struct sockaddr_in server, client1;

void tokenize(char arr[])
{
    int i = 0;
    char *ptr = strtok(arr, " ");
    char *str;

    while(ptr!=NULL)
    {
        char test[50];
        strcpy(test, ptr);
        test[strlen(test)]='\0';
        //test[strlen(test)+1]='a';
	int k;

        k=sendto(skt, test, strlen(test), MSG_CONFIRM, (const struct sockaddr *)&server, sizeof(server));
	if(k<0)
        {
            perror("sendtoi");
        }
        //printf(message.msg_text);
        
        ptr = strtok(NULL, " ");
    }
    return;
}

int main()
{
    int count = 0;
    skt = socket(AF_INET, SOCK_DGRAM, 0);
    if(skt<0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

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

    /**if(sendto(skt, "\n\n", 2, MSG_CONFIRM, (const struct sockaddr *)&server, sizeof(server))==-1);
    {
        perror("sendto");
    }**/

    close(skt);
    return 0;

}
