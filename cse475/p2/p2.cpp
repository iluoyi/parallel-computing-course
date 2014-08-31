/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-09-11
 *  Version: 1
 *
 *  Description: This file parses command line options, outputs them, and
 *  then runs the sequential and parallel ocean functions.
 *
 *  For the purposes of the assignment, this file should be treated as
 *  read-only.
 */

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdint.h>

#include "config_t.hpp"
#include "ocean.hpp"
#include "timing.hpp"

using std::cout;
using std::endl;
using std::string;

/**
 *  Report on how to use the command line to configure this program
 */
void usage()
{
    cout << "Command-Line Options:" << endl
         << "  -i <int>    : the number of iterations to run " << endl
         << "  -n <string> : provide a single word that prefixes "
                            "all output, to" << endl
         << "                aid in understanding different runs "
                            "of the program" << endl
         << "  -s <int>    : provide an integer to serve as the "
                            "seed for random" << endl
         << "                number generation" << endl
         << "  -g <int>    : provide an integer to serve as the "
                            "size of the grid" << endl
         << "                being simulated" << endl
         << "  -v          : enable verbose output" << endl
         << "  -h          : display this message" << endl << endl;
}

/**
 *  Parse command line arguments using getopt()
 */
void parseargs(int argc, char** argv, config_t& cfg)
{
    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "i:s:n:g:hv")) != -1) {
        switch (opt) {
          case 'i': cfg.iters = atoi(optarg);    break;
          case 'n': cfg.name = string(optarg);   break;
          case 's': cfg.seed = atoi(optarg);     break;
          case 'g': cfg.gridsize = atoi(optarg); break;
          case 'v': cfg.verbose = true;          break;
          case 'h': usage();                     break;
        }
    }
}

/**
 *  The main routine simply parses the arguments, dumps the arguments, runs a
 *  simulation, and reports elapsed time.
 */
int main(int argc, char** argv)
{
    // get the configuration, print it
    config_t config;
    parseargs(argc, argv, config);
    config.dump();

    // crash if the iterations are negative
    assert(config.iters > 0);

    // get the time, run the sequential test, get the time
    init_ocean(config);
    uint64_t seqstart = getElapsedTime();
    ocean_sequential(config);
    uint64_t seqstop = getElapsedTime();
    dump_ocean(config);
    free_ocean(config);

	cout << endl;
	
    // get the time, run the parallel test, get the time
    init_ocean(config);
    uint64_t parstart = getElapsedTime();
    ocean_parallel(config);
    uint64_t parstop = getElapsedTime();
    dump_ocean(config);
    free_ocean(config);

    // report data
    cout << "Sequential:\t" << (seqstop - seqstart) << " (ns)" << endl;
    cout << "Parallel:\t"   << (parstop - parstart) << " (ns)" << endl;
}
