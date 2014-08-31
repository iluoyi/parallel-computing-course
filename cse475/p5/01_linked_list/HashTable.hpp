#include "List.hpp"

class hashTable {
  private:
    List** table; // only pointers, no references. "List table[]" is invalid...
	int capacity = 1024;
	int size = 0;
	
	int hashCode(int val) {
		return val;
	}
	
	int hash(int hashCode) {
		return hashCode & (capacity - 1);
	}
	
  public:
	hashTable() {
		table = new List*[capacity];
	}
	
	int getSize() {
		return size;
	}
	
	bool lookup(int val) {
		int bucketIndex = hash(hashCode(val));
		if (table[bucketIndex] != NULL) {
			List* bucket = table[bucketIndex];
			if (bucket->lookup(val))
				return true;
		}
		return false;
	}
  __attribute__((transaction_safe))
	bool insert(int val) {
    __transaction_atomic{ 
    if (lookup(val))
			return false;
		//__transaction_atomic{
		int bucketIndex = hash(hashCode(val));
		if (table[bucketIndex] == NULL) {
			table[bucketIndex] = new List();
		}
		table[bucketIndex]->insert(val);
		size ++;
		}
		return true;
	}
  __attribute__((transaction_safe))
	bool remove(int val) {
    __transaction_atomic{ 
    if (!lookup(val))
			return false;
		//__transaction_atomic{
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
