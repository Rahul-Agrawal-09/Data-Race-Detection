/**
 * Program: SPD3 (dynamic data race detector in async/finish parallel program)
 * Author: Rahul Agrawal
 * Mentor: Vivek Kumar
 * 
 * Referece: https://files.sri.inf.ethz.ch/website/papers/pldi12-spd3.pdf
 * 
 * The Program detect data race for dynamic memory location in async/finish parallel program
 *  > async   = spawns new task which run asynchronous with other tasks
 *  > finish  = blocks the execution till all the task spawned within finish terminates
 */

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "dpst.h"

/*
    DPST (Dynamic Program Sructure Tree) is a tree structure in which grows during run time.

    There are three type of node in DPST
    finish node => this node is created when finish api is called
    async node  => this node is crested when async api is called
    step node   => all the instruction execution occue in step node which is child of a finish or an async node

    DPST tree initialises with a root finish node when the function is launched and grows as we keep on 
    spawning new tasks using async and synchronize them with finish functions. 

    =>  If a async_node been spawned inside step node of a async/finish function then a new async node (AN) will 
        be added as the right most child of the node representing the parent async/finish function.

    =>  If a finish_node been spawned inside step node of a async/finish function then a new finish node (FN) will
        be added as the right most child of the node representing the parent async finish funtcion.

    =>  For every new async/finish (NewNode) node a new step node is added to the new async node representing the 
        computation inside that new async/finish ndoe and a new step node is added as child to the parent's 
        async/finish node (after NewNode) whihc represents the computation followint NewNode inside the parent.
*/

//id variable for allocating node_id member of the DPST node (for debugging)
uint64_t id=1;

generic_node* create_node(){
    //creates new generic_node and return it's reference
    generic_node*n=(generic_node*)malloc(sizeof(generic_node));
    
    //initialising node_id to new id and num_children to 0 as initially no children are there
    n->node_id=id++;
    n->num_children=0;
    return n;
}


generic_node*create_step_node(generic_node*parent){
    //create new step generic_node and return it's reference
    generic_node*sn=create_node();

    //assigning different fields of node such that parent is the parent node
    sn->type=STEP;                  
    sn->parent=parent;
    sn->depth=(parent->depth)+1;
    parent->num_children++;
    sn->seq_no=parent->num_children; 
    return sn;
}


generic_node* create_finish_node(generic_node*parent){
    //creates new generic_node and return it's reference
    generic_node*fn=create_node();

    //assigning different fields of node such that parent is the parent node 
    fn->type=FINISH;                    //assigning type field to specific macro
    fn->parent=parent;                  //assigning parent field to parent argument
    fn->depth=(parent->depth)+1;        //depth of current node is one more than its parent depth
    parent->num_children++;             //increasing number of children of the parent node
    fn->seq_no=parent->num_children;    //position of the child in parent
    return create_step_node(fn);        //the step node for the child async/finish node
}


generic_node* create_async_node(generic_node*parent){
    //create new async generic_node and return it's reference
    generic_node*an=create_node();

    //assigning different fields of node such that parent is the parent node (assignment same as create_finish_node)
    an->type=ASYNC;
    an->parent=parent;
    an->depth=(parent->depth)+1;
    parent->num_children++;
    an->seq_no=parent->num_children;
    return create_step_node(an);
}


generic_node* dpst_init(){
    //initialises the DPST when hclib launch is called and adding the implicit finish root node
    generic_node*fn=create_node();
    fn->type=FINISH;
    fn->depth=0;
    fn->seq_no=1;
    fn->parent=NULL;
    ROOT=fn;
    return create_step_node(fn);
}


generic_node* dpst_add_async_node(generic_node*parent){
    //this function add a new async node to parent (returns the pointer to the step node added)
    return create_async_node(parent);
}

generic_node* dpst_add_step_node(generic_node*parent){
    //this function add a new step node to the parent (return the pointer to the step node been addded)
    return create_step_node(parent);
}

generic_node* dpst_add_finish_node(generic_node*parent){
    //this function add finish node to the parent (returns the pointer to the step added been added)
    return create_finish_node(parent);
}


generic_node* dpst_LCA(generic_node*s1,generic_node*s2){\
    //this function find Lowest Common Ancestors (LCA) of two step nodes s1 and s2 (return pointer to LCA of s1 and s2)

    //bringing both the node pointer to same level by traversing lower step node pointer upwards 
    while(s1->depth != s2->depth){
        if(s1->depth > s2->depth)
            s1=s1->parent;
        if(s1->depth < s2->depth)
            s2=s2->parent;
    }

    //with both pointers at same level we move both pointer upwards till both lands on same node (this is our LCA)
    while (s1!=s2){
        s1=s1->parent;
        s2=s2->parent;
    }
    return s1;
}


generic_node* dpst_A_and_C(generic_node*step,generic_node*LCA){
    //this function will find the node which is ancestors of step node but child of LCA node 
    //(return pointer to resulting node)
    while(step!=NULL && step->parent!=LCA){
        step=step->parent;
    }
    return step;
}

bool dpst_DMHP(generic_node*s1 , generic_node*s2){
    // Dynamic May Happen in Parallel (DMHP) checks whether two step nodes s1 and s2 can run in paralle or not

    generic_node *lca=dpst_LCA(s1,s2);          //finding the LCA of s1 ans s2
    generic_node*A1=dpst_A_and_C(s1,lca);       //getting the node whihc is child of LCA but ancestors of s1 
    generic_node*A2=dpst_A_and_C(s2,lca);       //getting the node whihc is child of LCA but ancestors of s2

    /*
    the steps s1 ans s2 can happen in parallel if the step on the left of the LCA have an async node as 
    an ancestors of that step node and a child of the LCA.
    */
    if(A1->seq_no < A2->seq_no){
        return (A1->type==ASYNC);               
    }
    else{
        return (A2->type==ASYNC);
    }
}
