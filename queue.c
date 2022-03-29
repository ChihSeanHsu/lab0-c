#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (head == NULL) {
        return NULL;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (l == NULL) {
        return;
    }
    element_t *node, *safe;
    list_for_each_entry_safe (node, safe, l, list) {
        list_del_init(&node->list);
        free(node->value);
        free(node);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL) {
        return false;
    }
    element_t *new_node = malloc(sizeof(element_t));
    if (new_node == NULL) {
        return false;
    }
    char *copy = malloc(strlen(s) + 1);
    if (copy == NULL) {
        free(new_node);
        return false;
    }
    strncpy(copy, s, strlen(s) + 1);
    LIST_HEAD(new_list);
    new_node->value = copy;
    new_node->list = new_list;
    list_add(&new_node->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL) {
        return false;
    }
    element_t *new_node = malloc(sizeof(element_t));
    if (new_node == NULL) {
        return false;
    }
    char *copy = malloc(strlen(s) + 1);
    if (copy == NULL) {
        free(new_node);
        return false;
    }
    strncpy(copy, s, strlen(s) + 1);
    LIST_HEAD(new_list);
    new_node->value = copy;
    new_node->list = new_list;
    list_add_tail(&new_node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head)) {
        return NULL;
    }
    element_t *entry = list_first_entry(head, element_t, list);
    if (sp != NULL) {
        *sp = '\0';
        strncat(sp, entry->value, bufsize - 1);
    }
    list_del_init(&entry->list);
    return entry;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head)) {
        return NULL;
    }
    element_t *entry = list_last_entry(head, element_t, list);
    if (sp != NULL) {
        *sp = '\0';
        strncat(sp, entry->value, bufsize - 1);
    }
    list_del_init(&entry->list);
    return entry;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (head == NULL) {
        return 0;
    }
    int count = 0;
    struct list_head *node;
    list_for_each (node, head) {
        count++;
    }
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (list_empty(head)) {
        return false;
    }
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    struct list_head *fast = head->next, *slow = head->next;
    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }
    list_del_init(slow);
    element_t *entry = list_entry(slow, element_t, list);
    free(entry->value);
    free(entry);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    struct list_head *node = head->next, *tmp, *next_node;
    while (node != head) {
        next_node = node->next;
        element_t *curr = list_entry(node, element_t, list),
                  *next_ele = list_entry(next_node, element_t, list);
        if (next_node != head && strcmp(curr->value, next_ele->value) == 0) {
            while (next_node != head &&
                   strcmp(curr->value, next_ele->value) == 0) {
                tmp = next_node->next;
                list_del_init(next_node);

                free(next_ele->value);
                free(next_ele);
                next_node = tmp;
                next_ele = list_entry(next_node, element_t, list);
            }
            list_del_init(node);
            free(curr->value);
            free(curr);
            node = next_node;
        } else {
            node = node->next;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (list_empty(head) || list_is_singular(head)) {
        return;
    }
    struct list_head *first, *second;
    first = head->next;
    second = first->next;
    while (first != head && second != head) {
        list_move(first, second);
        first = first->next;
        second = first->next;
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head == NULL) {
        return;
    }
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        list_move(node, head);
    }
}


/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    struct list_head left, right;
    element_t *pivot;
    element_t *node = NULL, *safe = NULL;

    if (head == NULL || list_empty(head) || list_is_singular(head)) {
        return;
    }

    INIT_LIST_HEAD(&left);
    INIT_LIST_HEAD(&right);
    pivot = list_first_entry(head, element_t, list);
    list_del_init(&pivot->list);

    list_for_each_entry_safe (node, safe, head, list) {
        if (strcmp(node->value, pivot->value) < 0) {
            list_move(&node->list, &left);
        } else {
            list_move(&node->list, &right);
        }
    }
    q_sort(&left);
    q_sort(&right);

    list_add_tail(&pivot->list, &left);
    list_splice_tail(&right, &left);
    list_splice(&left, head);
}
