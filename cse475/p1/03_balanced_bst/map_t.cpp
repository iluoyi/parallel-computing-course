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
	nilNode = new treeNode; // default color is 1 (black)
	nilNode->rightChild = rootNode;
	nilNode->leftChild = rootNode;
	nilNode->parent = rootNode; // this pointer is no real use
	rootNode = nilNode; // a fake null pointer
}

template <class K, class V>
bool map_t<K, V>::leftRotation(treeNode* crtNode) {
	if (crtNode == nilNode || crtNode->rightChild == nilNode) {
		return false;
	}
	treeNode* right_Child = crtNode->rightChild;
	crtNode->rightChild = right_Child->leftChild; // might be nilNode
	right_Child->parent = crtNode->parent; // might be nilNode
	if (right_Child->leftChild != nilNode) {
		right_Child->leftChild->parent = crtNode;
	}
	if (crtNode->parent != nilNode) {
		right_Child->parent = crtNode->parent;
		if (crtNode == crtNode->parent->leftChild) {
			crtNode->parent->leftChild = right_Child;
		} else {
			crtNode->parent->rightChild = right_Child;
		}
	} else { // current node is the root
		rootNode = right_Child;
		// refresh the nilNode
		nilNode->leftChild = rootNode;
		nilNode->rightChild = rootNode;
		nilNode->parent = rootNode;
	}
	crtNode->parent = right_Child;
	right_Child->leftChild = crtNode;
	return true;
}

template <class K, class V>
bool map_t<K, V>::rightRotation(treeNode* crtNode) {
	if (crtNode == nilNode || crtNode->leftChild == nilNode) {
		return false;
	}
	treeNode* left_Child = crtNode->leftChild;
	crtNode->leftChild = left_Child->rightChild; // might be nilNode
	left_Child->parent = crtNode->parent; // might be nilNode
	if (left_Child->rightChild != nilNode) {
		left_Child->rightChild->parent = crtNode;
	}
	if (crtNode->parent != nilNode) {
		left_Child->parent = crtNode->parent;
		if (crtNode == crtNode->parent->leftChild) {
			crtNode->parent->leftChild = left_Child;
		} else {
			crtNode->parent->rightChild = left_Child;
		}
	} else { // current node is the root
		rootNode = left_Child;
		// refresh the nilNode
		nilNode->leftChild = rootNode;
		nilNode->rightChild = rootNode;
		nilNode->parent = rootNode;
	}
	crtNode->parent = left_Child;
	left_Child->rightChild = crtNode;
	return true;
}

/**
 *  Insert (key, val) if and only if the key is not currently present in
 *  the data structure.  Returns true on success, false if the key was
 *  already present.
 *
 *  TODO: fix this function
*/
template <class K, class V>
bool map_t<K, V>::insert(K key, V val) {
	// insert the new node
	treeNode* newNode;
	treeNode* crtNode;
	if (rootNode == nilNode) {
		newNode = new treeNode; // default color = 1 (black)
		newNode->nodeData = pair<K, V>(key, val);
		newNode->rightChild = nilNode;
		newNode->leftChild = nilNode;
		
		rootNode = newNode; 
		rootNode->parent = nilNode; // the parent of root is set to nilNode
		nilNode->leftChild = rootNode;
		nilNode->rightChild = rootNode;
		nilNode->parent = rootNode;
	} else {
		crtNode = rootNode;
		treeNode* preNode = nilNode;
		while (crtNode != nilNode) {
			if (key > (crtNode->nodeData).first) {
				preNode = crtNode;
				crtNode = crtNode->rightChild;
			} else if (key < (crtNode->nodeData).first) {
				preNode = crtNode;
				crtNode = crtNode->leftChild;
			} else {
				return false; // detected duplicate
			}
		}
		newNode = new treeNode; // default color is 1 (black)
		newNode->nodeData = pair<K, V>(key, val);
		newNode->rightChild = nilNode;
		newNode->leftChild = nilNode;
		
		if (key > (preNode->nodeData).first) { // should be put in the right child node
			preNode->rightChild = newNode;
		} else {
			preNode->leftChild = newNode; // the parent node is preNode now
		}
		newNode->parent = preNode;
	}
		
	// adjust the whole tree
	crtNode = newNode;
	crtNode->color = 0; // set the current node's color as Red

	while (crtNode->parent->color == 0) {// is red, if true, it must be at level >= 2
		if (crtNode->parent == crtNode->parent->parent->leftChild) {// parent node is a left guy
			treeNode* uncleNode = crtNode->parent->parent->rightChild;
			if (uncleNode->color == 0) { // if uncle is red
				crtNode->parent->color = 1;
				uncleNode->color = 1;
				crtNode->parent->parent->color = 0;
				crtNode = crtNode->parent->parent; // move the current node pointer up to its grandpa
			} else { // if uncle is black
				if (crtNode == crtNode->parent->rightChild) { // current node is a right child of its parent of course
					crtNode = crtNode->parent;
					leftRotation(crtNode);
				} else {
					crtNode->parent->parent->color = 0;
					crtNode->parent->color = 1;
					crtNode = crtNode->parent->parent;
					rightRotation(crtNode);
				}
			}
		} else { // parent node is a right guy
			treeNode* uncleNode = crtNode->parent->parent->leftChild;
			if (uncleNode->color == 0) { // if uncle is red
				crtNode->parent->color = 1;
				uncleNode->color = 1;
				crtNode->parent->parent->color = 0;
				crtNode = crtNode->parent->parent; // move the current node pointer up to its grandpa
			} else { // if uncle node is black
				if (crtNode == crtNode->parent->leftChild) { // current node is a right child of its parent of course
					crtNode = crtNode->parent;
					rightRotation(crtNode);
				} else {
					crtNode->parent->parent->color = 0;
					crtNode->parent->color = 1;
					crtNode = crtNode->parent->parent;
					leftRotation(crtNode);
				}
			}
		}
	} 
	rootNode->color = 1;
	
    return true;
}

  /**
     *  Remove the (key, val) pair if it is present in the data structure.
     *  Returns true on success, false if the key was not already present.
     *
     *  TODO: implement this function
     */
template <class K, class V>
bool map_t<K, V>::remove(K key) {
	//first, we should detect the node to be removed
	treeNode* crtNode = rootNode;
	while (crtNode != nilNode) {
			if (key > (crtNode->nodeData).first) {
				crtNode = crtNode->rightChild;
			} else if (key < (crtNode->nodeData).first) {
				crtNode = crtNode->leftChild;
			} else {
				break;
			}
		}
	if (crtNode == nilNode) {
		return false;
	}
	// now crtNode is the one to be removed and now not nilNode
	treeNode* removedNode = crtNode;
	
	if (removedNode->rightChild != nilNode && removedNode->leftChild != nilNode) {
		treeNode* smallNode = removedNode->rightChild;

		while (smallNode->leftChild != nilNode) {
			smallNode = smallNode->leftChild;
		}
		
		removedNode->nodeData = smallNode->nodeData;
		removedNode = smallNode;
	} 
	treeNode* childOfRemovedNode;
	if (crtNode->rightChild != nilNode) {
		childOfRemovedNode = removedNode->rightChild;
	} else if (crtNode->leftChild != nilNode) {
		childOfRemovedNode = removedNode->leftChild;
	} else {
		childOfRemovedNode = nilNode;
	}
	
	if (removedNode->parent == nilNode) {
		rootNode = childOfRemovedNode;
		childOfRemovedNode->parent = nilNode;
		nilNode->rightChild = rootNode;
		nilNode->leftChild = rootNode;
		nilNode->parent = rootNode;
	} else {
		if ((removedNode->nodeData).first > (removedNode->parent->nodeData).first) {
				removedNode->parent->rightChild = childOfRemovedNode;
			} else {
				removedNode->parent->leftChild = childOfRemovedNode;
			}
		}
	childOfRemovedNode->parent = removedNode->parent;
		
	// to adjust the tree
	if (removedNode->color == 1 &&
		!(childOfRemovedNode == nilNode && //a leafNode (one-level higher than nilNode) is removed
		childOfRemovedNode->parent == nilNode)) { //a root is removed
		// a black one is removed AND !(a leafNode is removed AND a root is removed)
		crtNode = childOfRemovedNode; // the adjustment starts from the current node 
		treeNode* brotherNode;
		while (crtNode != rootNode && crtNode->color == 1) {
			if (crtNode == crtNode->parent->leftChild) {
				brotherNode = crtNode->parent->rightChild; // refresh the brother node each time
				if (brotherNode->color == 0) { // case 1: if the brother node is RED
					brotherNode->color = 1;
					crtNode->parent->color = 0;
					leftRotation(crtNode->parent);
				} else { 
					// case 2: if the brother node is BLACK and both children are BLACK
					if (brotherNode->leftChild->color == 1 && brotherNode->rightChild->color == 1) {
						brotherNode->color = 0;
						crtNode = crtNode->parent;
					} 
					// case 3: if the brother node is BLACK and its left child is RED while right child is BLACK
					else if (brotherNode->leftChild->color == 0 && brotherNode->rightChild->color == 1) {
						brotherNode->color = 0;
						brotherNode->leftChild->color = 1;
						rightRotation(brotherNode);
					}
					// case 3: if the brother node is BLACK and its right child is RED
					else if (brotherNode->rightChild->color == 0) {
						brotherNode->color = crtNode->parent->color;
						crtNode->parent->color = 1;
						brotherNode->rightChild->color = 1;
						leftRotation(crtNode->parent);
						crtNode = rootNode;
					}
				}
			}
			else {
				brotherNode = crtNode->parent->leftChild; // refresh the brother node each time
				if (brotherNode->color == 0) { // case 1: if the brother node is RED
					brotherNode->color = 1;
					crtNode->parent->color = 0;
					rightRotation(crtNode->parent);
				} else { 
					// case 2: if the brother node is BLACK and both children are BLACK
					if (brotherNode->rightChild->color == 1 && brotherNode->leftChild->color == 1) {
						brotherNode->color = 0;
						crtNode = crtNode->parent;
					} 
					// case 3: if the brother node is BLACK and its right child is RED while left child is BLACK
					else if (brotherNode->rightChild->color == 0 && brotherNode->leftChild->color == 1) {
						brotherNode->color = 0;
						brotherNode->rightChild->color = 1;
						leftRotation(brotherNode);
					}
					// case 3: if the brother node is BLACK and its left child is RED
					else if (brotherNode->leftChild->color == 0) {
						brotherNode->color = crtNode->parent->color;
						crtNode->parent->color = 1;
						brotherNode->leftChild->color = 1;
						rightRotation(crtNode->parent);
						crtNode = rootNode;
					}
				}
			}
		}
		nilNode->rightChild = rootNode;
		nilNode->leftChild = rootNode;
		nilNode->parent = rootNode;
		rootNode->color = 1;
	}
	delete removedNode;
	return true;
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
	treeNode* crtNode = rootNode;
	while (crtNode != nilNode) {
			if (key > (crtNode->nodeData).first) {
				crtNode = crtNode->rightChild;
			} else if (key < (crtNode->nodeData).first) {
				crtNode = crtNode->leftChild;
			} else {
				(crtNode->nodeData).second = val;
				return true;
			}
		}
	return false;
}
  /**
     *  If key is present in the data structure, return a pair consisting of
     *  the corresponding val and true.  Otherwise, return a pair with the
     *  boolean set to false. 
     *
     *  TODO: implement this function
     *
     *  TODO: What requirements does the use of std::pair place on
     *        constructors for the template parameter V? (copy constructor)
     */
template <class K, class V>
pair<V, bool> map_t<K, V>::lookup(K key) {
	treeNode* crtNode = rootNode;
	while (crtNode != nilNode) {
			if (key > (crtNode->nodeData).first) {
				crtNode = crtNode->rightChild;
			} else if (key < (crtNode->nodeData).first) {
				crtNode = crtNode->leftChild;
			} else {
				//std::cout << (crtNode->nodeData).second << std::endl;
				return pair<V, bool>((crtNode->nodeData).second, true);
			}
		}
	pair<V, bool> rltPair;
	rltPair.second = false;
	return rltPair;
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
    inorder(rootNode, f);
}

template <class K, class V>
void map_t<K, V>::inorder(treeNode* crtNode, std::function<void (K,V)> f) {
	if (crtNode != nilNode) {
		inorder(crtNode->leftChild, f);
		f((crtNode->nodeData).first, (crtNode->nodeData).second);
		inorder(crtNode->rightChild, f);
	}
}

template <class K, class V>
void map_t<K, V>::pre_order(std::function<void (K,V)> f) {
    preorder(rootNode, f);
}

template <class K, class V>
void map_t<K, V>::preorder(treeNode* crtNode, std::function<void (K,V)> f) {
	if (crtNode != nilNode) {
		f((crtNode->nodeData).first, (crtNode->nodeData).second);
		preorder(crtNode->leftChild, f);
		preorder(crtNode->rightChild, f);
	}
}

template <class K, class V>
void map_t<K, V>::level_order(std::function<void (K,V)> f) {

    queue<treeNode*>* nodeQueue = new queue<treeNode*>();
	nodeQueue->push(rootNode);
	
	treeNode* crtNode;
	int index = 0;
	while (nodeQueue->size() > 0) {
		index ++;
		crtNode = nodeQueue->front();
		nodeQueue->pop();
		f((crtNode->nodeData).first, index);
		
		if (crtNode->leftChild != nilNode)
			nodeQueue->push(crtNode->leftChild);
		if (crtNode->rightChild != nilNode)
			nodeQueue->push(crtNode->rightChild);
	}
}



/**
 *  TODO: explain why this line is needed
 *
 *  TODO: you will probably need more lines of this sort to complete all of
 *        your various tests.
 */
template class map_t<int, int>; // if I include the implementation of map_t (map_t.cpp), I will not need this instantiation
