/*
 * linked.c
 *
 *  Created on: 18 Oct 2014
 *      Author: blake
 */

#include <lists/linked.h>
#include <stdlib.h>

linked_list_t* linkedListCreate(void* payload) {
	linked_list_t* created = (linked_list_t*) malloc(sizeof(linked_list_t));
	created->next = 0;
	created->payload = payload;
	return created;
}

linked_list_t* linkedListPrepend(linked_list_t* list, void* payload) {
	linked_list_t* head = linkedListCreate(payload);
	head->next = list;
	return head;
}

linked_list_t* linkedListRemove(linked_list_t* item, linked_list_t* list) {

	if (item == list) {
		linked_list_t* head = item->next;
		free(item);
		return head;
	}

	for (linked_list_t* iter = list; iter != 0; item = item->next) {
		if (iter->next == item) {
			iter->next = item->next;
			free(item);
			return list;
		}
	}

	return list;
}

linked_list_t* linkedListNext(linked_list_t* item) {
	return item->next;
}
