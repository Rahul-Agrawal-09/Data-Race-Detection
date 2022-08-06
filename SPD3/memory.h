#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "dpst.h"

/*
This is shadow memory given as meta data to every 
dynamic memory alloocated.
w-> reference to node last wrote the memory
r1,r2-> reference to node last read the memory
*/
typedef struct shadow_memory{
    generic_node *w;
    generic_node *r1;
    generic_node *r2;
    pthread_mutex_t lock;
} shadow_mem;


#define sm_size sizeof(shadow_mem)

//custom malloc to allocate memory of size 
void* spd3_malloc(size_t size);

//custom function to write value to the memory location
void spd3_write_int(generic_node*step,int*ptr,int value);

//custom function to read a memory location
int spd3_read_int(generic_node*step,int*ptr);