#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
  
sem_t mutex;
int last_index = 0;
int FCFS_flag = 1;

void* thread(void* arg)
{
    int index = *((int*)arg);
    sem_wait(&mutex);

    if (last_index > index)
        FCFS_flag = 0;

    last_index = index;
    printf("%d is executing\n", index);
    sleep(1);
      
    sem_post(&mutex);
}
  
int main()
{
    enum numbers;
    sem_init(&mutex, 0, 1);
    pthread_t t1,t2,t3,t4;
    int a = 1, b = 2, c = 3, d = 4;
    pthread_create(&t1,NULL,thread,&a);
    sleep(0.001);
    pthread_create(&t2,NULL,thread,&b);
    sleep(0.001);
    pthread_create(&t3,NULL,thread,&c);
    sleep(0.001);
    pthread_create(&t4,NULL,thread,&d);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);

    if (!FCFS_flag)
        printf("Not followed FCFS\n");
    else
        printf("FCFS followed\n");
    
    return 0;
}