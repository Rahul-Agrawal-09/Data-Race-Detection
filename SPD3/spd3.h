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

#include "memory.h"


typedef struct pthread_argument{
    generic_node *step;
    char **argv;
} fun_arg;


/**
 * @brief 
 * This functions acts as an entry point for the SPD3 program. The main function is called 
 * using this function which initialises the DPST and initializes the root finish node represent 
 * the implicit finish enclosing the main program. This is needed so as to ensure all the task 
 * are completed before the program terminates.
 */
void spd3_launch(void client_main(generic_node*), int num_thread);


/**
 * @brief 
 * 
 */
void spd3_async(void *client_async(void*), generic_node*step_spawning, char **argv);

/**
 * @brief 
 * 
 */
void spd3_finish(void *client_finish(void*), generic_node*step_spawning, char **argv);

