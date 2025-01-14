# Cache Simulation with LRU Implementation

This repository contains a cache simulation program written in **C**. The project implements a **Least Recently Used (LRU)** cache policy to simulate the behavior of a CPU cache. It includes support for set-associative caches and provides functionality for testing and validation.

---

## Files Included

- **cachesim.c**: Implements the main cache simulation logic.
- **cachesim.h**: Header file defining the interface for the cache simulator.
- **lrustack.c**: Implements the LRU stack used to track the cache's block replacement policy.
- **lrustack.h**: Header file defining the interface for the LRU stack.

---

## Overview

The simulation focuses on implementing an LRU replacement policy for a set-associative cache. The program takes cache configuration parameters as input, simulates memory accesses, and provides cache performance statistics, including hit and miss rates.

### Key Features
1. **Cache Simulation**:
   - Simulates set-associative caches.
   - Tracks cache hits, misses, and block replacements.
   
2. **LRU Replacement Policy**:
   - Maintains the order of cache blocks using an LRU stack.
   - Supports operations to get the least recently used (LRU) block and mark a block as the most recently used (MRU).

3. **Configurable Parameters**:
   - Cache size
   - Block size
   - Associativity

4. **Testing and Debugging**:
   - Includes functions for validating the LRU stack behavior.
   - Generates output for waveform analysis.

---

## Setup Instructions

### Prerequisites

- **C Compiler**: `gcc` or any other standard C compiler.
- **Make** (Optional): For easier build process.

---

## Compilation and Execution

1. **Compile the Cache Simulator**:
   ```bash
   gcc -o cachesim cachesim.c lrustack.c
   ```
2. ** Run the Simulator**:
   ```bash
   ./cachesim <cache_size> <block_size> <associativity> <trace_file>
   ```

## Modules Breakdown

### **cachesim.c**
- **Purpose**: Implements the main logic for simulating the cache.
- **Key Functions**:
  - Parse input parameters.
  - Simulate memory accesses based on the trace file.
  - Compute cache hit and miss rates.

### **lrustack.c**
- **Purpose**: Implements the LRU stack functionality.
- **Key Functions**:
  - `init_lru_stack(size)`: Initializes an LRU stack with a given size.
  - `lru_stack_get_lru(stack)`: Returns the index of the least recently used block.
  - `lru_stack_set_mru(stack, index)`: Marks the block as the most recently used.
  - `lru_stack_cleanup(stack)`: Frees dynamically allocated memory for the LRU stack.

### **lrustack.h**
- **Purpose**: Defines the interface for the LRU stack.

---

## Testing

### **Simulation**
- Run test cases to validate the cache behavior under different configurations.
- Analyze the hit and miss statistics for various input traces.

### **LRU Stack**
- Validate the LRU stack using the provided `lrustacktest.c`.
- Ensure correctness of `get LRU` and `set MRU` operations.

