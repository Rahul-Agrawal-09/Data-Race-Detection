#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "vector.h"
#include "dpst.h"

int id=1;
node* create_node(){
    node*n=(node*)malloc(sizeof(node));
    n->node_id=(int*)malloc(sizeof(int));
    *(n->node_id)=5;
    return n;
}

step_node*create_step_node(){
    step_node*sn=(step_node*)malloc(sizeof(step_node));
    sn->my_node=create_node();
    return sn;
}

finish_node* create_finish_node(){
    finish_node*fn=(finish_node*)malloc(sizeof(finish_node));
    fn->my_node=create_node();
    fn->childs=vector_create();
    vector_push(fn->childs,create_step_node());
    return fn;
}

async_node* create_async_node(){
    async_node*an=(async_node*)malloc(sizeof(async_node));
    an->my_node=create_node();
    an->childs=vector_create();
    vector_push(an->childs,create_step_node());
    return an;
}

void dpst_init(finish_node* root){
    root=create_finish_node();
}

int main(int argc, char const *argv[])
{
    finish_node*root;
    dpst_init(root);
    printf("hello\n");
    printf("%d\n", *(root->my_node->node_id));
    return 0;
}
