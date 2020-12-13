/*
 *   Program 2: Driver for LinkedList
 *   2 October 2020
 *   Program 2
 *   CS 320
 *   Brandon Nguyen
 */
#include "llist.h"

#define INPUT_ERROR -1 /* wrong argument count */
#define FILE_ERROR  -1 /* problem with file */

void printId() { printf("\nProgram 2, cssc2650, Brandon Nguyen\n\n"); }

int main(int argc, char *argv[]) {
    FILE *file;
    char buffer[256];

    /* 1. Output student/account info */
    printId();

    /* 2. Error CLI arguments using perror() */
    if (argc != 2) { /* check if arguments are valid */
        perror("Input file error");
        return INPUT_ERROR;
    }

    /* 3. Open file (error check) using fopen */
    file = fopen(argv[1], "r");

    if (file == NULL) { /* check if file is valid */
        perror("Error opening file");
        return FILE_ERROR;
    }

    /* 4. Read in file content into char buffer and echo buffer */
    while (EOF != fscanf(file, "%[^\n]\n", buffer)) {
        printf("> %s\n", buffer);
    }

    /* 5. Crete list from buffer using strToList */
    Link list = strToList(buffer);

    /* 6. print() the newly created list */
    print(list);

    /* 7. Print size() */
    printf("\n%d\n", size(list));

    /* 8. insert() an 'o' after the letter 't' if 't' is in the list */
    Link t_pos = find('t', list);
    while (t_pos != NULL) {
        Link o_link = malloc(sizeof(Link));
        o_link->data = 'o';
        insert(t_pos, t_pos->next, o_link); /* insert 'o' after 't' */
        t_pos = find('t', t_pos->next);
    }

    /* 10. print() the list */
    print(list);

    /* 9. Print size() */
    printf("\n%d\n", size(list)); /* print size with o*/

    /* 11. delete() the third letter in the list */
    Link second = list->next;
    Link third = list->next->next;
    delete(second, third);
    print(list); /* print list after removing third character */

    /* 12. deleteList() */
    deleteList(list);

    /* 13. fclose() file pointer */
    fclose(file);

    /* 14. return(0) */
    return 0;
}
