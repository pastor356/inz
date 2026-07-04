#include "test_h/test.h"

void test_checkifexists(void) {
	int size = 5;
	tinfo_t* topics[size];
	tlist_t* root = create_node(NULL, NULL, NULL);

	root->topic = create_tinfo();
	init_tinfo(root->topic, "root_topic!");

	char buf[VBUFSIZ];
	for(size_t i = 0; i < size; ++i) {
		topics[i] = create_tinfo();
		sprintf(buf, "topic_no %ld", i);
		init_tinfo(topics[i], buf);
		push_back(&root, topics[i]);
	}

	print_list(&root, "test no 4");

	int ex = checkif_topic_exists(root, "topic_no 4");
	printf("%d idx of topic\n", ex);

	destroy_tinfo(root->topic);
	for(size_t i = 0; i < size; ++i) {
		destroy_tinfo(topics[i]);
	}
	free_list(&root);
	return;
}
