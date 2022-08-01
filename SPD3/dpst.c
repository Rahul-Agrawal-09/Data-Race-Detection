#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "vector.h"
#include "dpst.h"

#define STEP 0
#define ASYNC 1
#define FINISH 2
generic_node*root;

uint64_t id=1;
generic_node* create_node(){
    generic_node*n=(generic_node*)malloc(sizeof(generic_node));
    n->node_id=id++;
    n->childs=NULL;
    return n;
}

generic_node*create_step_node(){
    generic_node*sn=create_node();
    sn->type=STEP;
    sn->childs=vector_create();
    return sn;
}

generic_node* create_finish_node(){
    generic_node*fn=create_node();
    fn->type=FINISH;
    fn->childs=vector_create();
    vector_push(fn->childs,create_step_node());
    return fn;
}

generic_node* create_async_node(){
    generic_node*an=create_node();
    an->type=ASYNC;
    an->childs=vector_create();
    vector_push(an->childs,create_step_node());
    return an;
}

void dpst_init(){
    root=create_finish_node();
}

int main(int argc, char const *argv[])
{
    // finish_node*root=create_finish_node();
    dpst_init();
    printf("hello\n");
    generic_node*root2=create_finish_node();
    // two ways of printing uint64_t
    printf("%ld\n", (long)((generic_node*)vector_get(root2->childs,0))->node_id); 
    printf("%" PRIu64 "\n", (root2->node_id));
    return 0;
}
