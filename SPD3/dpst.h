#include <stdlib.h>
#include <inttypes.h>
#include "vector.h"

//this is common node parent of all 3 type of node
typedef struct dpst_node{
    int* node_id;
} node;

//structure of finish node
typedef struct dpst_finish_node{
    node* my_node;
    vector childs;
} finish_node;

//structure of async node
typedef struct dpst_async_node{
    node* my_node;
    vector childs;
} async_node;

//structure of step node 
typedef struct dpst_step_node{
    node* my_node;
} step_node;

//this function creates new node
node* create_node();

//this function creates new finish node
finish_node* create_finish_node();

//this function create new async node
async_node* create_async_node();

//this function create new step node
step_node* create_step_node();

//this functon initialises the dpst when hclib launch is called
void dpst_init(finish_node* root);