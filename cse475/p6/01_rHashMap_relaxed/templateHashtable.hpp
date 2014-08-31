#include "templateList.hpp"
#include <iostream>

using std::cout;
using std::endl;

template <class K, class V>
class hashTable {
  private:
    List<K, V>** table; // only pointers, no references. "List table[]" is invalid...
	int capacity = 0;
	int size = 0;
	
	std::hash<K> hashCode; // can be customized
	
	int hash(int hashCode) {
		return hashCode & (capacity - 1);
	}
	
	void rehash() {
		List<K, V>* tempList = new List<K,V>();
		List<K, V>* bucket;
		
		for (int i = 0; i < capacity; i ++) { 
			if (table[i] != NULL){
				bucket = table[i];
				for (int j = 0; j < bucket->getSize(); j ++) {
					tempList->insert((bucket->get(j)).first, (bucket->get(j)).second);
				}
				delete(bucket);
			}
		}
		size = 0;
		delete(table);
		
		capacity *= 2;
		table = new List<K, V>*[capacity];
		
		for (int i = 0; i < tempList->getSize(); i ++) {
			insert((tempList->get(i)).first, (tempList->get(i)).second);
		}
		delete(tempList);
	}
	
  public:
	hashTable() {
		capacity = 1024; // initial capacity
		table = new List<K, V>*[capacity];
	}
	
	int getSize() {
		return size;
	}
	
	bool lookup(K key) {
		__transaction_relaxed{
			int bucketIndex = hash(hashCode(key));
			if (table[bucketIndex] != NULL) {
				List<K, V>* bucket = table[bucketIndex];
				if (bucket->lookup(key))
					return true;
			}
		}
		return false;
	}
	
	bool insert(K key, V val) {
		__transaction_relaxed{
			if (lookup(key))
				return false;
				
			int bucketIndex = hash(hashCode(key));
			
			if (table[bucketIndex] != NULL) {
				if (table[bucketIndex]->getSize() >= 8) {
					rehash();
					bucketIndex = hash(hashCode(key));
				}
			}

			if (table[bucketIndex] == NULL) {
				table[bucketIndex] = new List<K, V>();
			} // else, table[bucketIndex] is existing
			
			table[bucketIndex]->insert(key, val);
			size ++;
		}
		return true;
	}
	
	bool remove(K key) {
		__transaction_relaxed{
			if (!lookup(key))
				return false;
				
			int bucketIndex = hash(hashCode(key));
			if (table[bucketIndex] != NULL) {
				List<K, V>* bucket = table[bucketIndex];
				bucket->remove(key);
			}
			size --;
		}
		return true;
	}
};