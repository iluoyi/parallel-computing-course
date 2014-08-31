/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-09-11
 *  Version: 1
 *
 *  Description: This file declares a struct for storing per-execution
 *  configuration information.
 *
 *  For the purposes of the assignment, this file should be treated as
 *  read-only.
 */

#include <iostream>
#include <string>

/**
 *  This struct stores all of our command-line configuration parameters
 */
struct config_t
{
    /**
     *  Store the random seed
     */
    unsigned int seed;

    /**
     *  The number of iterations for which a test should be run
     */
    int iters;

    /**
     *  The size of the (square) grid to simulate
     */
    int gridsize;

    /**
     *  Indicate if the final grid should be printed or not
     */
    bool verbose;

    /**
     *  A string that is output along with other information, to assist in
     *  producing easy-to-parse output
     */
    std::string  name;

    /**
     *  Simple constructor
     */
    config_t()
        : seed(0), iters(1024), gridsize(514), verbose(false), name("no_name")
    { }

    /**
     *  Print the values of the fields
     */
    void dump();
};
