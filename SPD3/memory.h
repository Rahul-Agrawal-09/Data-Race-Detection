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

#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "dpst.h"


/**
 * This is shadow memory given as meta data to every dynamic memory alloocated.
 * Following are the component of the shadow memory (meta-data):
 *  > w = reference to step node which last wrote that memory location
 *  > r1, r2 = refereces to last two step node whihc read the memory location.
 *             these node are selected such that every step node that reads the
 *             the memory location lies within the sub-tree rooted at LCA(r1,r2)
 * Note: The above r1 and r2 condition is needed so as to ensure that if any write 
 * race with any other reads also race with one of r1 or r2 al well. Hence other 
 * reads can be negelected.
 *  > lock = this is the mutex lock needed to ensure that the memory location is 
 *           accessed by only one thread at a time. This prevents data-race in SPD3 program.
 * 
 * Note: Only Constant Space per shared memory location is required to track data race in SPD3.
 */
typedef struct shadow_memory{
    generic_node *w;
    generic_node *r1;
    generic_node *r2;
    pthread_mutex_t lock;
} shadow_mem;

//macro to save size of the shadow memory so as to add and remove metadata when needed
#define sm_size sizeof(shadow_mem)


/**
 * @brief 
 * spd3_malloc(size_t size) is a custom malloc function used for dynamic memory allocation.
 * The function adds metadata to the head of the memory location and returns the pointer 
 * excluding the meta data from the allocated memory. 
 * 
 * Every location allocated using spd3_malloc funciotn will be considered for data race detection.
 * with the constrain that read and writes are also performed by the custome spd3_write 
 * and spd3_read function.
 */
void* spd3_malloc(size_t size);


/**
 * @brief 
 * spd3_write_int is used to write integer to a memory location and w field of shadow memory
 * is updated is new access is different from old access.  
 * The arguments required are:
 *  > step = the step node which is requesting the write
 *  > ptr = pointer to the memory location been written
 *  > value = the value to be written
 * Note: This function can only be used to write an integer 
 * (we need to define similar functions for other data types).
 *  
 * To find data race following check are performed:
 *  > if previous write may happen in parallel with current write then there is a write-write data race.
 *  > if current write may happen in parallel with any of the reads then there is a read-write data race.
 *  
 * At last the value at the memory location is updated to new value. 
 */
void spd3_write_int(generic_node*step,int*ptr,int value);


/**
 * @brief 
 * spd3_read_int is used to read integer from a memory location and r1 ,r2 field of shadow memory
 * is updated is new access is different from old access.
 * The arguments required are:
 *  > step = the step node whihc is requesting the read
 *  > ptr = pointer to the memory location been read
 * Note: This funciton is used to read only int variables.
 * 
 * To find data race following check are performed:
 *  > if curent read and write of the shadow memory may happen in parallel then there is a read-write data race.
 * 
 * After check the r1 and r2 of the shadow memory is updated as follows:
 *  > if current read can not happen in parallel to r1 and r2 then old r1 and r2 is discarded and
 *    r1 is set to current read and r2 to null.
 *  > if current read may happen in parallel with r1 then it will also happen in parallel with r2 so we find 
 *    LCA(r1, current_step) and LCA(r2, current_step) and update the r1 to current_step if any of these LCAs 
 *    have less depth than LCA(r1,r2).
 * This is done to ensure that every read of that memory location is within the sub tree rooted at LCA(r1,r2).
 * At last the value of the memory locaiton is returned
 */
int spd3_read_int(generic_node*step,int*ptr);