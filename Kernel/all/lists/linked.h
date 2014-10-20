/*
 * linked.h
 *
 *  Created on: 18 Oct 2014
 *      Author: blake
 */

#ifndef LINKED_H_
#define LINKED_H_

typedef struct linked_list {
	void* payload;
	struct linked_list* next;
} linked_list_t;

linked_list_t* linkedListPrepend(linked_list_t* list, void* payload);
linked_list_t* linkedListAppend(linked_list_t* list, void* payload);
linked_list_t* linkedListEnd(linked_list_t const* list);
linked_list_t* linkedListRemove(linked_list_t* item, linked_list_t* list);
linked_list_t* linkedListNext(linked_list_t* item);

#endif /* LINKED_H_ */
