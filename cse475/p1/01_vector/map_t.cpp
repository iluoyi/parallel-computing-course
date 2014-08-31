/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-08-27
 *  Version: 1
 *
 *  Description: This file implements a few of the functions for a simple
 *  map, but does so poorly.  The student's job is to complete the
 *  implementation and do it well.
 */

#include "map_t.hpp"
#include <iostream>

/**
 *  Construct two vectors for our map
 *
 *  TODO: change this according to the changes to map_t's fields
 */
template <class K, class V>
map_t<K, V>::map_t() {
    storage = new vector<pair<K,V>>();
}

/**
 *  Insert (key, val) if and only if the key is not currently present in
 *  the data structure.  Returns true on success, false if the key was
 *  already present.
 *
 *  TODO: fix this function
*/
template <class K, class V>
bool map_t<K, V>::insert(K key, V val) {// to explicitly define a function of a specific class

    // if the key is in the map, return false.
    for (auto i = storage->begin(); i != storage->end(); ++i) { 
	// i is the iterator of the vector(namely vector<pair<K,V>>::iterator), I have to dereference to get the real element (pair<K,V>)
    //if ((*i).first == key) {
		if (i->first == key) {
            return false;
        }
    }

    // binary search to insert a new element in a sorted list in O(log(n)) time
	int i = 0;
	int j = storage->size() - 1;
	int k = 0;
	auto newPair = pair<K,V>(key, val);
	
	if (j >= 0) {
		do {
			k = (i + j) / 2;
			if (key >= (storage->at(k)).first) {
				i = k + 1; 
			} else {
				j = k - 1;
			}
		} while (i <= j);
		if (i >= storage->size()) {
			storage->push_back(newPair);
		} else {
			storage->insert(storage->begin()+i, newPair);
		}
	} else {
		storage->push_back(newPair);
	}
	
    return true;
}

/**
 *  Perform a traversal of the map, and execute function 'f()' on each k/v
 *  pair
 *
 *  TODO: This code is obviously not correct, since it doesn't use the values
 *        vector at all.  It's probably the case that for an implementation
 *        that uses a pair of vectors, the aggressive use of auto and vector
 *        ranges is too optimistic. "what kind of order?"
 */
template <class K, class V>
void map_t<K, V>::in_order(std::function<void (K,V)> f) {
    for (auto i : *storage) {	// range-based for loop, c++11 new feature, i is not an iterator but a real element
        f(i.first, i.second);   // it is the same to "values->at(t)"
    }
}

template <class K, class V>
void map_t<K, V>::pre_order(std::function<void (K,V)> f) {
    for (auto i : *storage) {	
        f(i.first, i.second); 
    }
}

template <class K, class V>
void map_t<K, V>::level_order(std::function<void (K,V)> f) {
    for (auto i : *storage) {	
        f(i.first, i.second); 
    }
}

/**
     *  If key is present in the data structure, replace its value with val
     *  and return true; if key is not present in the data structure, return
     *  false.
     *
     *  TODO: implement this function
     */
template <class K, class V>
bool map_t<K, V>::update(K key, V val) {
	//Yi: this way cannot change the original value. (a copy)
	//
	//for (auto i : *storage) {
	//	if (i.first == key) {
	//		i.second = val;
	//		return true;
	//	}
	//}
	
	for (auto i = storage->begin(); i != storage->end(); ++i) {
		if (i->first == key) {
			i->second = val;
            return true;
        }
    }
	return false;
}

   /**
     *  Remove the (key, val) pair if it is present in the data structure.
     *  Returns true on success, false if the key was not already present.
     *
     *  TODO: implement this function
     */
template <class K, class V>
bool map_t<K, V>::remove(K key) {
	for (auto i = storage->begin(); i != storage->end(); ++i) {
		if (i->first == key) {
			storage->erase(i);
            return true;
        }
    }
	return false;
}

  /**
     *  If key is present in the data structure, return a pair consisting of
     *  the corresponding val and true.  Otherwise, return a pair with the
     *  boolean set to false.   (what about V?)
     *
     *  TODO: implement this function
     *
     *  TODO: What requirements does the use of std::pair place on
     *        constructors for the template parameter V?
     */
template <class K, class V>
pair<V, bool> map_t<K, V>::lookup(K key) {
	for (auto i : *storage) {
		if (i.first == key) {
			return pair<V, bool>(i.second, true);
		}
	}
	pair<V, bool> resultPair;
	resultPair.second = false;
	return resultPair;
}

/**
 *  TODO: explain why this line is needed
 *
 *  TODO: you will probably need more lines of this sort to complete all of
 *        your various tests.
 */
template class map_t<int, int>;  // explicit instantiation (because map_t.cpp and tests.cpp are independent in compiling process)
                                 // if I include the implementation of map_t (map_t.cpp), I will not need this instantiation
