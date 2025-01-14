#include <stdlib.h>
#include <stdio.h>
#include "lrustack.h"

/**
 * Function to initialize an LRU stack for a cache set with a given <size>. This function
 * creates the LRU stack. 
 * 
 * @param size is the size of the LRU stack to initialize. 
 * @return the dynamically allocated stack. 
 */
lru_stack_t* init_lru_stack(int size) {
    //  Use malloc to dynamically allocate a lru_stack_t
	lru_stack_t* stack = (lru_stack_t*) malloc(sizeof(lru_stack_t));
    //  Set the stack size the caller passed in
	stack->size = size;
    
    ////////////////////////////////////////////////////////////////////
    //  code needed to initialize your LRU Stack. 
    ////////////////////////////////////////////////////////////////////

    // Priority bits array for lru
    stack -> priority_bits = (int*)malloc(sizeof(int)*size);
    for(int i = 0; i < stack->size; i++) {
        stack->priority_bits[i] = i;
    }
	return stack;
}

/**
 * Function to get the index of the least recently used cache block, as indicated by <stack>.
 * This operation should not change/mutate your LRU stack. 
 * 
 * @param stack is the stack to run the operation on.
 * @return the index of the LRU cache block.
 */
int lru_stack_get_lru(lru_stack_t* stack) {
    ////////////////////////////////////////////////////////////////////
    //  code to get the index of the LRU block from the LRU stack.
    ////////////////////////////////////////////////////////////////////

    // Index (size - 1) = lru
    for (int i = 0; i < stack->size; i++) {
        if ((stack->size - 1) == stack->priority_bits[i]) {
            return i;
        }
    }
}

/**
 * Function to mark the cache block with index <n> as MRU in <stack>. This operation should
 * change/mutate the LRU stack.
 * 
 * @param stack is the stack to run the operation on.
 * @param n the index to promote to MRU.  
 */
void lru_stack_set_mru(lru_stack_t* stack, int n) {
	////////////////////////////////////////////////////////////////////
    //  code to set the passed in block index  as the MRU 
    //  element in the LRU Stack. 
    ////////////////////////////////////////////////////////////////////

    // Run each index that didn't passed into the block, and increment
    for (int i = 0; i < stack->size; i++) {
        if (stack->priority_bits[n] > stack->priority_bits[i]) {
            stack->priority_bits[i]++;
        }
    }
    
    stack -> priority_bits[n] = 0;
}

/**
 * Function to free up any memory you dynamically allocated for <stack>
 * 
 * @param stack the stack to free
 */
void lru_stack_cleanup(lru_stack_t* stack) {
    ////////////////////////////////////////////////////////////////////
    //  code to do additional heap allocation
    //  cleanup
    ////////////////////////////////////////////////////////////////////

    free(stack->priority_bits);
    free(stack);        // Free the stack struct we malloc'd
}
