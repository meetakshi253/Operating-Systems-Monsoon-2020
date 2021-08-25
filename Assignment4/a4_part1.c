#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct my_semaphore
{
	pthread_mutex_t lock;
	pthread_cond_t cond;
	int count;
};

struct my_semaphore *saucepans;
struct my_semaphore forks[5];
int phil_num = 5;

void initalise(struct my_semaphore* sem)
{
	sem->count =1;
	sem->cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	sem->lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	return;

}


void wait_block(struct my_semaphore* sem)
{
       //printf("%d\n", sem->count);
	pthread_mutex_lock(&(sem->lock));
	pthread_mutex_unlock(&(sem->lock));

	while(sem->count <= 0)
	{
		pthread_cond_wait(&(sem->cond), &(sem->lock));
	}
	sem->count = sem->count-1;
	pthread_mutex_unlock(&(sem->lock));

}

void signal_block(struct my_semaphore* sem)
{
    	//printf("jiii");
	pthread_mutex_lock(&(sem->lock));
	sem->count = sem->count+1;
	pthread_cond_signal(&(sem->cond));
	pthread_mutex_unlock(&(sem->lock));
}

void* loopstart(void *p)
{
    	//printf("inside");
	int frk1,frk2,pno = *((int *)p);
	if(pno == phil_num-1)
	{
		frk1 = 0;
		frk2 = pno;
		forks[frk2].count=1;
		forks[frk1].count=1;
		
	}
	else
	{
		frk1 = pno;
		frk2 = pno+1;
		forks[frk2].count=1;
		forks[frk1].count=1;
		
	}
	while(1)
	{
		wait_block(&forks[frk1]);
		wait_block(&forks[frk2]);
		wait_block(saucepans);
		printf("Philosopher %d eats using forks %d and %d\n", pno+1, frk1, frk2);
		signal_block(saucepans);
		signal_block(&forks[frk2]);
		signal_block(&forks[frk1]);
	}
}

void signal_print(struct my_semaphore* sem, int n)
{
    pthread_mutex_trylock(&(sem->lock));
    sem->count = sem->count+1;
    printf("n %d and count %d\n", n, sem->count);
    pthread_cond_signal(&(sem->cond));
    pthread_mutex_unlock(&(sem->lock));
}


int main()
{
scanf("%d", &phil_num);
pthread_t thread[phil_num];
int philosophers[phil_num];
for(int i=0;i<phil_num;i++)
	{
		philosophers[i] = i;
	}
for(int i=0;i<phil_num;i++)
	{
		initalise(&forks[phil_num]);
	
	}
	saucepans = malloc(sizeof(struct my_semaphore));
	initalise(saucepans);
		for(int i=0;i<phil_num;i++)
	{
		pthread_create(&thread[i], NULL, &loopstart, &philosophers[i]);
		//printf("here");
	}

	for(int i=0;i<phil_num;i++)
		pthread_join(thread[i], NULL);
	return 0;

}


