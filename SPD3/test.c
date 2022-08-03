#include <stdio.h>
#include <stdlib.h>
#include "spd3.h"

//NOTE: ALL THE SHARED VARIABLES SHOULD BE DECLARED DYNAMICALLY

void fun_async(generic_node* step){
    printf("This is fun_async. %ld\n",(long)step->node_id);
}

void fun_finish(generic_node* step){
    printf("This is fun_finish (before spawning async). %ld\n",(long)step->node_id);

    //init spawning async node
    spd3_async(fun_async,step);

    printf("This is fun_finish (after spawning async). %ld\n",(long)step->node_id);
}

void init(generic_node* step){
    printf("This is init (before spawning finish). %ld\n",(long)step->node_id);

    //init spawning async node
    spd3_finish(fun_finish,step);

    printf("This is init (after spawning finish). %ld\n",(long)step->node_id);
}

int main(int argc, char const *argv[])
{
    printf("TEST PROGRAM START\n");
    //this function calls the initialiation function using init
    spd3_launch(init);
    return 0;
}
