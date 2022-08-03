#include"spd3.h"

generic_node* ROOT;

//This function initialises the dpst
void spd3_launch(void client_main(generic_node*)){
    ROOT = dpst_init();

    //Passing step_id as parameter so as to use write and read method
    client_main(((generic_node*)vector_get(ROOT->childs,0)));
    return;
}

//This function spawns an async node and add it to nearest ancestors
void spd3_async(void client_async(generic_node*), generic_node*step_spawning){
    generic_node*parent=step_spawning->parent;
    generic_node*step_async = dpst_add_async_node(parent);
    *step_spawning=*dpst_add_step_node(parent);
    client_async(step_async);
}

void spd3_finish(void client_finish(generic_node*), generic_node*step_spawning){
    generic_node*parent=step_spawning->parent;
    generic_node*step_finish=dpst_add_finish_node(parent);
    *step_spawning=*dpst_add_step_node(parent);
    client_finish(step_finish);
}

