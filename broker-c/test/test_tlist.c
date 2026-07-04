#include "header.h"
#include "list_tinfo.h"

void test_1(void);
void test_2(void);

void print_list(tlist_t** root, const char* what) {
	if(!root || !(*root)) {
		printf("%s: bad params\n", __func__);
		return;
	}

	static int test_no = 0;
	printf("### TEST NO. %d: %s ###\n", test_no, what);

	tlist_t* tmp = *root;
	if(tmp->topic) printf("\ttopic: %s\n\t%ld is fdsize\n\t", tmp->topic->topic_name, tmp->topic->fdarr->sz);
	else tmp = tmp->next;

	for(size_t i = 0; i < tmp->topic->fdarr->sz; ++i) {
		printf(" %d |", tmp->topic->fdarr->fdarr[i].fd);
	}
	printf("\n\n");

	while(tmp->next != NULL) {
		tmp = tmp->next;
		printf("\ttopic: %s\n\t%ld is fdsize\n\t", tmp->topic->topic_name, tmp->topic->fdarr->sz);
		for(size_t i = 0; i < tmp->topic->fdarr->sz; ++i) {
			printf(" %d |", tmp->topic->fdarr->fdarr[i].fd);
		}
		printf("\n\n");
	}

	printf("\n\n");
	++test_no;

	return;
}



void test_2(void) {
	tinfo_t topic1;
	init_tinfo(&topic1, "fancy_topic1");
	for(size_t i = 0; i < 11; ++i) {
		push_backf(topic1.fdarr, 2 * i + 1);
	}

	tinfo_t topic2;
	init_tinfo(&topic2, "fancy_topic2");
	for(size_t i = 0; i < 5; ++i) {
		push_backf(topic2.fdarr, 2 * i);
	}

	tlist_t* root = create_node(&topic1, NULL, NULL);
	push_back(&root, &topic2);

	print_list(&root, "created new list");


	tinfo_t topic3;
	init_tinfo(&topic3, "topc3");
	for(size_t i = 0; i < 12; ++i) {
		push_backf(topic3.fdarr, i);
	}
	push_back(&root, &topic3);


	tinfo_t topic4;
	init_tinfo(&topic4, "t4");
	for(size_t i = 0; i < 12; ++i) {
		push_backf(topic4.fdarr, 3*i);
	}
	push_back(&root, &topic4);

	tlist_t* tmp = root;
	while(tmp->next != NULL) {
		tmp = tmp->next;
	}




	remove_this(&root, &root);
	print_list(&root, "remove_this(root branch) test");

	remove_this(&root, &tmp);
	print_list(&root, "remove_this(tail branch) test");

	remove_this(&root, &root->next);
	print_list(&root, "remove_this(middle branch) test");


	destroy_tinfo(&topic1);
	destroy_tinfo(&topic2);
	destroy_tinfo(&topic3);
	destroy_tinfo(&topic4);

	free_list(&root);
	return; 
}






void test_1(void) {
	tinfo_t topic;
	init_tinfo(&topic, "newtopic!");

	tinfo_t topic2;
	init_tinfo(&topic2, "topic2");

	tinfo_t topic3;
	init_tinfo(&topic3, "topic3");

	tlist_t *root = create_node(&topic, NULL, NULL);

	for(size_t i = 0; i < 20; ++i) {
		push_backf(topic.fdarr, i);
	}


	for(size_t i = 0; i < 10; ++i) {
		push_backf(topic2.fdarr, i);
	}

	push_back(&root, &topic2);
	print_list(&root, "push_back() test");
	push_back(&root, &topic3);

	push_backf(topic3.fdarr, 141);
	push_backf(topic3.fdarr, 142);

	print_list(&root, "push_backf() test");

	remove_at(&root, 0);
	print_list(&root, "remove_at(0) test");

	remove_at(&root, 1);
	print_list(&root, "remove_at(1) test");

	remove_at(&root, 0);
	print_list(&root, "remove_at(0) test (error expected)");

	destroy_tinfo(&topic);
	destroy_tinfo(&topic2);
	destroy_tinfo(&topic3);

	free_list(&root);

	return;
}


void test_fabric_tinfo(void) {
	tinfo_t* topic = create_tinfo();
	if(!topic) abort();

	init_tinfo(topic, "newtopic");

	tinfo_t* topi2 = create_tinfo();
	init_tinfo(topi2, "newtopic2");


	tlist_t* root = create_node(topic, NULL, NULL);

	for(size_t i = 0; i < 3; ++i) push_backf(topic->fdarr, 3*i);
	push_back(&root, topi2);
		
	for(size_t i = 0; i < 3; ++i) push_backf(topi2->fdarr, 2*i);
	print_list(&root, "fabric test");

	destroy_tinfo(topic);
	destroy_tinfo(topi2);

	free_list(&root);

	return;
}


