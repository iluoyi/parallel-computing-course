#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include "bmconfig.hpp"
#include "HashTable.hpp"
//#include "libitm.h"

/**
 * the data structure we will manipulate in the experiment
 */
hashTable* SET;
//static __attribute__((transaction_safe))
/**
 * Initialize the data structure and warm it up
 */
void bench_init()
{
    SET = new hashTable();
    // warm up the datastructure by filling it half full
    for (uint32_t w = 0; w < Config::CFG.elements; w+=2)
        SET->insert(w);
}

/**
 * Run a bunch of random operations
 */
void bench_test(uintptr_t, uint32_t* seed)
{
    
    uint32_t val = rand_r(seed) % Config::CFG.elements;


    uint32_t act = rand_r(seed) % 100;


    if (act < Config::CFG.lookpct)
      //__attribute__((transaction_safe))
      //[[transaction_safe]]
      //__transaction_atomic{
	SET->lookup(val);
    //}
    else if (act < Config::CFG.inspct)
      //__attribute__((transaction_safe))
      //[[transaction_safe]]
      //__transaction_atomic{
      SET->insert(val);
//}
    else
      //__attribute__((transaction_safe))
      //	[[transaction_safe]]
      //__transaction_atomic{
      SET->remove(val);
    //}
}

/**
 * Ensure the final state of the benchmark satisfies all invariants
 */
bool bench_verify()
{
    return true;
}
