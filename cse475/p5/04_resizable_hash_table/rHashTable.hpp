#include "List.hpp"
#include <iostream>

using std::cout;
using std::endl;

class hashTable {
private:
    List** table; // only pointers, no references. "List table[]" is invalid...
    int capacity = 0;
    int size = 0;
  
    int hashCode(int val) {
        return val;
    }
  
    int hash(int hashCode) {
        return hashCode & (capacity - 1);
    }
  
    [[transaction_safe]] 
    void rehash() {
        List* tempList = new List();
	List* bucket;
	for (int i = 0; i < capacity; i ++) { 
	    if (table[i] != NULL){
	        bucket = table[i];
		for (int j = 0; j < bucket->getSize(); j ++) {
		    tempList->insert(bucket->get(j));
		}
		delete(bucket);
	    }
	}
	size = 0;
	delete(table);
	
	capacity *= 2;
	
	table = new List*[capacity];
	
	for (int i = 0; i < tempList->getSize(); i ++) {
	    insert(tempList->get(i));
	}
	
	delete(tempList);
    }
	
public:
    hashTable() {
        capacity = 10; // initial capacity
	table = new List*[capacity];
    }
  
    int getSize() {
        return size;
    }
    //__attribute__((transaction_safe))
    bool lookup(int val) {
	int bucketIndex = hash(hashCode(val));
	__transaction_atomic{
	    if (table[bucketIndex] != NULL) {
	        List* bucket = table[bucketIndex];
		if (bucket->lookup(val))
		    return true;
	    }
	}
	return false;
    }
  


  bool insert(int val) {
      if (lookup(val))
	  return false;
      int bucketIndex = hash(hashCode(val));
      __transaction_relaxed{
	  if (table[bucketIndex] != NULL) {
	      if (table[bucketIndex]->getSize() >= 8) {
		  
		  rehash();
		  
		  bucketIndex = hash(hashCode(val));
	      }
	  }
	  
	  if (table[bucketIndex] == NULL) {
	      table[bucketIndex] = new List();
	  }
	  
	  table[bucketIndex]->insert(val);
	  size ++;
      }
      return true;
  }
  
  
  //__attribute__((transaction_safe))
  bool remove(int val) {
      
	if (!lookup(val))
	  return false;
	__transaction_atomic{
	  int bucketIndex = hash(hashCode(val));
	  if (table[bucketIndex] != NULL) {
	      List* bucket = table[bucketIndex];
	      bucket->remove(val);
	  }
	  size --;
      }
      return true;
  }
};
