#include <stdio.h>
#include <stdlib.h>
#include "spd3.h"

void fun_async(generic_node* step){
    printf("This is async. %ld\n",(long)step->node_id);
}

void init(generic_node* step){
    printf("This is init. %ld\n",(long)step->node_id);

    //init spawning async node
    spd3_async(fun_async,step);
}

int main(int argc, char const *argv[])
{
    printf("TEST PROGRAM START\n");
    //this function calls the initialiation function using init
    spd3_launch(init);
    return 0;
}
