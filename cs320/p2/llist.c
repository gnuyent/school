#include "llist.h"

Link strToList(char s[]) {
    Link head = malloc(sizeof(Link));
    while (s[0] != '\0') {
        head->data = s[0];
        printf("%s: %c\n", s, s[0]);
        char *sp = s;
        sp++;
        head->next = strToList(sp);
        return head;
    }
    return NULL;
}

int size(Link head) {
    if (head->next == NULL) {
        return 1;
    } else {
        return 1 + size(head->next);
    }
}

Link find(Data c, Link head) {
    /* reached end */
    if (head == NULL) {
        return NULL;
    } /* match */
    else if (head->data == c) {
        return head;
    /* continue searching */
    } else {
        return find(c, head->next);
    }
}

void insert(Link p1, Link p2, Link q) {
    p1->next = q;
    q->next = p2;
}

void delete (Link p, Link q) { p->next = q->next; }

void deleteList(Link head) {
    if (head->next == NULL) {
        printf("\ndeleting %c\n", head->data);
        free(head);
    } else {
        printf("\ndeleting %c", head->data);
        deleteList(head->next);
        free(head);
    }
}

void print(Link head) {
    if (head->next == NULL) {
        printf("%c", head->data);
    } else {
        printf("%c ", head->data);
        print(head->next);
    }
}
