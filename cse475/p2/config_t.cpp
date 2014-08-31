/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-09-11
 *  Version: 1
 *
 *  Description: This file implements the config_t::dump() function
 *
 *  For the purposes of the assignment, this file should be treated as
 *  read-only.
 */
#include <iostream>

#include "config_t.hpp"

using std::cout;
using std::endl;

/**
 *  Print the values of the seed, iters, and name fields as comma separated
 *  values, with a header row
 */
void config_t::dump()
{
    cout << "# name, seed, gridsize, verbose, iters" << endl;
    cout << name << ", " << seed << ", " << gridsize << ", "
         << verbose << ", " << iters << endl;
}
