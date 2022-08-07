#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "spd3.h"

#define SIZE 8
int arr[SIZE];
int num_thread;
int chunk_size;
int *sum;

void *fun_async(void *arg_struct){
    fun_arg* arg=(fun_arg*)arg_struct;
    int start=atoi(arg->argv[0]);
    int end=atoi(arg->argv[1]);
    for (int i =start ; i < end; i++){
        int temp=spd3_read_int(arg->step,sum);
        spd3_write_int(arg->step,sum,(temp+arr[i]));
    }
    return NULL;
}

void fun_main(generic_node* step){
    sum=(int*)spd3_malloc(sizeof(int));
    for (size_t i = 0; i < SIZE; i++)
        arr[i]=(i+1);
    assert(SIZE%num_thread == 0);
    chunk_size=SIZE/num_thread;
    for (int worker = 0; worker < num_thread; worker++){
        char **argv;
        argv=(char**)malloc(sizeof(char*));
        argv[0]=(char*)malloc(sizeof(char)*10);
        argv[1]=(char*)malloc(sizeof(char)*10);
        int start=worker*chunk_size;
        int end=start+chunk_size;
        sprintf(argv[0], "%d", start);
        sprintf(argv[1], "%d", end);
        printf("\nSpawning %d thread using spd3_async() function\n",(worker+1));
        spd3_async(fun_async,step,argv);
    }
}

int main(int argc, char const *argv[])
{
    printf("----TEST PROGRAM STARTED----\n");
    if(argc!=2){
        printf("\n----ERROR: Invalid number of arguments-----\nPlease rerun program with correct syntax:\n");
        printf("\n\tInput formate: ./a.out num_thread(int)\n\n");
        return 0;
    }
    else{
        num_thread=atoi(argv[1]);
        if(num_thread==0){
            printf("\n----ERROR: Invalid argument-----\nPlease rerun program with valid argument:\n");
            printf("\n\tInput formate: ./a.out num_thread(int)\n\n");
            return 0;
        }
    }
    
    //this function calls the initialiation function using init
    spd3_launch(fun_main,num_thread);

    printf("\nFinal SUM is: %d\n",*sum);

    return 0;
}