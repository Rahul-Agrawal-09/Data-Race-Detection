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
#include <stdbool.h>

/**
 * @brief 
 * Generic_node of represents the node of all 3 type inside the DPST.
 * The diferent members used in the generic_node of the DPST are
    > type: Indicates the type of node [async/finish/step]
            type=0 => step node
            type=1 => async node
            type=2 => finish node 
    > node_id: unique identification for every node (used for debugging purposes)
    > depth: the depth of the node in the DPST (root level is 0)
    > num_children: number of children of the node (start with 0 and grows as child nodes are added)
    > seq_no: the sequence number at which the node is in the list of child nodes of the parent
    > parent: pointer to the parent node 
 */
typedef struct dpst_node{
    uint8_t type;
    uint64_t node_id;
    uint64_t depth;
    uint16_t num_children;
    uint64_t seq_no;
    struct dpst_node*parent;
} generic_node;

//macros for type member of the node
#define STEP 0
#define ASYNC 1
#define FINISH 2

//Reference to the root of the dpst
generic_node* ROOT;


/**
 * @brief 
 * This function initialises new generic_node and initialises the trivial 
 * fields such as node_id and num_children and return pointer to the new node.
 */
generic_node* create_node();


/**
 * @brief 
 * This function initialises a step node and with its fields and return the 
 * pointer to the new node.
 */
generic_node* create_step_node(generic_node*parent);


/**
 * @brief 
 * This fuction initialises a new finish node and add this node as a child to the parent node.
 * 
 * The function returns pointer to the step node of the new finish node
 */
generic_node* create_finish_node(generic_node*parent);


/**
 * @brief 
 * This fuction initialises a new async node and add this node as a child to the parent node.
 * 
 * The function returns pointer to the step node of the new async node
 */
generic_node* create_async_node(generic_node*parent);


/**
 * @brief 
 * This is the initialization function of the DPST and called when program is launched.
 * 
 * In this function the root node is initialised with a finish node. This finish node represent 
 * the implicit finish surrounding the main function in an async/finish parallel program.
 */
generic_node* dpst_init();


/**
 * @brief 
 * This function add an async node as a child to the parent node and returns the pointer to the 
 * step node of the new child async node.
 */
generic_node* dpst_add_async_node(generic_node*parent);


/**
 * @brief 
 * This function adds a step node as a child to the parent node and returns the pointer to the 
 * new step node.
 */
generic_node* dpst_add_step_node(generic_node*parent);


/**
 * @brief 
 * This function add a finish node as a child to the parent node and returns the pointer to the 
 * step node of the new child finish node.
 */
generic_node* dpst_add_finish_node(generic_node*parent);


/**
 * @brief 
 * This function is used to find Lowest Common Ancestors (LCA) of two step node in the DPST.
 * Following are the steps to find LCA :
 *  1.Starting with two pointer to the two node traverse the pointer with greater depth 
 *    upwards till it reaches the same depth as other.
 *  2.Traverse both the pointer upwards (i.e. parent of the node) till both pointer points to 
 *    the same node. This node is the LCA of the two step node.
 */
generic_node* dpst_LCA(generic_node*s1 ,generic_node*s2);


/**
 * @brief 
 * Ancestors and Child (A_and_C) function find the node that is ancestors of the step node but 
 * child of the parent node and return pointer to this node.
 * 
 * To find A_and_C start with step node and travese upwards (i.e. parent of the node) till parent
 * of current node is equal to LCA. 
 * 
 * Note: since LCA is an ancestors of step so such an A_and_C node always exist.
 */
generic_node* dpst_A_and_C(generic_node*step,generic_node*LCA);


/**
 * @brief 
 * Dynamic May Happen in Parallel (DMHP) checks if the provided two steps can happen in parallel 
 * or not. This is done using the theorem
 * 
 * Theorem: If S1 and S2 (S1!=S2) are two step node with S1 been left node. Node A is the A_and_C node 
 * of S1 and LCA(S1,S2). The S1 and S2 can happen in parallel if A is an async node.
 * 
 * Following are the steps to find DMHP :
 *  1.Find LCA of S1 and S2 let this node be LCA.
 *  2.Find A_and_C of S1 and LCA let this node be A1.
 *  3.Find A_and_C of S2 and LCA let this node be A2.
 *  4.Return TRUE if "A1 is left (seq_no of A1 is less) and A1 is an async node" OR if 
 *    "A2 is left (seq_no of A2 is less) and A2 is an async node." else return false.
 */
bool dpst_DMHP(generic_node*s1 , generic_node*s2);