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


int wait_no_block(struct my_semaphore* sem)
{
       //printf("%d\n", sem->count);
	int res = pthread_mutex_trylock(&(sem->lock));
	if (res !=0)
		return -1;

	if(sem->count<=0)
	{
		pthread_mutex_unlock(&(sem->lock));
		return -1;
	}
	sem->count = sem->count-1;
	pthread_mutex_unlock(&(sem->lock));
	return 0;

}

int signal_no_block(struct my_semaphore* sem)
{
    	//printf("jiii");
	int res = pthread_mutex_trylock(&(sem->lock));
	if (res !=0)
		return -1;
	sem->count = sem->count+1;
	pthread_mutex_unlock(&(sem->lock));
	return 0;
}

void* loopstart(void *p)
{
	int frk1,frk2,pno = *((int *)p);
    int wb1=0, wb2=0, wb3=0, sb1=0, sb2=0, sb3=0;
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
		wb1 = wait_no_block(&forks[frk1]);
        if(wb1!=0)
        {
            continue;
        }
		wb2 = wait_no_block(&forks[frk2]);
        if(wb2!=0)
        {
            do{
                sb1 = signal_no_block(&forks[frk1]);
            }
            while(sb1!=0);
            continue;
        }
		wb3 = wait_no_block(saucepans);
        {
            if(wb3!=0){
            do{
                sb1 = signal_no_block(&forks[frk1]);
            }
            while(sb1!=0);
            do{
                sb2 = signal_no_block(&forks[frk2]);
            }
            while(sb2!=0);

            continue;}
 
        }
		printf("Philosopher %d eats using forks %d and %d\n", pno+1, frk1, frk2);
        do{
            sb3 = signal_no_block(saucepans);

        }while(sb3!=0);
        do{
            sb2 = signal_no_block(&forks[frk2]);

        }while(sb2!=0);
        do{
            sb1 = signal_no_block(&forks[frk1]);

        }while(sb1!=0);

	}
}

void signal_print(struct my_semaphore* sem, int n)
{
    int res = pthread_mutex_trylock(&(sem->lock));
    if(res!=0)
    {
        return -1;
    }
    sem->count = sem->count+1;
    printf("n %d and count %d\n", n, sem->count);
    pthread_mutex_unlock(&(sem->lock));
        return 0;
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


