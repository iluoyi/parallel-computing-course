/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-09-11
 *  Version: 1
 *
 *  Description: This file implements the ocean simulations
 *
 *  This should be the focal point for student efforts to implement their
 *  programs.
 */

#include <iostream>

#include "config_t.hpp"
#include "ocean.hpp"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

using namespace tbb;
using std::cout;
using std::endl;

/**
 *  An array of points, to simulate our ocean
 */
float* ocean_points;

/**
 *  Allocate an ocean and populate it with random floats.  Since this is an
 *  ocean simulation, we'll use reasonable celsius temperatures (e.g.,
 *  between 0 and 51), and we'll only have 3 significant digits to the right
 *  of the decimal.
 */
void init_ocean(config_t& cfg)
{
    // make a copy of the seed, since it gets modified by each call to
    // rand_r()
    unsigned seed = cfg.seed;

    ocean_points = new float[cfg.gridsize * cfg.gridsize];
    for (int i = 0; i < cfg.gridsize * cfg.gridsize; ++i) {
        // NB: making a random float from two random ints that we mod into a
        // reasonable range
        unsigned lhs = rand_r(&seed);
        unsigned rhs = rand_r(&seed);
        lhs = lhs % 50;
        rhs = rhs % 1000;
        float value = rhs;
        value = value / (float)1000;
        value += lhs;
        ocean_points[i] = value; // between 0 and 51, e.g. 48.235
    }
}

/**
 *  When a simulation is done, we can use this to free the ocean_points data
 *  structure.
 */
void free_ocean(config_t& cfg)
{
    free(ocean_points);
}

/**
 *  Simple print routine to show the state of the ocean_points array
 */
void dump_ocean(config_t& cfg)
{
    if (!cfg.verbose)
        return;
    for (int i = 0; i < cfg.gridsize * cfg.gridsize; ++i) {
        cout << ocean_points[i];
        if ((i + 1) % cfg.gridsize == 0)
            cout << endl;
        else
            cout << ", ";
    }
}

/**
 *  Perform a sequential ocean simulation
 */
void ocean_sequential(config_t& cfg)
{
    // todo: simulate ocean for appropriate number of timesteps
	float* ocean_points_copy;
	float* ocean_points_temp;
	ocean_points_copy = new float[cfg.gridsize * cfg.gridsize];
	
	for (int t = 0; t < cfg.iters; ++t) {
		for (int i = 0; i < cfg.gridsize * cfg.gridsize; ++i) {
			if ((i < cfg.gridsize) || (i > (cfg.gridsize - 1) * cfg.gridsize - 1) ||
				(i % cfg.gridsize == 0) || (i % cfg.gridsize == cfg.gridsize - 1)) {
				ocean_points_copy[i] = ocean_points[i]; // don't change the edge grids
			} else {
				ocean_points_copy[i] = (ocean_points[i] + ocean_points[i - cfg.gridsize] + 
										ocean_points[i + cfg.gridsize] + ocean_points[i - 1] + 
										ocean_points[i + 1]) * 0.2; // same as 1/5
			}
		}
		ocean_points_temp = ocean_points_copy;
		ocean_points_copy = ocean_points;
		ocean_points = ocean_points_temp;
	}
	delete(ocean_points_copy);
}

/**
 *  This struct can also be a class. This is useful for TBB implementation
 */
struct Ocean {
	float* ocean_points_start;
	float* ocean_points_end;
	int gridsize;
	
	Ocean(float* o_p_s, float* o_p_e, int size): ocean_points_start(o_p_s), ocean_points_end(o_p_e), gridsize(size) { }
	
	void operator() (const blocked_range<int>& r) const {
		for ( int i = r.begin(); i != r.end(); i++ ) { // the algorithm keeps the same as the sequential implementation
			if ((i < gridsize) || (i > (gridsize - 1) * gridsize - 1) ||
				(i % gridsize == 0) || (i % gridsize == gridsize - 1)) {
				ocean_points_end[i] = ocean_points_start[i]; // don't change the edge grids
			} else {
				ocean_points_end[i] = (ocean_points_start[i] + ocean_points_start[i - gridsize] + 
										ocean_points_start[i + gridsize] + ocean_points_start[i - 1] + 
										ocean_points_start[i + 1]) * 0.2; // same as 1/5
			}
		}
	}
};
/**
 *  Perform a parallel ocean simulation, using TBB
 */
void ocean_parallel(config_t& cfg)
{
    // todo: simulate ocean for appropriate number of timesteps
	float* ocean_points_copy = new float[cfg.gridsize * cfg.gridsize];
	float* ocean_points_temp;
	
	for (int t = 0; t < cfg.iters; ++t) {
		parallel_for(blocked_range<int>(0, cfg.gridsize * cfg.gridsize, cfg.gridsize),
			Ocean(ocean_points, ocean_points_copy, cfg.gridsize)); // TBB's loop parallelization
	
		ocean_points_temp = ocean_points_copy;
		ocean_points_copy = ocean_points;
		ocean_points = ocean_points_temp;
	}
	delete(ocean_points_copy);
}
