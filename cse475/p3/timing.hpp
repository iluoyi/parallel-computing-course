/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-09-11
 *  Version: 1
 *
 *  Description: This file declares functions for computing the elapsed time
 *  in a program
 *
 *  For the purposes of the assignment, this file should be treated as
 *  read-only.
 */

#ifndef TIMING_HPP__
#define TIMING_HPP__

#include <time.h>
#include <stdint.h>

/**
 *  The Linux clock_gettime is reasonably fast, has good resolution, and is not
 *  affected by TurboBoost.  Using MONOTONIC_RAW also means that the timer is
 *  not subject to NTP adjustments, which is preferably since an adjustment in
 *  mid-experiment could produce some funky results.
 */
inline uint64_t getElapsedTime()
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    uint64_t tt = (((long long)t.tv_sec) * 1000000000L) + ((long long)t.tv_nsec);
    return tt;
}

#endif // TIMING_HPP__
