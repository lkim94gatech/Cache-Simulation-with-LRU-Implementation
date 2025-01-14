#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cachesim.h"

// Statistics you will need to keep track. DO NOT CHANGE THESE.
counter_t accesses = 0;     // Total number of cache accesses
counter_t hits = 0;         // Total number of cache hits
counter_t misses = 0;       // Total number of cache misses
counter_t writebacks = 0;   // Total number of writebacks

/**
 * Function to perform a very basic log2. It is not a full log function, 
 * but it is all that is needed for this assignment. The <math.h> log
 * function causes issues for some people, so we are providing this. 
 * 
 * @param x is the number you want the log of.
 * @returns Techinically, floor(log_2(x)). But for this lab, x should always be a power of 2.
 */
int simple_log_2(int x) {
    int val = 0;
    while (x > 1) {
        x /= 2;
        val++;
    }
    return val; 
}

//  Here are some global variables you may find useful to get you started.
//      Feel free to add/change anyting here.
cache_set_t* cache;     // Data structure for the cache
int block_size;         // Block size
int cache_size;         // Cache size
int ways;               // Ways
int num_sets;           // Number of sets
int num_offset_bits;    // Number of offset bits
int num_index_bits;     // Number of index bits. 
int num_tag_bits;       // Number of tag bits

/**
 * Function to intialize your cache simulator with the given cache parameters. 
 * Note that we will only input valid parameters and all the inputs will always 
 * be a power of 2.
 * 
 * @param _block_size is the block size in bytes
 * @param _cache_size is the cache size in bytes
 * @param _ways is the associativity
 */
void cachesim_init(int _block_size, int _cache_size, int _ways) {
    // Set cache parameters to global variables
    block_size = _block_size;
    cache_size = _cache_size;
    ways = _ways;

    ////////////////////////////////////////////////////////////////////
    //  initialize cache simulator.
    //      - Calculate any values needed such as number of index bits.
    //      - Allocate any data structures needed.   
    ////////////////////////////////////////////////////////////////////

    // # of index
    num_sets = cache_size / (block_size * ways);  
    // # of index bit k
    num_index_bits = simple_log_2(num_sets);
    // # of offset bit b
    num_offset_bits = simple_log_2(block_size);
    // # of tag bit t -> max 32 
    num_tag_bits = 32 - num_offset_bits - num_index_bits;        

    // For cache pointer
    cache = (cache_set_t*) malloc(sizeof(cache_set_t) * num_sets);

    // Allocate pointers and set other variables for each index 
    for (int i = 0; i < num_sets; i++) {
        cache[i].size = ways;
        cache[i].stack = init_lru_stack(ways);
        cache[i].blocks = (cache_block_t *)malloc(ways * sizeof(cache_block_t));

        // initial valid, dirty and tag bits 
        for (int j = 0; j < ways; j++){
            cache[i].blocks[j].valid = 0;
            cache[i].blocks[j].dirty = 0;
            cache[i].blocks[j].tag = 0;
        }
    }
}

/**
 * Function to perform a SINGLE memory access to your cache. In this function, 
 * you will need to update the required statistics (accesses, hits, misses, writebacks)
 * and update your cache data structure with any changes necessary.
 * 
 * @param physical_addr is the address to use for the memory access. 
 * @param access_type is the type of access - 0 (data read), 1 (data write) or 
 *      2 (instruction read). We have provided macros (MEMREAD, MEMWRITE, IFETCH)
 *      to reflect these values in cachesim.h so you can make your code more readable.
 */
void cachesim_access(addr_t physical_addr, int access_type) {
    ////////////////////////////////////////////////////////////////////
    //  code needed to perform a cache access to the cache simulator.
    //      - SHOULD NOT bring another cache block in when it is a  
    //        cache hit.
    //      - When it is a cache miss, bring a new cache block
    //        in. If the set is full, evict the LRU block.
    //      - update all the necessary statistics as necessary
    //      - correctly update valids and dirty bits.  
    ////////////////////////////////////////////////////////////////////
    
    accesses++;

    // Extracting tag bits, offset bits, and index bits from an address using bit masking and AND operation.
    int offset_b = physical_addr & ((1 << num_offset_bits) - 1);
    int index_b = (physical_addr >> (num_offset_bits)) & ((1 << num_index_bits) - 1);
    int tag_b = (physical_addr >> (num_offset_bits + num_index_bits)) & ((1 << num_tag_bits) - 1);

    // Checking hit
    for (int i = 0; i < ways; i++){
        if ((cache[index_b].blocks[i].valid == 1) && (cache[index_b].blocks[i].tag == tag_b)){
            hits++;

            // If Memwrite, set dirty bit to 1
            if (access_type == MEMWRITE){
                cache[index_b].blocks[i].dirty = 1;
            }

            // Updating lru
            lru_stack_set_mru(cache[index_b].stack, i);

            return;
        }
    }

    misses++;

    // Checking invalid block
    for (int i = 0; i < ways; i++){
        if (cache[index_b].blocks[i].valid == 0){
            // Found, so bring in tag and set the valid bit
            cache[index_b].blocks[i].tag = tag_b;
            cache[index_b].blocks[i].valid = 1;

            // If Memwrite, set dirty bit
            if (access_type == MEMWRITE){
                cache[index_b].blocks[i].dirty = 1;
            }

            // Updating mru
            lru_stack_set_mru(cache[index_b].stack, i);
            return;
        }
    }

    // Getting lru tag
    int lru_index = lru_stack_get_lru(cache[index_b].stack);

    // If necessary, update writeback
    if ((cache[index_b].blocks[lru_index].dirty == 1) && (cache[index_b].blocks[lru_index].valid == 1)){
        writebacks++;

        // Also, reset dirty
        cache[index_b].blocks[lru_index].dirty = 0;
    }

    // New tag addr to lru
    cache[index_b].blocks[lru_index].tag = tag_b;

    // Replaced lru -> becomes mru
    lru_stack_set_mru(cache[index_b].stack, lru_index);

    // If necessary, set dirty bit
    if (access_type == MEMWRITE){
        cache[index_b].blocks[lru_index].dirty = 1;
    }
}

/**
 * Function to free up any dynamically allocated memory you allocated
 */
void cachesim_cleanup() {
    ////////////////////////////////////////////////////////////////////
    //  code to do any heap allocation cleanup
    ////////////////////////////////////////////////////////////////////

    for (int i = 0; i < num_sets; i++){
        lru_stack_cleanup(cache[i].stack);
        free(cache[i].blocks);
    }
    // Free entire cache
    free(cache);
}

/**
 * Function to print cache statistics
 * DO NOT update what this prints.
 */
void cachesim_print_stats() {
    printf("%llu, %llu, %llu, %llu\n", accesses, hits, misses, writebacks);  
}

/**
 * Function to open the trace file
 * You do not need to update this function. 
 */
FILE *open_trace(const char *filename) {
    return fopen(filename, "r");
}

/**
 * Read in next line of the trace
 * 
 * @param trace is the file handler for the trace
 * @return 0 when error or EOF and 1 otherwise. 
 */
int next_line(FILE* trace) {
    if (feof(trace) || ferror(trace)) return 0;
    else {
        int t;
        unsigned long long address, instr;
        fscanf(trace, "%d %llx %llx\n", &t, &address, &instr);
        cachesim_access(address, t);
    }
    return 1;
}

/**
 * Main function. See error message for usage. 
 * 
 * @param argc number of arguments
 * @param argv Argument values
 * @returns 0 on success. 
 */
int main(int argc, char **argv) {
    FILE *input;

    if (argc != 5) {
        fprintf(stderr, "Usage:\n  %s <trace> <block size(bytes)>"
                        " <cache size(bytes)> <ways>\n", argv[0]);
        return 1;
    }
    
    input = open_trace(argv[1]);
    cachesim_init(atol(argv[2]), atol(argv[3]), atol(argv[4]));
    while (next_line(input));
    cachesim_print_stats();
    cachesim_cleanup();
    fclose(input);
    return 0;
}

