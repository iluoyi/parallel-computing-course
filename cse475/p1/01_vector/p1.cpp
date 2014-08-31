/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-08-27
 *  Version: 1
 *
 *  Description: This file parses command line options, outputs them, and
 *  then runs the test function.
 *
 *  For the purposes of the assignment, this file should be treated as
 *  read-only.
 */

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

#include "config_t.hpp"
#include "tests.hpp"

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
         << "  -h          : display this message" << endl << endl;
}

/**
 *  Parse command line arguments using getopt()
 *
 *  TODO: why is cfg passed by reference?
 */
void parseargs(int argc, char** argv, config_t& cfg)
{
    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "i:s:n:h")) != -1) {
        switch (opt) {
          case 'i': cfg.iters = atoi(optarg);  break;
          case 'n': cfg.name = string(optarg); break;
          case 's': cfg.seed = atoi(optarg);   break;
          case 'h': usage();                   break;
        }
    }
}

/**
 *  The main routine simply parses the arguments, dumps the arguments, populates the
 */
int main(int argc, char** argv)
{
    // get the configuration, print it
    config_t config;
    parseargs(argc, argv, config);
    config.dump();

    // crash if the iterations are negative
    assert(config.iters > 0);

    // launch the tests
    run_custom_tests(config);
}
