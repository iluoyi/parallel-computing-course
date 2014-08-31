 #include <utility>
 #include <iostream>

using std::cout;
using std::endl;
using std::pair;
 
 /**
     *  Type for the nodes of the list
     */
template <class K, class V>
struct Node
{
	/**
    *  The value stored at this node
    */
	pair<K,V> nodeData;

	/**
    *  The next entry
    */
	Node* next;

	/**
	*  Basic constructor
    */
	Node(): next(NULL) { }
	Node(K k, V v): nodeData(k, v), next(NULL) { }
		
	K key(){
		return nodeData.first;
	}
	V val(){
		return nodeData.second;
	}
};
	
template <class K, class V>
class List
{
    /**
     *  The head of the list
     */
    Node<K, V>* sentinel;
	
	int size;

  public:

    /**
     * constructor just makes a sentinel for the data structure
     */
    List() : sentinel(new Node<K, V>()), size(0) { }
	
	int getSize() {
		return size;
	}

    /**
     * simple sanity check: make sure all elements of the list are in sorted
     * order
     */
    bool isSane(void) const
    {
        Node<K, V>* prev = sentinel;
        Node<K, V>* curr = prev->next;

        while (curr != NULL) {
            if ((prev->nodeData).first >= (curr->nodeData).first)
                return false;
            prev = curr;
            curr = curr->next;
        }
        return true;
    }

    /**
     * insert method; find the right place in the list, add val so that it is
     * in sorted order; if val is already in the list, exit without inserting
     */
    void insert(K key, V val)
    {
        // traverse the list to find the insertion point
        Node<K, V>* prev =sentinel;
        Node<K, V>* curr = prev->next;
        while (curr != NULL) {
            if ((curr->nodeData).first >= key)
                break;
            prev = curr;
            curr = prev->next;
        }

        // now insert new_node between prev and curr
        //
        // NB: if the test fails, it means we quit the above loop on account
        // of /finding/ val, in which case we just exit
        if (!curr || ((curr->nodeData).first > key)) {
            // create the new node
			Node<K,V> * i = new Node<K,V>(key, val);
            i->next = curr;
            // insert it
            prev->next = i;
			size ++;
        }
    }

    /**
     * lookup method: just traverse the list in order, and see if we find the
     * val
     */
    bool lookup(K key)
    {
        Node<K, V>* curr = sentinel->next;

        while (curr != NULL) {
            if ((curr->nodeData).first >= key)
                break;
            curr = curr->next;
        }

        return ((curr != NULL) && ((curr->nodeData).first == key));
    }

    /**
     *  To remove from the list, we need to keep a pointer to the previous
     *  node, too.  Note that this is much easier on account of us having a
     *  sentinel
     */
    void remove(K key)
    {
        // find the node whose val matches the request
        Node<K, V>* prev = sentinel;
        Node<K, V>* curr = prev->next;
        while (curr != NULL) {
            // if we find the node, disconnect it and end the search
            if ((curr->nodeData).first == key) {
                prev->next = curr->next;
                // delete curr...
                free(curr);
				size --;
                break;
            }
            else if ((curr->nodeData).first > key) {
                // this means the search failed
                break;
            }
            // advance one node
            prev = curr;
            curr = prev->next;
        }
    }
	
	pair<K, V> get(int index) {
        Node<K, V>* curr = sentinel->next;
		for (int i = 0; i < index; i ++) {
            curr = curr->next;
		}
		return curr->nodeData;
	}
};