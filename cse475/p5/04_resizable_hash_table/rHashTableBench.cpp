#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include "bmconfig.hpp"
#include "rHashTable.hpp"

hashTable* SET;

void bench_init()
{
    SET = new hashTable();
     
    for (uint32_t w = 0; w < Config::CFG.elements; w+=2)
        SET->insert(w);
}

void bench_test(uintptr_t, uint32_t* seed)
{
    
    uint32_t val = rand_r(seed) % Config::CFG.elements;
    uint32_t act = rand_r(seed) % 100;

    if (act < Config::CFG.lookpct)
	SET->lookup(val);
    else if (act < Config::CFG.inspct)
      SET->insert(val);
    else
      SET->remove(val);
}

bool bench_verify()
{
    return true;
}
