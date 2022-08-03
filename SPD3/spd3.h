#include "memory.h"

//this function initialises the tree with the implicit finish node 
void spd3_launch(void client_main(generic_node*));

//this function adds a async node and its step in the tree
void spd3_async(void client_async(generic_node*), generic_node*step_spawning);

//this function adds a finish node and its step in the tree
void spd3_finish(void client_finish(generic_node*), generic_node*step_spawning);
