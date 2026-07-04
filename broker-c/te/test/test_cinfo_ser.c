#include "client_info.h"
#include "./test_h/test.h"


void print_cinfo(cinfo_t* client) {
	if(client == NULL) {
		return;
	}


	static int test_no = 0;

	printf("\ntest_no: %d\n", test_no);
	printf("client role: %d\n", client->role);
	printf("client topic: %s\n", client->topic_name);

	return;
}


void test_cinfo_ser(void) {
	cinfo_t* client = NULL;
	char serbuf[VBUFSIZ];

	client = create_cinfo();


	if(init_cinfo(client, "newtopic", PROD) < 0) {
		printf("init_cinfo() failed\n");
		exit(EXIT_FAILURE);
	}


	serialize_cinfo(client, serbuf);

	printf("%s is serialized buf\n", serbuf);

	client->role = 0;
	memset(client->topic_name, 0, VBUFSIZ);

	deserialize_cinfo(client, serbuf);

	print_cinfo(client);

	destroy_cinfo(client);
	return;
}
