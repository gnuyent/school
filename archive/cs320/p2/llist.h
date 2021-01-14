#include <stdio.h>
#include <stdlib.h>

typedef char Data;   /* store char in list */

struct llist {
    Data data;
    struct llist *next;
};

typedef struct llist Node;
typedef Node * Link;

/* strToList
 *    Creates a dynamic list from a string (char[] in C)
 *       Prints current string with each call to strToList
 *    Returns head of new list
 */
Link strToList(char s[]);

/* size
 *    Recursively counts elements in the list
 *    Returns int
 */
int size(Link head);

/* find
 *    Recursively iterates through the list to locate the Data element
 *    Returns NULL if list is empty or it reaches end of list (NULL)
 *    Returns Link a pointer to the Data element if found
 */
Link find(Data c, Link head);

/* insert
 *    Adds element q to the list after element p1 and before element p2
 *    Returns void
 *    Assumptions: arguments passed to insert are valid Link elements
 */
void insert(Link p1, Link p2, Link q);

/* delete
 *    Removes element q whose predecessor is element p
 *    Returns void (q contains a pointer to the removed element)
 */
void delete(Link p, Link q);

/* deleteList
 *    Recursively deletes an element from the list, freeing memory
 *       as each element is removed. Prints the element being deleted.
 *    Returns void
 */
void deleteList(Link head);

/* print
 *    Recursively prints the Data elements in the list
 *    Returns void
 */
void print(Link head);
