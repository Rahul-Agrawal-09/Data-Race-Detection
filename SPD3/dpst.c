#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "dpst.h"

#define STEP 0
#define ASYNC 1
#define FINISH 2

uint64_t id=1;
generic_node* create_node(){
    generic_node*n=(generic_node*)malloc(sizeof(generic_node));
    n->node_id=id++;
    n->num_children=0;
    return n;
}

generic_node*create_step_node(generic_node*parent){
    generic_node*sn=create_node();
    sn->type=STEP;
    sn->parent=parent;
    sn->depth=(parent->depth)+1;
    parent->num_children++;
    sn->seq_no=parent->num_children;
    return sn;
}

generic_node* create_finish_node(generic_node*parent){
    generic_node*fn=create_node();
    fn->type=FINISH;
    fn->parent=parent;
    fn->depth=(parent->depth)+1;
    parent->num_children++;
    fn->seq_no=parent->num_children;
    return create_step_node(fn);
}

generic_node* create_async_node(generic_node*parent){
    generic_node*an=create_node();
    an->type=ASYNC;
    an->parent=parent;
    an->depth=(parent->depth)+1;
    parent->num_children++;
    an->seq_no=parent->num_children;
    return create_step_node(an);
}


//this function initialises the dpst 
generic_node* dpst_init(){
    generic_node*fn=create_node();
    fn->type=FINISH;
    fn->depth=0;
    fn->seq_no=1;
    fn->parent=NULL;
    ROOT=fn;
    return create_step_node(fn);
}

generic_node* dpst_add_async_node(generic_node*parent){
    return create_async_node(parent);
}

generic_node* dpst_add_step_node(generic_node*parent){
    return create_step_node(parent);
}

generic_node* dpst_add_finish_node(generic_node*parent){
    return create_finish_node(parent);
}

generic_node* dpst_LCA(generic_node*s1,generic_node*s2){
    while(s1->depth != s2->depth){
        if(s1->depth > s2->depth)
            s1=s1->parent;
        if(s1->depth < s2->depth)
            s2=s2->parent;
    }
    while (s1!=s2){
        s1=s1->parent;
        s2=s2->parent;
    }
    return s1;
}

generic_node* dpst_A_and_C(generic_node*step,generic_node*LCA){
    while(step->parent!=LCA){
        step=step->parent;
    }
    return step;
}

bool dpst_DMHP(generic_node*s1 , generic_node*s2){
    generic_node *lca=dpst_LCA(s1,s2);
    generic_node*A1=dpst_A_and_C(s1,lca);
    generic_node*A2=dpst_A_and_C(s2,lca);
    if(A1->seq_no < A2->seq_no){
        return (A1->type==ASYNC);
    }
    else{
        return (A2->type==ASYNC);
    }
}



// int main(int argc, char const *argv[])
// {
//     // finish_node*root=create_finish_node();
//     dpst_init();
//     printf("hello\n");
//     generic_node*root2=create_finish_node();
//     // two ways of printing uint64_t
//     printf("%ld\n", (long)((generic_node*)vector_get(root2->childs,0))->node_id); 
//     printf("%" PRIu64 "\n", (root2->node_id));
//     return 0;
// }
