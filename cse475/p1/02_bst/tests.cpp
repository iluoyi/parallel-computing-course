/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-08-27
 *  Version: 1
 *
 *  Description: This file implements a function that should be able to use the
 *  configuration information to drive tests that evaluate the correctness
 *  and performance of the map_t object.
 *
 *  This should be the focal point for student efforts to test their
 *  programs.  See below for suggestions.
 */

#include <iostream>

#include "config_t.hpp"
#include "tests.hpp"
#include "map_t.hpp"

using std::cout;
using std::endl;


/**
 *  For now, we just populate the data structure, and then print it out and
 *  sum its keys.
 *
 *  TODO: students should extend this function at least in all of the ways
 *        discussed below.
 */
void run_custom_tests(config_t& cfg)
{
    map_t<int, int> mc;

    // populate with even numbers between 0 and cfg.iters
    //
    // NB: this is a *bad* strategy for populating a tree, as it will not be
    //     balanced.  Use more randomization, or else implement a balanced
    //     tree.
	unsigned int seed_0 = cfg.seed;
	int success = 0;
    for (int i = 1; i <= cfg.iters; i++) {
       if (mc.insert(rand_r(&seed_0)%100, i)) { // now with random int
		success++;
	   }
	}
	cout << "\nInput iteration = " << cfg.iters << endl; // print out some statistics
	cout << "Successful insertion = " << success << endl;
	cout << "Duplicate keys detected = " << cfg.iters - success << endl;
	
    // TODO: Use a lambda to print every key, via in-order traversal, and get
    //       rid of the printer function
	cout << "\n1). To print all keys (key, value) with in_order(), namely in sorted order" << endl;
	mc.in_order([] (int k,int v) { 
				cout << "(" << k << ", " << v << ")" << endl;
				});

    // TODO: sum the keys without global variables or the sum_keys function
	cout << "\n2). To sum the keys" << endl;
    int sum = 0;
    mc.in_order([&] (int k, int v){ 
				sum += k;
				});
    cout << "sum = " << sum << endl;

    // TODO: average the keys
	cout << "\n3). To average the keys" << endl;
	int avg = 0;
	int num = 0;
    mc.in_order([&] (int k, int v){ 
				avg += k;
				num ++;
				});
    cout << "avg = " << avg << "/" << num << " = " << avg/num << "(rounded)" << endl;

    // TODO: develop tests to ensure that insert, update, remove, and lookup
    //       work properly
	cout << "\n4). To test insert()" << endl;
	unsigned int seed_1 = cfg.seed;
	int toBeInserted;
	for (int i = 0; i < cfg.iters/2; i++) rand_r(&seed_1);
	for (int i = 0; i < cfg.iters; i++) {    // insert some duplicate and then some new nodes
		toBeInserted = rand_r(&seed_1)%100;
		cout << "To insert (" << toBeInserted << ", 0): " << mc.insert(toBeInserted, 0) << ", [0-failed, 1-succeed]" << endl;
	}
	mc.in_order([] (int k,int v) { 
			cout << "(" << k << ", " << v << ")" << endl;
            });
			
	// update()
	cout << "\n5). To test update()" << endl;
	unsigned int seed_2 = cfg.seed;
	int toBeUpdated;
	for (int i = 0; i < cfg.iters; i++) rand_r(&seed_2);
	for (int i = 0; i < cfg.iters/2; i++) {
		toBeUpdated = rand_r(&seed_2)%100; // update what we have inserted just now
		cout << "To update (" << toBeUpdated << ", 0) with (" << toBeUpdated << ", -1): " << mc.update(toBeUpdated, -1) << ", [0-failed, 1-succeed]" << endl;
	}
	mc.in_order([] (int k,int v) { 
			cout << "(" << k << ", " << v << ")" << endl;
            });
			
	// remove()
	cout << "\n6). To test remove()" << endl;
	unsigned int seed_3 = cfg.seed;
	int toBeRemoved;
	for (int i = 0; i < cfg.iters; i++) rand_r(&seed_3);
	for (int i = 0; i < cfg.iters/2; i++) {
		toBeRemoved = rand_r(&seed_3)%100; // remove what we have updated just now
		cout << "To remove (key = " << toBeRemoved << "): " << mc.remove(toBeRemoved) << ", [0-failed, 1-succeed]" << endl;
	}
	mc.in_order([] (int k,int v) { 
			cout << "(" << k << ", " << v << ")" << endl;
            });
	
	// lookup()
	cout << "\n7). To test lookup()" << endl;
	unsigned int seed_4 = cfg.seed;
	int toBeLookedUp;
	for (int i = 0; i < cfg.iters/2; i++) rand_r(&seed_4);
	for (int i = 0; i < cfg.iters; i++) {
		toBeLookedUp = rand_r(&seed_4)%100;  // look up some old nodes and some new nodes (which have been removed)
		cout << "To look up " << toBeLookedUp << " : " << mc.lookup(toBeLookedUp).second << ", [0-not existed, 1-existed]" << endl;
	}
	
    // TODO: print the keys via pre-order traversal
	cout << "\n8). To print all keys (key, value) with pre_order()" << endl;
    mc.pre_order([] (int k,int v) { 
				cout << "(" << k << ", " << v << ")" << endl;
                });
			
    // TODO: print the tree nicely (i.e., in the shape of a tree)
	cout << "\n9). To print the tree" << endl;
	mc.level_order([] (int key, int index){ 
				cout << key << ", ";
				});
	cout << endl;
    
	// TODO: perform a search of all values to find one with a particular
    //       value.
	cout << "\n10). To perform a function that use the instead of only the key, so I print out all (key, value) pairs with an even value" << endl;
	mc.in_order([] (int k, int v) { 
				if (v % 2 == 0) {
					cout << "(" << k << ", " << v << ")" << endl;
				}
            });
}





