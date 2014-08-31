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
		
		volatile bool marker = false;		
		
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
	
	bool validate(Node* prev, Node* curr) {
		return (!prev->marker) && (!curr->marker) && (prev->next == curr);
	}

    /**
     * insert method; find the right place in the list, add val so that it is
     * in sorted order; if val is already in the list, exit without inserting
     */
    void insert(int val)
    {
		while (true) {
			// traverse the list to find the insertion point
			Node* prev = sentinel;
			Node* curr = prev->next;
			
			while (curr != NULL) {
				if (curr->val >= val)
					break;
				prev = curr;
				curr = prev->next;
			}
			prev->lock();
			
			if (curr == NULL) {
				Node* i = (Node*)malloc(sizeof(Node));
				i->val = val;
				i->next = curr;
				// insert it
				prev->next = i;
			} else {
				curr->lock();				
				if (validate(prev, curr)) {
					if ((curr->val) > val) {
						// create the new node
						Node* i = (Node*)malloc(sizeof(Node));
						i->val = val;
						i->next = curr;
						// insert it
						prev->next = i;
					}
					curr->unlock();
					prev->unlock();
					break;
				}
			}
			if (curr != NULL)
				curr->unlock();
			prev->unlock();
		}// end of the while loop
    }

    /**
     * lookup method: just traverse the list in order, and see if we find the
     * val
     */
    bool lookup(int val)
    {
        Node* curr = sentinel;	
        while (curr != NULL) {
            if (curr->val >= val)
                break;	
            curr = curr->next;
        }
		bool result = ((curr != NULL) && (curr->val == val) && (!curr->marker));
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
		while (true) {
			Node* prev = sentinel;
			Node* curr = prev->next;
			
			while (curr != NULL) {
				if (curr->val >= val) {
					// this means the search failed
					break;
				}
				// advance one node
				prev = curr;
				curr = prev->next;
			}
			prev->lock();
			
			if (curr != NULL) {
				curr->lock();	
				if (validate(prev, curr)) {
					if ((curr->val) == val) {
						// if we find the node, disconnect it and end the search	
						curr->marker = true;
						prev->next = curr->next;
					}
					curr->unlock();
					prev->unlock();
					break;
				}
			} else {
				prev->unlock();
				break;
			}			
			if (curr != NULL)
				curr->unlock();
			prev->unlock();
		}
    }
};

#endif // LIST_HPP__
