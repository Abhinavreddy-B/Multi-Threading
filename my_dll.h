#include "node.h"

#ifndef _MY_DLL_H_
#define _MY_DLL_H_


/**
 * @brief the structure to hold the head and tail of a doubly linked list
 * @param root the pointer to the first node of the list
 * @param tail the pointer to the last node of the list
*/
typedef struct my_dll my_dll;


/**
 * @brief the structure to hold the head and tail of a doubly linked list
 * @param root the pointer to the first node of the list
 * @param tail the pointer to the last node of the list
*/
struct my_dll{
    nodeptr root;
    nodeptr tail;
};


/**
 * @brief creates an empty doubly linked list
 * @return A copy of the created List
*/
my_dll CreateList(void);


/**
 * @brief checks if list is empty
 * @param _L the structure of the list
 * @return 0 if non-empty, otherwise returns 1
*/
int IsEmpty(const my_dll _L);


/**
 * @brief adds node at the end
 * @param _x the new data to be inserted
 * @param _L pointer to the structure of the list
*/
void insert(my_dll* _L, ListElement _x);


/**
 * @brief adds node at the specified position(zero-indexed)
 * @param _L pointer to the structure of the list
 * @param _x the new data to be inserted
 * @param _i the position after which the node is to be inserted
*/
void insert_at(my_dll* _L,ListElement _x,int _i);


/**
 * @brief node node at the specified position(zero-indexed)
 * @param _L pointer to the structure of the list
 * @param _i the position of the node to be deleted
*/
void delete(my_dll* _L,int _i);


/**
 * @brief finds for the first occurence of a data
 * @param _L pointer to the structure of the list
 * @param _x the new data to be found
 * @return returns the index (zero-indexed) of the first occurence of _x, if not found returns -1
*/
int Find(const my_dll* _L,ListElement _x);


/**
 * @brief delete the elements at the odd indexes
 * @param _L pointer to the structure of the list
*/
void prune(my_dll* _L);


/**
 * @brief Print all the elements of the linked list starting from index 0.
 * @param _L structure of the list
*/
void print(const my_dll _L);


/**
 * @brief Print all the elements of the linked list in reverse order
 * @param _L structure of the list
*/
void print_reverse(const my_dll _L);


/**
 * @brief to find the size of the list
 * @param _L structure of the list
 * @return the size of the list
*/
unsigned int get_size(const my_dll _L);

#endif