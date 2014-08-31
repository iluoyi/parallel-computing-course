#ifndef LIST_HPP__
#define LIST_HPP__

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
        Node* prev =sentinel;
        Node* curr = prev->next;
        while (curr != NULL) {
            if (curr->val >= val)
                break;
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
    }

    /**
     * lookup method: just traverse the list in order, and see if we find the
     * val
     */
    bool lookup(int val)
    {
        Node* curr = sentinel->next;

        while (curr != NULL) {
            if (curr->val >= val)
                break;
            curr = curr->next;
        }

        return ((curr != NULL) && (curr->val == val));
    }

    /**
     *  To remove from the list, we need to keep a pointer to the previous
     *  node, too.  Note that this is much easier on account of us having a
     *  sentinel
     */
    void remove(int val)
    {
        // find the node whose val matches the request
        Node* prev = sentinel;
        Node* curr = prev->next;
        while (curr != NULL) {
            // if we find the node, disconnect it and end the search
            if (curr->val == val) {
                prev->next = curr->next;

                // delete curr...
                free(curr);
                break;
            }
            else if (curr->val > val) {
                // this means the search failed
                break;
            }
            // advance one node
            prev = curr;
            curr = prev->next;
        }
    }
};

#endif // LIST_HPP__
