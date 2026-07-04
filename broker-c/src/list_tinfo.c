#include "header.h"
#include "fdarr.h"
#include "list_tinfo.h"


tlist_t* create_node(const tinfo_t* t, tlist_t* p, tlist_t* n) {
	tlist_t* newnode = (tlist_t*) malloc(sizeof(tlist_t));
	if(!newnode) {
		printf("%s: malloc() failed\n", __func__);
		return(NULL);
	}

	if(t) newnode->topic = t;
		else newnode->topic = NULL;


	if(p) newnode->prev = p;
		else newnode->prev = NULL;


	if(n) newnode->next = n;
		else newnode->next = NULL;
	
	return(newnode);
}


int push_back(tlist_t** root, tinfo_t* topic) {
	if(!topic || !root) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(*root != NULL) {
		tlist_t* curr = *root;
		while(curr->next != NULL) {
			curr = curr->next;
		}

		curr->next = create_node(topic, curr, NULL);
		if(!curr->next) {
			printf("%s: create_node() failed\n", __func__);
			return(-1);
		}
	} else {
		*root = create_node(topic, NULL, NULL);
		if(!(*root)) {
			printf("%s: create_node() failed\n", __func__);
		}
	}

	return(0);
}


int pop_back(tlist_t** root, tinfo_t* poped_topic) {
	if(!root) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	tlist_t* node_to_pop = NULL;

	if(!(*root)->next) {
		node_to_pop = *root;

	} else {
		tlist_t* curr = *root;
		while(curr->next != NULL) {
			curr = curr->next;
		}

		node_to_pop = curr;
	}

	if(!poped_topic) poped_topic = node_to_pop->topic;

	if(release_node(&node_to_pop) < 0) {
		printf("%s: release_node() failed\n", __func__);
		return(-1);
	}

	return(0);
}

int remove_at(tlist_t** root, int idx) {
	if(!*root || idx < 0) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	tlist_t* node_to_delete = NULL;
	tlist_t* tmp = *root;

	int c = 0;
	while(tmp->next != NULL && c < idx) {
		tmp = tmp->next;
		++c;
	}

	if(c < idx) {
		printf("%s: idx out of range\n", __func__);
		return(-1);
	}

	if(c == 0) { // removing root
			
		// if root->next == NULL
		// root is the only one node in list
		if((*root)->next == NULL) { 
			node_to_delete = *root;
			*root = NULL;
		} else {
			tmp = *root;
			node_to_delete = tmp;
			*root = tmp->next;
		}

	} else {
		
		if(tmp->next == NULL) { // removing tail
			node_to_delete = tmp;
			tmp->prev->next = NULL;
		} else { // removing from middle
			tmp->prev->next = tmp->next;
			tmp->next->prev = tmp->prev;
			node_to_delete = tmp;
		}	

	}

	if(release_node(&node_to_delete) < 0) {
		printf("%s: release_node() failed\n", __func__);
		return(-1);
	}

	return(0);
}



int remove_this(tlist_t** root, tlist_t** node) {
	if(!(*root) || !node || !(*node)) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	tlist_t* tmp = *root;
	if(*root == *node) { // we want to delete root
		tmp = *root;
		if((*root)->next != NULL) {
			(*root) = (*root)->next;
			(*root)->prev = NULL;
		} else {
			(*root) = NULL;
		}

	} else { // not root
		if((*node)->next == NULL) { // node is tail
			tmp = *node;
			(*node)->prev->next = NULL;
		} else { // node is in the middle
			(*node)->prev->next = (*node)->next;
			(*node)->next->prev = (*node)->prev;
			tmp = *node;
		}
	}

	if(release_node(&tmp) < 0) {
		printf("%s: release_node() failed\n", __func__);
		return(-1);
	}

	return(0);
}





int release_node(tlist_t** node) {
	if(!node || !(*node)) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}


	(*node)->prev = NULL;
	(*node)->next = NULL;
	free(*node);

	return(0);
}


int free_list(tlist_t** root) {
	if(!root || !(*root)) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if((*root)->next != NULL) {
		tlist_t* curr = *root;
		tlist_t* next = NULL;

		while(curr->next != NULL) {
			next = curr->next;
			release_node(&curr);
			curr = next;
		}

		release_node(&curr);

	} else {
		release_node(root);
	}

	return(0);
}



int checkif_topic_exists(tlist_t* root, const char* topic) {
	if(!root) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	tlist_t* tmp = root;
	int c = 0;

	while(tmp->next != NULL) {
		if(tmp->topic) {
			if(strcmp(tmp->topic->topic_name, topic) == 0) return(c);
		}
		tmp = tmp->next;
		++c;
	}

	if(tmp->topic) {
		if(strcmp(tmp->topic->topic_name, topic) == 0) return(c);
	}

	return(-1);
}
