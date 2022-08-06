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

/** 
 * This file contains the functions with is used by the user to interact with the SPD3 program.
 * To initialise the SPD3 program the user need to first call spd3_launch() function with the 
 * pointer to the main program (initial program). 
 */


void spd3_launch(void client_main(generic_node*), int num_thread){
    printf("\n===================================\n");
    printf("  SPD3 STARTED WITH NUM-THREAD: %d  ",num_thread);
    printf("\n===================================\n");

    //This function initialises the DPST of SPD3 program
    generic_node* root_step = dpst_init();

    //Passing step_id as parameter so as to use write and read method
    client_main(root_step);
    return;
}


void spd3_async(void client_async(generic_node*), generic_node*step_spawning){
    //This function spawns an async node and add it as child to the nearest ancestor 
    //of the spawning step (i.e. the parent of the spawning step)
    generic_node*parent=step_spawning->parent;
    generic_node*step_async = dpst_add_async_node(parent);

    //after adding the async node a new step node need to be added to the parent that represents the
    //computation whihc is after the new async section of code.
    *step_spawning=*dpst_add_step_node(parent);
    client_async(step_async);
    return;
}

void spd3_finish(void client_finish(generic_node*), generic_node*step_spawning){
    //This function spawns a finish node and add it as child to the nearest ancestor 
    //of the spawning step (i.e. the parent of the spawning step)
    generic_node*parent=step_spawning->parent;
    generic_node*step_finish=dpst_add_finish_node(parent);

    //after adding the finish node a new step node need to be added to the parent that represents the
    //computation which is after the new finish section of code.
    *step_spawning=*dpst_add_step_node(parent);
    client_finish(step_finish);
    return;
}

