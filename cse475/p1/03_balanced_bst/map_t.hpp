/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-08-27
 *  Version: 2
 *	Author:	Yi Luo (yil712)
 *
 *  Description: This file specifies a simple map type, implemented using two
 *  vectors (one for keys, the other for values).  The expectation is that
 *  items in equal positions in the two vectors correlate.  That is, for all
 *  values of i, keys[i] is the key for the item in values[i].
 *
 *  This is a partially implemented class.  The specification for each
 *  function appears as comments.  Students are responsible for correcting
 *  and completing the implementation.
 */
#include <vector>
#include <queue>

#include <functional>
#include <utility>

using std::vector;
using std::queue;
using std::pair;

/**
 *  This is a map, which stores key, value pairs.  The assignment is to:
 *
 *    (a) complete the implementation using a vector of pairs
 *
 *    (b) re-implement the map using a binary search tree
 *
 *    (c) demonstrate the power of lambdas by extending the
 *        run_custom_tests() function.
 *
 *    (d) make the tree balanced
 */
template <class K, class V>
class map_t
{
  struct treeNode {	
	 int color; //0-red, 1-black
	 pair<K,V> nodeData;
	 treeNode* leftChild;
	 treeNode* rightChild;
	 treeNode* parent;
	 treeNode(): color(1), leftChild(NULL), rightChild(NULL), parent(NULL) {}
  };
  
  private:
	treeNode* rootNode;
	treeNode* nilNode; // same as NULL, but contains color info!
	
	bool leftRotation(treeNode* crtNode);
	bool rightRotation(treeNode* crtNode);
	void inorder(treeNode* crtNode, std::function<void(K, V)>f);
	void preorder(treeNode* crtNode, std::function<void(K, V)>f);

  public:
    /**
     *  The constructor is responsible for initializing internal data
     *  structures for storing keys and values.
     */
    map_t();
    /**
     *  Insert (key, val) if and only if the key is not currently present in
     *  the data structure.  Returns true on success, false if the key was
     *  already present.
     *
     *  TODO: fix this function
     */
    bool insert(K key, V val);

    /**
     *  If key is present in the data structure, replace its value with val
     *  and return true; if key is not present in the data structure, return
     *  false.
     *
     *  TODO: implement this function
     */
    bool update(K key, V val);

    /**
     *  Remove the (key, val) pair if it is present in the data structure.
     *  Returns true on success, false if the key was not already present.
     *
     *  TODO: implement this function
     */
    bool remove(K key);

    /**
     *  If key is present in the data structure, return a pair consisting of
     *  the corresponding val and true.  Otherwise, return a pair with the
     *  boolean set to false.
     *
     *  TODO: implement this function
     *
     *  TODO: What requirements does the use of std::pair place on
     *        constructors for the template parameter V? (V should have a copy constructor)
     */
    pair<V, bool> lookup(K key);

    /**
     *  Apply a function to each key, in order
     *
     *  TODO: switch to std::function, instead of ugly C function pointer
     *        syntax
     */
	void in_order(std::function<void (K,V)>);
	

    /**
     *  TODO: implement a function that can do a pre-order traversal.  As
     *        above, it should use std::function for its parameter
     */
    void pre_order(std::function<void (K,V)>);
	
	// for printing the tree
	void level_order(std::function<void (K,V)>);
};










