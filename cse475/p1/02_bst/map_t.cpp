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
    rootNode = NULL; // a null pointer
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
	if (rootNode == NULL) {
		treeNode* newNode = new treeNode;
		newNode->nodeData = pair<K, V>(key, val);
		rootNode = newNode;
	} else {
		treeNode* crtNode = rootNode;
		treeNode* preNode = NULL;
		while (crtNode != NULL) {
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
		treeNode* newNode = new treeNode;
		newNode->nodeData = pair<K, V>(key, val);
		
		if (key > (preNode->nodeData).first) { // should be put in the right child node
			preNode->rightChild = newNode;
		} else {
			preNode->leftChild = newNode;
		}
	}
	
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
	treeNode* preNode = NULL;
	while (crtNode != NULL) {
			if (key > (crtNode->nodeData).first) {
				preNode = crtNode;
				crtNode = crtNode->rightChild;
			} else if (key < (crtNode->nodeData).first) {
				preNode = crtNode;
				crtNode = crtNode->leftChild;
			} else {
				break;
			}
		}
	if (crtNode == NULL) {
		return false;
	}
	

	//case 1: the current node has only the leftchild
	if (crtNode->rightChild == NULL) {
		if (preNode == NULL) {
			rootNode = crtNode->leftChild;
		} else {
			if ((crtNode->nodeData).first > (preNode->nodeData).first) {
				preNode->rightChild = crtNode->leftChild;
			} else {
				preNode->leftChild = crtNode->leftChild;
			}
		}
		delete crtNode;
	} 
	//case 2: the current node has both leftchild and rightchild
	else {
		treeNode* smallNode = crtNode->rightChild;
		treeNode* momOfSmallNode = crtNode;
		while (smallNode->leftChild != NULL) {
			momOfSmallNode = smallNode;
			smallNode = smallNode->leftChild;
		}
		
		crtNode->nodeData = smallNode->nodeData;

		if (momOfSmallNode == crtNode) {
			crtNode->rightChild = smallNode->rightChild;
		} else {
			momOfSmallNode->leftChild = smallNode->rightChild;
		}
		delete smallNode;
	}

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
	while (crtNode != NULL) {
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
	while (crtNode != NULL) {
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

// the helper function
template <class K, class V>
void map_t<K, V>::inorder(treeNode* crtNode, std::function<void (K,V)> f) {
	if (crtNode != NULL) {
		inorder(crtNode->leftChild, f);
		f((crtNode->nodeData).first, (crtNode->nodeData).second);
		inorder(crtNode->rightChild, f);
	}
}

template <class K, class V>
void map_t<K, V>::pre_order(std::function<void (K,V)> f) {
    preorder(rootNode, f);
}
// the helper function
template <class K, class V>
void map_t<K, V>::preorder(treeNode* crtNode, std::function<void (K,V)> f) {
	if (crtNode != NULL) {
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
		
		if (crtNode->leftChild != NULL)
			nodeQueue->push(crtNode->leftChild);
		if (crtNode->rightChild != NULL)
			nodeQueue->push(crtNode->rightChild);
	}
}


/**
 *  TODO: explain why this line is needed
 *
 *  TODO: you will probably need more lines of this sort to complete all of
 *        your various tests.
 */
template class map_t<int, int>;  // if I include the implementation of map_t (map_t.cpp), I will not need this instantiation
