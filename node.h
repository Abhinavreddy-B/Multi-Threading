#ifndef _NODE_H_
#define _NODE_H_

#include <stdlib.h>

/**
 * @brief just defining this for code reusability
*/
typedef int ListElement;

/**
 * @brief a basic node that can be used in a doubly linked list
 * @param data it contains the data to be present in that node
 * @param prev the pointer to the previous node
 * @param next the pointer to the next node
*/
typedef struct _node node;


/**
 * @brief a pointer to a node 
*/
typedef struct _node* nodeptr;


/**
 * @brief a basic node that can be used in a doubly linked list
 * @param data it contains the data to be present in that node
 * @param prev the pointer to the previous node
 * @param next the pointer to the next node
*/
struct _node{
    ListElement data;
    struct _node* prev;
    struct _node* next;
};


/**
 * @brief allocate space for a single node
 * @param _x the data to be present in new node
 * @return pointer to the created new node
*/ 
nodeptr MakeNode(const ListElement _x);

#endif