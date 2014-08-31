#ifndef LIST_HPP__
#define LIST_HPP__

#include <unistd.h>

#define MAX_DELAY 1024
#define MIN_DELAY 2

/**
 *  A simple sorted linked list holding integers.  Note that we insert a
 *  "dummy" node at the head of each list, which makes the rest of the code
 *  much simpler.
 */
class List
{
    /**
     *  Type for the nodes of the list
     */
    struct Node
    {
        /**
         *  The value stored at this node
         */
        int   val;

        /**
         *  The next entry
         */
        Node* next;

        /**
         *  Basic constructor
         */
        Node(int _val = -1, Node* _next = NULL) : val(_val), next(_next) { }
		
		// volatile for good reading
		volatile bool state = false;
	
		void lock() {
			int limit = MIN_DELAY;
			int delay;
			while (true) {
				while (state) {};
				if (!__sync_lock_test_and_set(&state, true)) {
					return;
				} else {
					delay = rand() % limit;
					if (limit <= MAX_DELAY) {
						limit *= 2;
					}
					sleep(delay/1000);
				}
			}
		}
		
		void unlock() {
			state = false;
		}
    };

    /**
     *  The head of the list
     */
    Node* sentinel;

  public:

    /**
     * constructor just makes a sentinel for the data structure
     */
    List() : sentinel(new Node()) { }

    /**
     * simple sanity check: make sure all elements of the list are in sorted
     * order
     */
    bool isSane(void) const
    {
        Node* prev = sentinel;
        Node* curr = prev->next;

        while (curr != NULL) {
            if (prev->val >= curr->val)
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
    void insert(int val)
    {
        // traverse the list to find the insertion point
        sentinel->lock();
		Node* prev = sentinel;
        Node* curr = prev->next;
		
        while (curr != NULL) {
			curr->lock();
            if (curr->val >= val)
                break;
			prev->unlock();
            prev = curr;
            curr = prev->next;
        }

        // now insert new_node between prev and curr
        //
        // NB: if the test fails, it means we quit the above loop on account
        // of /finding/ val, in which case we just exit
        if (!curr || ((curr->val) > val)) {
            // create the new node
            Node* i = (Node*)malloc(sizeof(Node));
            i->val = val;
            i->next = curr;
            // insert it
            prev->next = i;
        } 
		
		if (curr != NULL)
			curr->unlock();
		prev->unlock();
    }

    /**
     * lookup method: just traverse the list in order, and see if we find the
     * val
     */
    bool lookup(int val)
    {
		sentinel->lock();
        Node* prev = sentinel;
        Node* curr = prev->next;
		
        while (curr != NULL) {
			curr->lock();
            if (curr->val >= val)
                break;	
			prev->unlock();
            prev = curr;
            curr = prev->next;
        }

		bool result = ((curr != NULL) && (curr->val == val));
		if (curr != NULL)
			curr->unlock();
		prev->unlock();
		
        return result;
    }

    /**
     *  To remove from the list, we need to keep a pointer to the previous
     *  node, too.  Note that this is much easier on account of us having a
     *  sentinel
     */
    void remove(int val)
    {
        // find the node whose val matches the request
		sentinel->lock();
		Node* prev = sentinel;
        Node* curr = prev->next;
		
        while (curr != NULL) {
			curr->lock();
            // if we find the node, disconnect it and end the search
            if (curr->val == val) {
                prev->next = curr->next;
			    curr->unlock(); // other threads are not able to visit it since the prev is still locked
                // delete curr...
                free(curr);
				break;
            }
            else if (curr->val > val) {
                // this means the search failed
				break;
            }
            // advance one node
			prev->unlock();
            prev = curr;
            curr = prev->next;
        }
		if (curr != NULL)
			curr->unlock();
		prev->unlock();
    }
};

#endif // LIST_HPP__
