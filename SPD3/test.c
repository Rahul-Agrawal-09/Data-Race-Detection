#include <stdio.h>
#include <stdlib.h>
#include "spd3.h"
int num_thread;

//NOTE: ALL THE SHARED VARIABLES SHOULD BE DECLARED DYNAMICALLY
int*i;

void *fun_async(void *arg_struct){
    fun_arg* arg=(fun_arg*)arg_struct;
    printf("This is fun_async. %ld  Level:%ld\n",(long)arg->step->node_id, (long)arg->step->depth);
    spd3_write_int(arg->step,i,123);
    printf("Value of i is :%d\n",*i);
}

void *fun_finish(void* arg_struct){
    fun_arg* arg=(fun_arg*)arg_struct;
    printf("This is fun_finish (before spawning async). %ld  Level:%ld\n",(long)arg->step->node_id, (long)arg->step->depth);

    //init spawning async node
    spd3_async(fun_async,arg->step,NULL);

    int k=spd3_read_int(arg->step,i);
    printf("Value of i is :%d\n",k);
    printf("This is fun_finish (after spawning async). %ld  Level:%ld\n",(long)arg->step->node_id, (long)arg->step->depth);
}

void fun_main(generic_node* step){
    i=(int*)spd3_malloc(sizeof(int));
    printf("This is init (before spawning finish). %ld  Level:%ld\n",(long)step->node_id, (long)step->depth);

    //init spawning async node
    spd3_finish(fun_finish,step, NULL);
    printf("This is init (after spawning finish). %ld  Level:%ld\n",(long)step->node_id, (long)step->depth);

    spd3_async(fun_async,step, NULL);
    printf("This is init (after spawning async). %ld  Level:%ld\n",(long)step->node_id, (long)step->depth);
}

int main(int argc, char const *argv[])
{
    printf("----TEST PROGRAM STARTED----\n");
    if(argc!=2){
        printf("\n----ERROR: Invalid number of arguments-----\nPlease rerun program with correct syntax:\n");
        printf("\n\tInput formate: ./dpst num_thread(int)\n\n");
        return 0;
    }
    else{
        num_thread=atoi(argv[1]);
        if(num_thread==0){
            printf("\n----ERROR: Invalid argument-----\nPlease rerun program with valid argument:\n");
            printf("\n\tInput formate: ./dpst num_thread(int)\n\n");
            return 0;
        }
    }
    
    //this function calls the initialiation function using init
    spd3_launch(fun_main,num_thread);
    return 0;
}
