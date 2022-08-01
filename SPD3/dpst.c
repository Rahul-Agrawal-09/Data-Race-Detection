#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "vector.h"
#include "dpst.h"

#define STEP 0
#define ASYNC 1
#define FINISH 2

uint64_t id=1;
generic_node* create_node(){
    generic_node*n=(generic_node*)malloc(sizeof(generic_node));
    n->node_id=id++;
    n->childs=NULL;
    return n;
}

generic_node*create_step_node(generic_node*parent){
    generic_node*sn=create_node();
    sn->type=STEP;
    sn->childs=NULL;
    sn->parent=parent;
    return sn;
}

generic_node* create_finish_node(generic_node*parent){
    generic_node*fn=create_node();
    fn->type=FINISH;
    fn->childs=vector_create();
    fn->parent=parent;
    vector_push(fn->childs,create_step_node(fn));
    return fn;
}

generic_node* create_async_node(generic_node*parent){
    generic_node*an=create_node();
    an->type=ASYNC;
    an->childs=vector_create();
    an->parent=parent;
    vector_push(an->childs,create_step_node(an));
    return an;
}


//this function initialises the dpst 
generic_node* dpst_init(){
    generic_node* root=create_finish_node(NULL);
    return root;
}

generic_node* dpst_add_async_node(generic_node*parent){
    generic_node* async=create_async_node(parent);
    vector_push(parent->childs,async);
    return (generic_node*)vector_get(async->childs,0);
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
