#include <stdio.h>
#include <stdlib.h>
#include "spd3.h"

//NOTE: ALL THE SHARED VARIABLES SHOULD BE DECLARED DYNAMICALLY
int*i;

void fun_async(generic_node* step){
    printf("This is fun_async. %ld  Level:%ld\n",(long)step->node_id, (long)step->depth);
    i=(int*)spd3_malloc(sizeof(int));
    spd3_write_int(step,i,123);
}

void fun_finish(generic_node* step){
    printf("This is fun_finish (before spawning async). %ld  Level:%ld\n",(long)step->node_id, (long)step->depth);

    //init spawning async node
    spd3_async(fun_async,step);

    spd3_write_int(step,i,23);
    // printf("%d\n",k);
    printf("This is fun_finish (after spawning async). %ld  Level:%ld\n",(long)step->node_id, (long)step->depth);
}

void init(generic_node* step){
    printf("This is init (before spawning finish). %ld  Level:%ld\n",(long)step->node_id, (long)step->depth);

    //init spawning async node
    spd3_finish(fun_finish,step);

    printf("This is init (after spawning finish). %ld  Level:%ld\n",(long)step->node_id, (long)step->depth);
}

int main(int argc, char const *argv[])
{
    printf("TEST PROGRAM START\n");
    //this function calls the initialiation function using init
    spd3_launch(init);
    return 0;
}
