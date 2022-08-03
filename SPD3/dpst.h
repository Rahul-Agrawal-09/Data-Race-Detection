#include <stdlib.h>
#include <inttypes.h>

/*This is common generic_node of all 3 type of generic_node
type=0 => step node
type=1 => async node
type=2 => finish node */
typedef struct dpst_node{
    uint8_t type;
    uint64_t node_id;
    uint64_t depth;
    uint16_t num_children;
    uint64_t seq_no;
    struct dpst_node*parent;
} generic_node;


generic_node* ROOT;

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

//this function add async node in parent (returns the step added as its child)
generic_node* dpst_add_async_node(generic_node*parent);

//this function add step node [required after adding asyn and final node]
generic_node* dpst_add_step_node(generic_node*parent);

//this function add finish node in parent (returns the step added as its child)
generic_node* dpst_add_finish_node(generic_node*parent);