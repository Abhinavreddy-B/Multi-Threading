#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

enum State {NOTCAME=0,WAITING=1,WASHING=2,DONE=3};

sem_t Machines;
pthread_mutex_t StudentCnt;

int Student_Num=1;
int N,M;
int *T,*W,*P;
enum State *Status;
time_t Start_time;

void *Student_thread(void* arg){

    pthread_mutex_lock(&StudentCnt);
    int Student = Student_Num;
    Student_Num++;
    pthread_mutex_unlock(&StudentCnt);


    sleep(T[Student-1]);
    printf("%d: Student %d arrives\n",(int) difftime(time(0),Start_time),Student);

    struct timespec timer;
    clock_gettime(CLOCK_REALTIME, &timer);
    timer.tv_sec += P[Student-1];
    if(sem_timedwait(&Machines,&timer) == -1){
        printf("%d: Student %d leaves without washing\n",(int) difftime(time(0),Start_time),Student);
    }else{
        printf("%d: Student %d starts washing\n",(int) difftime(time(0),Start_time),Student);
        sleep(W[Student-1]);
        printf("%d: Student %d leaves after washing\n",(int) difftime(time(0),Start_time),Student);
        sem_post(&Machines);
    };
    return NULL;
}

int main(){
    scanf("%d%d",&N,&M);
    T = (int *) malloc(sizeof(int) * N);
    W = (int *) malloc(sizeof(int) * N);
    P = (int *) malloc(sizeof(int) * N);
    for(int i=0;i<N;i++){
        scanf("%d%d%d",&T[i],&W[i],&P[i]);
    }
    sem_init(&Machines,0,M);
    pthread_t Students[N];
    Status = (enum State *) malloc(sizeof(enum State)*N);
    for(int i=0;i<N;i++){
        Status[i] = NOTCAME;
    }
    pthread_mutex_init(&StudentCnt,NULL);
    Start_time = time(0);
    for (int i = 0; i < N; i++)
    {
        pthread_create(&Students[i],NULL,Student_thread,NULL);
    }
    for(int i=0;i<N;i++){
        pthread_join(Students[i],NULL);
    }
}