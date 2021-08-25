//For Ubuntu
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

enum  state {eating=0, hungry=1, thinking=2};

//positions left and right
#define right (phil_no+1)%5
#define left (phil_no+4)%5

sem_t mutual_exclusion;     //binary semaphore for mutual exclusion
sem_t s[5];        //semaphore for synchronisation

enum state present_state[5];
int philosophers[5] = {0, 1, 2, 3, 4};

int eaten[5] = {0,0,0,0,0};

void test(int phil_no) //critical section
{
    if(present_state[phil_no] == hungry && present_state[right] != eating && present_state[left] != eating) //do not allow philosophers to eat at once to prevent deadlock
    {
        present_state[phil_no] = eating;
        sleep(1);

        printf("Philosopher %d picks up chopsticks %d and %d\n", phil_no+1, left+1, phil_no+1);
        printf("Philosopher %d is eating\n", phil_no+1);

        sem_post(&s[phil_no]);  //"activate" hungry philosophers in putback_chopsticks
    }
}

void take_chopsticks(int phil_no)
{
    sem_wait(&mutual_exclusion);        //critical section
    present_state[phil_no] = hungry;
    
    printf("Philosopher %d is hungry\n", phil_no+1);
    
    test(phil_no);
    sem_post(&mutual_exclusion);          //end critical section 

    sem_wait(&s[phil_no]);              
    sleep(1);

}

void putback_chopsticks(int phil_no)
{
    int sum = 0;
    sem_wait(&mutual_exclusion);            //critical section

    present_state[phil_no] = thinking; //goes to thinking state after putting down chopsticks
    printf("Philosopher %d has put chopsticks %d and %d down\n", phil_no+1, left+1, phil_no+1);
    printf("Philosopher %d is thinking\n", phil_no+1);

    eaten[phil_no] = 1;

    for(int i=0;i<5;i++)
    {
        sum+=eaten[i];
    }
    if(sum==5)
    {
        printf("All philosophers have eaten\n");
        exit(0);
    }

    
    test(left);          //let philosphers on left and right eat if possible
    test(right);

    sem_post(&mutual_exclusion);            //end critical section
}

void * philosopher(void* n)
{
    while(1)
    {
        int *ptr = n;
        sleep(1);
        take_chopsticks(*ptr);
        //sleep(0);
        putback_chopsticks(*ptr);
    }
}

int main()
{
    pthread_t thread[5];

    sem_init(&mutual_exclusion, 0, 1);

    for(int i=0; i<5; i++)
    {
        sem_init(&s[i], 0, 0);
    }

    for(int i=0; i<5; i++)
    {
        pthread_create(&thread[i], NULL, philosopher, &philosophers[i]);
        printf("Philosopher %d is thinking\n", i+1);
    }

    for(int i=0; i<5; i++)
    {
        pthread_join(thread[i], NULL);
    }
    return 0;
}

/**
* Avoid deadlocks by:
* not allowing all philosophers to eat at once
* picking up both chopsticks in the critical section (wait for both chopsticks to be available, that is)
**/
