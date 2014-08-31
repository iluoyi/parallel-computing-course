/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-09-11
 *  Version: 1
 *
 *  Description: This file declares functions for performing ocean
 *  simulations
 *
 *  For the purposes of the assignment, this file should be treated as
 *  read-only.
 */

/**
 *  Run an ocean simulation that is sequential
 */
void ocean_sequential(config_t& cfg);

/**
 *  Run an ocean simulation that uses TBB to execute in parallel
 */
void ocean_parallel(config_t& cfg);

/**
 *  (Re)Initialize data structures based on command-line parameters
 */
void init_ocean(config_t& cfg);

/**
 *  Clean up data structures after a simulation has been run
 */
void free_ocean(config_t& cfg);

/**
 *  Print the ocean, for verification
 */
void dump_ocean(config_t& cfg);

