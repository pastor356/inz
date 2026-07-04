#include "test_h/test.h"

#define SERBUFSIZ 100

void test_serializer(void) {
	void* buf = init_serbuf(SERBUFSIZ);

	if(serialize_metadata(buf, SERBUFSIZ, (uint32_t)32, (uint32_t)44) < 0) {
		printf("serialize_metadata() fucked up\n");
	}

	uint32_t hsiz, bsiz;

	if(deserialize_metadata(buf, SERBUFSIZ, &hsiz, &bsiz) < 0) {
		printf("deserialize_metadata() fucked up\n");
	}

	printf("deserialized vals: %d and %d\n", hsiz, bsiz);


	char hdr[VBUFSIZ] = "header";
	char body[VBUFSIZ] = "body of msg";

	char newhdr[VBUFSIZ];
	char newbody[VBUFSIZ];

	serialize_data(buf, SERBUFSIZ, hdr, strlen(hdr), body, strlen(body));

	deserialize_data(buf, SERBUFSIZ, newhdr, strlen(hdr), newbody, strlen(body));

	printf("deserialized data: %s\nand\n%s\n", newhdr, newbody);

	if(destroy_serbuf(buf) < 0) {
		printf("destroy_serbuf() fucked up\n");
	}

	return;
}
