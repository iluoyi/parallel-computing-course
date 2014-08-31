#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include "myString.hpp"
#include "bmconfig.hpp"
#include "templateHashtable.hpp"


/**
 * the data structure we will manipulate in the experiment
 */
hashTable<myString, int>* SET;

myString IntToString(uint32_t intVal)
{
	uint32_t intContainer = intVal;
	char charTemp[50];
	myString str;
	sprintf(charTemp, "%d", intContainer);
	str = charTemp;
	return str;
}

/**
 * Initialize the data structure and warm it up
 */
void bench_init()
{
	SET = new hashTable<myString, int>();
	// warm up the datastructure by filling it half full
	for (uint32_t w = 0; w < Config::CFG.elements; w+=2){
		myString s = IntToString(w);
		SET->insert(s, w);
	}
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
	myString s = IntToString(val);
	// do an operation using the values we just created
	if (act < Config::CFG.lookpct)
		SET->lookup(s);
	else if (act < Config::CFG.inspct)
		SET->insert(s, val);
	else
		SET->remove(s);
}

/**
 * Ensure the final state of the benchmark satisfies all invariants
 */
bool bench_verify()
{
    return true;
}
