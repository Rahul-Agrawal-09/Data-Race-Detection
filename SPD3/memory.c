#include "memory.h"

void* spd3_malloc(size_t size){
    void *ptr=(void*)malloc(sm_size+size);
    shadow_mem* sm=ptr;
    sm->w=NULL;
    sm->r1=NULL;
    sm->r2=NULL;
    return (ptr+sm_size);
}


void spd3_write_int(generic_node*step,int*ptr, int value){
    void*v_ptr=(void*)ptr;
    shadow_mem* sm=v_ptr-sm_size;
    if(sm->r1!=NULL && sm->r1!=step && dpst_DMHP(sm->r1,step)){
        printf("!---READ-WRITE RACE---! with %ld\n",(long)sm->r1->node_id);
    }
    if(sm->r2!=NULL && sm->r2!=step && dpst_DMHP(sm->r2,step)){
        printf("!---READ-WRITE RACE---! with %ld\n",(long)sm->r2->node_id);
    }
    if(sm->w!=NULL && sm->w!=step && dpst_DMHP(sm->w,step)){
        printf("!---WRITE-WRITE RACE---! with %ld\n",(long)sm->w->node_id);
    }
    sm->w=step;
    *ptr=value;
}

int spd3_read_int(generic_node*step,int*ptr){
    void*v_ptr=(void*)ptr;
    shadow_mem* sm=v_ptr-sm_size;
    if(sm->w!=NULL && sm->w!=step && dpst_DMHP(sm->w,step)){
        printf("!---READ-WRITE RACE---! with %ld\n",(long)sm->r2->node_id);
    }
    if(sm->r1==NULL){
        sm->r1=step;
    }
    else if (sm->r2==NULL){
        sm->r2=step;
    }
    if(step!=sm->r1 && step!=sm->r2){
        bool dmhp1=dpst_DMHP(sm->r1,step);
        bool dmhp2=dpst_DMHP(sm->r2,step);
        if(!dmhp1 && !dmhp2){
            sm->r1=step;
            sm->r2=NULL;
        }
        else if (dmhp1 && dmhp2){
            generic_node* lca12=dpst_LCA(sm->r1,sm->r2);
            generic_node* lca1s=dpst_LCA(sm->r1,step);
            generic_node* lca2s=dpst_LCA(sm->r2,step);
            if((lca1s->depth < lca12->depth) || (lca2s->depth < lca12->depth)){
                sm->r1=step;
            }
        }
    }
    return 5;
}