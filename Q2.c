#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#define MAX_ORDERS 10
#define MAX_CUSTOMERS 10
#define MAX_PIZZA_INGREDIANTS 10


int N,M,in,C,O,K;


sem_t drive_through;

typedef struct PizzaSt {
    int Num;
    int time;
    int NumIng;
    int* ingredients;
} Pizza;

typedef struct ChefSt {
    int Entry;
    int exit;
} Chef;


void *Car_Thread(void *arg){
    sem_wait(&drive_through);
    
}




int main(){
    
    scanf("%d%d%d%d%d%d",&N,&M,&in,&C,&O,&K);

    sem_init(&drive_through,0,K);

    Pizza Pizzas[M];
    for(int i=0;i<M;i++){
        scanf("%d%d%d",&Pizzas[i].Num,&Pizzas[i].time,&Pizzas[i].NumIng);
        Pizzas[i].ingredients = (int *) malloc(sizeof(int) * Pizzas[i].NumIng);
        for(int j=0;j<Pizzas[i].NumIng;j++){
            scanf("%d",&Pizzas[i].ingredients[j]);
        }
    }

    int Ing[in];
    for(int i=0;i<in;i++){
        scanf("%d",&Ing[in]);
    }

    Chef ChefArr[C];
    for(int i=0;i<C;i++){
        scanf("%d%d",&ChefArr[i].Entry,&ChefArr[i].exit);
    }

    int Ent_time=0;
    int Cust_Cnt=0;
    while (scanf("%d",&Ent_time)!=EOF)
    {
        printf("%d\n",Ent_time);
        int ordercnt=0;
        int Order[MAX_ORDERS];
        char c;
        while((scanf("%c",&c) != EOF )&& (c == ' ')){
            scanf("%d",&Order[ordercnt]);
            ordercnt++;
        }

        printf("\n");

        Cust_Cnt++;
    }

}