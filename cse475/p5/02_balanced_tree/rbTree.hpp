/**
 *  Origin:  Lehigh CSE Department
 *  Course:  CSE 375/475
 *  Date:    2013-12-7
 *  Version: 3
 *	Author:	Yi Luo (yil712), Jundong Yao
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
#include <functional>
#include <utility>
#include <iostream>

using std::cout;
using std::endl;
/**
 *  This is a set, which stores values as elements of nodes.
 */
class rbTree
{
  struct treeNode {	
	 int color; //0-red, 1-black
	 /**
      *  The value stored at this node
      */
	 int val; // replace "nodeData"
	 treeNode* leftChild;
	 treeNode* rightChild;
	 treeNode* parent;
	 treeNode(): color(1), leftChild(NULL), rightChild(NULL), parent(NULL) {}
  };
  
  private:
	treeNode* rootNode;
	treeNode* nilNode; // same as NULL, but contains color info!
	
	void inorder(treeNode* crtNode) {
		if (crtNode != nilNode) {
			inorder(crtNode->leftChild);
			cout << crtNode->val << ", ";
			inorder(crtNode->rightChild);
		}
	}
	
	bool leftRotation(treeNode* crtNode) {
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
			//right_Child->parent = crtNode->parent;
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
			//nilNode->parent = rootNode;
		}
		crtNode->parent = right_Child;
		right_Child->leftChild = crtNode;
		return true;
	}

	bool rightRotation(treeNode* crtNode) {
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
			//left_Child->parent = crtNode->parent;
			if (crtNode == crtNode->parent->rightChild) {
				crtNode->parent->rightChild = left_Child;
			} else {
				crtNode->parent->leftChild = left_Child;
			}
		} else { // current node is the root
			rootNode = left_Child;
			// refresh the nilNode
			nilNode->leftChild = rootNode;
			nilNode->rightChild = rootNode;
			//nilNode->parent = rootNode;
		}
		crtNode->parent = left_Child;
		left_Child->rightChild = crtNode;
		return true;
	}


  public:
    /**
     *  The constructor is responsible for initializing internal data
     *  structures for storing keys and values.
     */
    rbTree() {
		nilNode = new treeNode; // default color is 1 (black)
		nilNode->rightChild = rootNode;
		nilNode->leftChild = rootNode;
		nilNode->parent = rootNode; // this pointer is no real use
		rootNode = nilNode; // a fake null pointer
	}
    /**
     *  Insert (val) if and only if the val is not currently present in
     *  the data structure.
     */
  __attribute__((transaction_safe))
  bool insert(int val) {
	__transaction_atomic{
		// insert the new node
		treeNode* newNode;
		treeNode* crtNode;
		
		if (rootNode == nilNode) {
			newNode = new treeNode; // default color = 1 (black)
			newNode->val = val;
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
				if (val > crtNode->val) {
					preNode = crtNode;
					crtNode = crtNode->rightChild;
				} else if (val < crtNode->val) {
					preNode = crtNode;
					crtNode = crtNode->leftChild;
				} else {
					return false; // detected duplicate
				}
			}
			newNode = new treeNode; // default color is 1 (black)
			newNode->val = val;
			newNode->rightChild = nilNode;
			newNode->leftChild = nilNode;
			
			if (val > preNode->val) { // should be put in the right child node
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
						//crtNode = crtNode->parent->parent;
						rightRotation(crtNode->parent->parent);
					}
				}
			} else { // parent node is a right guy
				treeNode* uncleNode = crtNode->parent->parent->leftChild;
				if (uncleNode->color == 0) { // if uncle is red
					crtNode->parent->color = 1;
					uncleNode->color = 1;
					uncleNode->parent->color = 0;
					crtNode = crtNode->parent->parent; // move the current node pointer up to its grandpa
				} else { // if uncle node is black
					if (crtNode == crtNode->parent->leftChild) { // current node is a right child of its parent of course
						crtNode = crtNode->parent;
						rightRotation(crtNode);
					} else {
						crtNode->parent->parent->color = 0;
						crtNode->parent->color = 1;
						leftRotation(crtNode->parent->parent);
					}
				}
			}
		}
		 
		rootNode->color = 1;
		
		return true;
		}
	}
	
	treeNode* InOrderSuccessor(treeNode* node) {  
        if(node==nilNode)       
                {  
                    return nilNode;  
                }  
                treeNode* result = node->rightChild;  
                while(result!=nilNode)   
                {  
                    if(result->leftChild!=nilNode)       
                    {  
                        result = result->leftChild;  
                    }  
                    else  
                    {  
                        break;  
                    }  
                }                          
                if(result == nilNode)  
                {  
                    treeNode* index = node->parent;  
                    result = node;  
                    while(index!=nilNode && result == index->rightChild)  
                    {  
                        result = index;  
                        index = index->parent;  
                    }  
                    result = index; 
                }  
                return result;  
            }  

    /**
     *  Remove the (val) if it is present in the data structure.
     *  Returns true on success, false if the key was not already present.
     */
  __attribute__((transaction_safe))  
  bool remove(int val) {
    __transaction_atomic{
		//first, we should detect the node to be removed, from textbook
		treeNode* crtNode = rootNode;
		while (crtNode != nilNode) {
				if (val > crtNode->val) {
					crtNode = crtNode->rightChild;
				} else if (val < crtNode->val) {
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
			// treeNode* smallNode = removedNode->rightChild;

			// // to find the successor "smallNode"
			// while (smallNode->leftChild != nilNode) {
				// smallNode = smallNode->leftChild;
			// }
			
			treeNode* smallNode = InOrderSuccessor(removedNode);
			
			removedNode->val = smallNode->val;
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
		childOfRemovedNode->parent = removedNode->parent;
		if (removedNode->parent == nilNode) {
			rootNode = childOfRemovedNode;
			childOfRemovedNode->parent = nilNode;
			nilNode->rightChild = rootNode;
			nilNode->leftChild = rootNode;
			nilNode->parent = rootNode;
		} else if (removedNode == removedNode->parent->rightChild) {
			removedNode->parent->rightChild = childOfRemovedNode;
		} else {
			removedNode->parent->leftChild = childOfRemovedNode;
		}
			
		// to adjust the tree
		if (removedNode->color == 1 &&
			!(childOfRemovedNode == nilNode && //a leafNode (one-level higher than nilNode) is removed
			childOfRemovedNode->parent == nilNode)) { //a root is removed
			// a black one is removed AND !(a leafNode is removed AND a root is removed)
			removeFixUp(childOfRemovedNode);
		}
		delete removedNode;
    }
		return true;
	}
	
	void removeFixUp(treeNode* crtNode) {
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
						// case 4: if the brother node is BLACK and its right child is RED
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
						// case 4: if the brother node is BLACK and its left child is RED
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
			//nilNode->rightChild = rootNode;
			//nilNode->leftChild = rootNode;
			nilNode->parent = rootNode;
			crtNode->color = 1;
	}

    /**
     *  If val is present in the data structure, return true.  
	 *	Otherwise return false.
     */
  __attribute__((transaction_safe))
  bool lookup(int val) {
    __transaction_atomic{
		treeNode* crtNode = rootNode;
		while (crtNode != nilNode) {
				if (val > crtNode->val) {
					crtNode = crtNode->rightChild;
				} else if (val < crtNode->val) {
					crtNode = crtNode->leftChild;
				} else {
					return true;
				}
			}
    }
		return false;
	}
	
	void in_order() {
		inorder(rootNode);
	}
};










