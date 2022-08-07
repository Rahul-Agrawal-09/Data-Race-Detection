/**
 * Program: SPD3 (dynamic data race detector in async/finish parallel program)
 * Author: Rahul Agrawal
 * Mentor: Vivek Kumar
 * 
 * Referece: https://files.sri.inf.ethz.ch/website/papers/pldi12-spd3.pdf
 * 
 * The Program detect data race for dynamic memory location in async/finish parallel program
 *  > async   = spawns new task which run asynchronous with other tasks
 *  > finish  = blocks the execution till all the task spawned within finish terminates
 */
#include"spd3.h"
#include <pthread.h>
pthread_t *THREAD_ID;
int THREAD_INDEX=0;
/** 
 * This file contains the functions with is used by the user to interact with the SPD3 program.
 * To initialise the SPD3 program the user need to first call spd3_launch() function with the 
 * pointer to the main program (initial program). 
 */


void spd3_launch(void client_main(generic_node*), int const num_thread){
    printf("\n===================================\n");
    printf("  SPD3 STARTED WITH NUM-THREAD: %d  ",num_thread);
    printf("\n===================================\n");

    //This function initialises the DPST of SPD3 program 
    //and the array of thread id according to num thread argument provided by the user
    generic_node* root_step = dpst_init();
    THREAD_ID=(pthread_t*)malloc(sizeof(pthread_t)*num_thread);

    //Passing step_id as parameter so as to use write and read method
    client_main(root_step);

    for(int i=0;i<THREAD_INDEX;i++)
        pthread_join(THREAD_ID[i],NULL);
    return;
}

void spd3_async(void *client_async(void*), generic_node*step_spawning, char **argv){
    //This function spawns an async node and add it as child to the nearest ancestor 
    //of the spawning step (i.e. the parent of the spawning step)
    generic_node*parent=step_spawning->parent;
    generic_node*step_async = dpst_add_async_node(parent);

    //preparing the argument structure to pass arguments to the client_async function
    fun_arg *arg=(fun_arg*)malloc(sizeof(fun_arg));
    arg->step=(generic_node*)malloc(sizeof(generic_node));
    *arg->step=*step_async;
    arg->argv=argv;

    //after adding the async node a new step node need to be added to the parent that represents the
    //computation whihc is after the new async section of code.
    *step_spawning=*dpst_add_step_node(parent);

    //new thread is created and thread id stored in in an array to join in the implicit finish section
    pthread_create(&THREAD_ID[THREAD_INDEX++],NULL,client_async,(void*)arg);
    // pthread_join(THREAD_ID[THREAD_INDEX-1],NULL);    /*----uncomment to run the program in sequencial mode----*/
    
    return;
}

void spd3_finish(void *client_finish(void*), generic_node*step_spawning, char **argv){
    //This function spawns a finish node and add it as child to the nearest ancestor 
    //of the spawning step (i.e. the parent of the spawning step)
    generic_node*parent=step_spawning->parent;
    generic_node*step_finish=dpst_add_finish_node(parent);

    //preparing the argument structure to pass arguments to the client_finish function
    fun_arg *arg=(fun_arg*)malloc(sizeof(fun_arg));
    arg->step=(generic_node*)malloc(sizeof(generic_node));
    *arg->step=*step_finish;
    arg->argv=argv;

    //after adding the finish node a new step node need to be added to the parent that represents the
    //computation which is after the new finish section of code.
    *step_spawning=*dpst_add_step_node(parent);

    //no new thread creation in finish as it do not create a new task
    client_finish(arg);
    
    return;
}

