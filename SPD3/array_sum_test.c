#include <stdio.h>
#include <stdlib.h>
#include "spd3.h"
int num_thread;

void fun_main(generic_node* step){
    //write code here
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