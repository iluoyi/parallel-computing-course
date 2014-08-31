#ifndef BMCONFIG_HPP__
#define BMCONFIG_HPP__

#include <stdint.h>
#include <iostream>

/**
 * Benchmark configuration globals
 */
struct Config
{
    uint32_t    duration;               /// in seconds
    uint32_t    threads;                /// number of threads
    uint32_t    elements;               /// elements in data structure
    uint32_t    lookpct;                /// lookup percent
    uint32_t    inspct;                 /// insert percent

    volatile uint64_t totaltime;        /// total time for the experiment
    volatile bool     running;          /// is the test currently running?
    volatile uint32_t opcount;          /// number of operations that occurred

    /**
     * Simple constructor: set everything to default values for a
     * single-thread, single-second test with a 256-element data structure
     * and an even lookup/insert/remove ratio
     */
    Config() :
        duration(1), threads(1), elements(256), lookpct(34), inspct(66),
        totaltime(0), running(true), opcount(0)
    {
    }

    /**
     * A nice simple singleton pattern: we have exactly one Config object.
     * Note that due to this being a .hpp and not a .cpp, we need to make it
     * extern.
     */
    static Config CFG;
};

/** BENCHMARKS IMPLEMENT THE FOLLOWING FUNCTIONS */

/**
 * Initialize the data structure
 */
void bench_init();

/**
 * Run a bunch of random operations on the data structure
 */
void bench_test(uintptr_t, uint32_t*);

/**
 * Ensure the final state of the data structure satisfies all invariants
 */
bool bench_verify();

#endif // BMCONFIG_HPP__
