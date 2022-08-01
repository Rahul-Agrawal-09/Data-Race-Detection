#include <stdlib.h>
#include <inttypes.h>
#include "vector.h"

/*This is common generic_node of all 3 type of generic_node
type=0 => step node
type=1 => async node
type=2 => finish node */
typedef struct dpst_node{
    uint64_t node_id;
    uint8_t type;
    struct dpst_node*parent;
    vector childs;
} generic_node;

//this function creates new generic_node
generic_node* create_node();

//this function creates new finish generic_node
generic_node* create_finish_node();

//this function create new async generic_node
generic_node* create_async_node();

//this function create new step generic_node
generic_node* create_step_node();

//this functon initialises the dpst when hclib launch is called
generic_node* dpst_init();

//this function add async node in parent
generic_node* dpst_add_async_node(generic_node*parent);