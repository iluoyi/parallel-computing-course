/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-08-27
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
 *
 *  TODO: what is the only difference between a struct and a class in C++? struct is public
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
     *  A string that is output along with other information, to assist in
     *  producing easy-to-parse output
     */
    std::string  name;

    /**
     *  Simple constructor
     */
    config_t() : seed(0), iters(1024), name("no_name") { }

    /**
     *  Print the values of the seed, iters, and name fields
     */
    void dump();
};
