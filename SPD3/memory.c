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

void* spd3_malloc(size_t size){
    //This is a custom malloc function used for dynamic memory allocation.
    //memory is allocated using malloc and size is increased by size of meta data
    void *ptr=(void*)malloc(sm_size+size);
    shadow_mem* sm=ptr;

    //initialization of fields of shadow memory
    sm->w=NULL;
    sm->r1=NULL;
    sm->r2=NULL;

    //mutex of the shadow memory is intialised and checked
    if (pthread_mutex_init(&(sm->lock), NULL) != 0) {
        printf("\n MUTEX INIT HAS FAILED at %p\n",(ptr+sm_size));
        return NULL;
    }

    //pointer to memory location excluding meta-data is returned 
    return (ptr+sm_size);
}

void spd3_write_int(generic_node*step,int*ptr, int value){
    //spd3_write_int is used to write integer to a memory location

    //shadow memory extracted from meta data
    void*v_ptr=(void*)ptr;
    shadow_mem* sm=v_ptr-sm_size;

    //mutex locked to avoid any data race at this memory location
    pthread_mutex_lock(&sm->lock);
    
    //read-write data race is detected if current write may happens in parallel 
    //with r1 (previous read) of shadow memory
    if(sm->r1!=NULL && sm->r1!=step && dpst_DMHP(sm->r1,step)){
        printf("\n!---READ-WRITE RACE---! at %p\n",ptr);
    }
    //read-write data race is detected if current write may happens in parallel 
    //with r2 (previous read) of shadow memory 
    if(sm->r2!=NULL && sm->r2!=step && dpst_DMHP(sm->r2,step)){
        printf("\n!---READ-WRITE RACE---! at %p\n",ptr);
    }
    //write-write data race is detected if current write may happens in parallel 
    //with w (previous write) of shadow memory 
    if(sm->w!=NULL && sm->w!=step && dpst_DMHP(sm->w,step)){
        printf("\n!---WRITE-WRITE RACE---! at %p\n",ptr);
    }

    //the write field of shadow memory and the value of the memory location is updated
    sm->w=step;
    *ptr=value;
    
    //mutex unlocked
    pthread_mutex_unlock(&sm->lock);
    return;
}

int spd3_read_int(generic_node*step,int*ptr){
    //This function is used to read integer from a memory location

    //shadow memory extracted from meta data
    void*v_ptr=(void*)ptr;
    shadow_mem* sm=v_ptr-sm_size;

    //mutex locked to avoid any data race at this memory location
    pthread_mutex_lock(&sm->lock);

    //read-write data race is detected if current read may happen in parallel with previous write
    if(sm->w!=NULL && sm->w!=step && dpst_DMHP(sm->w,step)){
        printf("\n!---READ-WRITE RACE---! at %p\n",ptr);
    }
    
    //r1 or r2 is set to current read if any of these field is not initialised 
    if(sm->r1==NULL){
        sm->r1=step;
    }
    else if (sm->r2==NULL){
        sm->r2=step;
    }

    //if we have both r1 and r2 then we need to choose two reads among r1, r2 & current_read
    //such that all the reads comes under the sub-tree if LCA(r1,r2);
    if(step!=sm->r1 && step!=sm->r2){
        bool dmhp1=dpst_DMHP(sm->r1,step);
        bool dmhp2=dpst_DMHP(sm->r2,step);

        //if r1 and r2 cannot happen in parallel with current_read then we will discard 
        //previous reads and initialise r1 to current_read and r2 to NULL
        if(!dmhp1 && !dmhp2){
            sm->r1=step;
            sm->r2=NULL;
        }
        //if r1 and r2 may happen in parallel with r1 and r2 then r1 is updated to 
        //current_node if any one of LCA(r1,current_read) or LCA(r2,current_read) have depth lower 
        //than LCA(r1,r2). i.e. choose r1 and r2 with highest (nearest to root) LCA in the tree.
        else if (dmhp1 && dmhp2){
            generic_node* lca12=dpst_LCA(sm->r1,sm->r2);
            generic_node* lca1s=dpst_LCA(sm->r1,step);
            generic_node* lca2s=dpst_LCA(sm->r2,step);
            if((lca1s->depth < lca12->depth) || (lca2s->depth < lca12->depth)){
                sm->r1=step;
            }
        }
    }

    //mutex unlocked
    pthread_mutex_unlock(&sm->lock);

    //pointer value is returned
    return *ptr;
}