#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

sem_t Machines; // * Semaphore to keep track of washing machine count
sem_t Creation; // * Used to create next thread only if the previous thread captured its value of A[i]

pthread_mutex_t UnhappyMtx;
pthread_mutex_t WastedMtx;


// ? input variables.
int N,M;
int *T,*W,*P;
int *A;

// ? Some global variables, used while printing.
time_t Start_time;
int Unhappy=0;
int WastedTime=0;



/**
 * Comparision function to sort students in the order of their arrival 
*/
int cmp(const void *a,const void *b){
    int n1 = *(int *) a;
    int n2 = *(int *) b;

    // FCFS
    if( T[n1] < T[n2]){
        return -1;
    }

    // If same time of arrival , order of lower index
    if( T[n1] == T[n2] && n1 < n2){
        return -1;
    }
    return 1;
}



/**
 * Thread simulating a single student.
 * @param arg contains student number
*/
void *Student_thread(void* arg){

    int Student = *((int *) arg) + 1;

    time_t start_time_local = time(0);
    printf("\033[37;1m%d: Student %d arrives\n\033[0m",(int) difftime(time(0),Start_time),Student);

    // ? Set sem_wait timeout to current time + Patience
    struct timespec timer;
    clock_gettime(CLOCK_REALTIME, &timer);
    timer.tv_sec += P[Student-1];

    sem_post(&Creation);
    // ? timed wait for maschines
    if(sem_timedwait(&Machines,&timer) == -1){

        int wasted_time = (int) difftime(time(0),start_time_local);
        
        printf("\033[31;1m%d: Student %d leaves without washing\n\033[0m",(int) difftime(time(0),Start_time),Student);
        
        
        pthread_mutex_lock(&WastedMtx);
        WastedTime +=  wasted_time;
        pthread_mutex_unlock(&WastedMtx);
        
        
        pthread_mutex_lock(&UnhappyMtx);
        Unhappy++;
        pthread_mutex_unlock(&UnhappyMtx);
    }else{
        int wasted_time = (int) difftime(time(0),start_time_local);
        
        printf("\033[32;1m%d: Student %d starts washing\n\033[0m",(int) difftime(time(0),Start_time),Student);
        
        // ? Wash
        sleep(W[Student-1]);
        
        // ? Leave Maschine
        printf("\033[33;1m%d: Student %d leaves after washing\n\033[0m",(int) difftime(time(0),Start_time),Student);
        sem_post(&Machines);
        
        
        pthread_mutex_lock(&WastedMtx);
        WastedTime +=  wasted_time;
        pthread_mutex_unlock(&WastedMtx);
    };
    return NULL;
}

int main(){
    
    scanf("%d%d",&N,&M);
    
    
    T = (int *) malloc(sizeof(int) * N);
    W = (int *) malloc(sizeof(int) * N);
    P = (int *) malloc(sizeof(int) * N);
    A = (int *) malloc(sizeof(int) * N);
    for(int i=0;i<N;i++){
        A[i]=i;
    }
    sem_init(&Machines,0,M);
    sem_init(&Creation,0,0);
    pthread_t Students[N];

    for(int i=0;i<N;i++){
        scanf("%d%d%d",&T[i],&W[i],&P[i]);
    }

    qsort(A,N,sizeof(A[0]),cmp);

    Start_time = time(0);
    int time=0;
    for (int i = 0; i < N; i++)
    {
        // * Sleep till next student comes
        if(T[A[i]] -time != 0){
            sleep(T[A[i]] - time);
        }
        pthread_create(&Students[i],NULL,Student_thread,(void *) &A[i]);
        time = T[A[i]];
        sem_wait(&Creation);
    }
    for(int i=0;i<N;i++){
        pthread_join(Students[i],NULL);
    }
    printf("\033[0m");
    if((Unhappy * 100)/N >= 25){
        printf("%d\n%d\nYES\n",Unhappy,WastedTime);
    }else{
        printf("%d\n%d\nNO\n",Unhappy,WastedTime);
    }
}