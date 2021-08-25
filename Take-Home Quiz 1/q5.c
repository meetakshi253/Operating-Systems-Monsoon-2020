//works on linux 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>

#define shared_mem_size 8   //8 byte shared memory

pthread_mutex_t mutex_lock;
int count=0, flag=1;

key_t key;
int shmem_id;
char *memptr;

void *read_func(void *arg)
{
    for(int i=0;i<300;i++)
    {
        pthread_mutex_lock(&mutex_lock);
        char *readmem = (char*)shmat(shmem_id, (void *)0, 0);
        printf("Reader %d has read %s\n", arg, readmem);
        //sleep(1);
        pthread_mutex_unlock(&mutex_lock);  
        sleep(1);
    }
    pthread_exit(0);
}

void *write_func(void *arg)
{
    for(int i=0; i<100; i++)
    {
        pthread_mutex_lock(&mutex_lock);
        count++;
        char inp[8];
        sprintf(inp, "%d", count);
        strncpy(memptr, inp, shared_mem_size);
        printf("Writer has written %d\n", count);
        pthread_mutex_unlock(&mutex_lock);
        sleep(1);
    }
    pthread_exit(0);
    
}

int main()
{
    pthread_t read_thread[4], write_thread;
    if((key = ftok("q5.c", 55))==-1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    //create and connect to the memory segment
    if((shmem_id = shmget(key, shared_mem_size, 0644 | IPC_CREAT))== -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    //attach to the segment, get a pointer to it
    memptr = shmat(shmem_id, (void *)0, 0);
    if(memptr == (char *)(-1))
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    if(pthread_mutex_init(&mutex_lock, NULL) !=0)
    {
        perror("init");
        return 1;
    }

    pthread_create(&write_thread, NULL, &write_func, NULL);

    for(int i=0; i<4; i++)
    {
        int res = pthread_create(&(read_thread[i]), NULL, &read_func, (void *)(i+1));
    }

    pthread_join(write_thread, NULL);
    for(int i=0; i<4; i++)
    {
        pthread_join(read_thread[i], NULL);
    }

    if(shmdt(memptr) == -1)
    {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    shmctl(shmem_id, IPC_RMID, NULL);

    return 0;
}