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

/**
 * struct pthread_argument (fun_argv) is the structure used to pass argument to the async/finish 
 * function. Following are the two components of the structure:
 *  > step  = reference to the step node spawning the async/finish node
 *  > argv  = remaining argument to the async/finish function 
 *            (argument are passed in the same way we take command line arguments for a C program)
 */
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
 * This function is used as an API for async section of the code. The function takes 3 argument 
 *  > client_async = this is the pointer to the async function to be execution on the new thread
 *  > step_spawning = this is the pointer to the step node which spawned (genreated) the async node
 *  > argv = this is the list of all the arguments which need to be passed to the funcion apart from
 *           step_spawning node. This argument is passed in the same way we take command line argument 
 *           in a C program.
 * 
 * The arguments step_spawning and argv are passed to the client_async function as a structure called 
 * fun_arg.
 * At last new thread is formed using pthread library with client_async function and fun_arg as argument.
 */
void spd3_async(void *client_async(void*), generic_node*step_spawning, char **argv);

/**
 * @brief 
 * This function is used as an API for finish section of the code. The function takes 3 argument 
 *  > client_finish = this is the pointer to the finish function to be execution
 *  > step_spawning = this is the pointer to the step node which spawned (genreated) the finish node
 *  > argv = this is the list of all the arguments which need to be passed to the funcion apart from
 *           step_spawning node. This argument is passed in the same way we take command line argument 
 *           in a C program.
 * 
 * The arguments step_spawning and argv are passed to the client_finish function as a structure called 
 * fun_arg and then the client_finish function is called with fun_arg as the argument.
 */
void spd3_finish(void *client_finish(void*), generic_node*step_spawning, char **argv);

