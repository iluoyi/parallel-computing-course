#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include "bmconfig.hpp"
#include "templateHashtable.hpp"

/**
 * the data structure we will manipulate in the experiment
 */
hashTable<int, int>* SET;

/**
 * Initialize the data structure and warm it up
 */
void bench_init()
{
  SET = new hashTable<int, int>();
    // warm up the datastructure by filling it half full
    for (uint32_t w = 0; w < Config::CFG.elements; w+=2)
      SET->insert(w, w);
}

/**
 * Run a bunch of random operations
 */
void bench_test(uintptr_t, uint32_t* seed)
{
    // select a random value
    uint32_t val = rand_r(seed) % Config::CFG.elements;

    // select a random number to guide what operation to perform
    uint32_t act = rand_r(seed) % 100;

    // do an operation using the values we just created
    if (act < Config::CFG.lookpct)
      SET->lookup(val);
    else if (act < Config::CFG.inspct)
      SET->insert(val, val);
    else
      SET->remove(val);
}

/**
 * Ensure the final state of the benchmark satisfies all invariants
 */
bool bench_verify()
{
    return true;
}
