#pragma once

#include "header.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct topic_list {
	tinfo_t* topic;
	struct topic_list* next;
	struct topic_list* prev;
} tlist_t;

tlist_t* create_node(const tinfo_t* t, tlist_t* p, tlist_t* n);
int push_back(tlist_t** root, tinfo_t* topic);
int pop_back(tlist_t** root, tinfo_t* poped_topic);
int remove_at(tlist_t** root, int idx);
int remove_this(tlist_t** root, tlist_t** node);
int release_node(tlist_t** node);
int free_list(tlist_t** root); 

// returns index of list in case of existing topic, 
// returns < 0 val in case of not existing
int checkif_topic_exists(tlist_t* root, const char* topic);
