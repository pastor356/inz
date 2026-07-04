#include "test_h/test.h"


int print_fdarr(fdarr_t* arr) {

	static int test_no = 0;


	printf("\n\ntest_no: %d\narr size: %ld\narr cap: %ld\n",
			test_no, arr->sz, arr->cap);

	for(size_t i = 0; i < arr->sz; ++i) {
		printf(" %d |", arr->fdarr[i].fd);
	}
	++test_no;

	printf("\n");

	return(0);
}

int print_fd(tinfo_t* topic) {
	if(!topic) {
		return(-1);
	}
	static int test_number = 0;

	for(size_t i = 0; i < topic->fdarr->sz; ++i) {
		printf("%d is descriptor in test no. %d\n", topic->fdarr->fdarr[i].fd, 
				test_number);
	}

	++test_number;
	printf("\n");
	return(0);
}


void test_tinfo_fdarr(void) {
	printf("\n");
	tinfo_t *tinfo = create_tinfo();


	init_tinfo(tinfo, "new_topic_lalal");
	push_backf(tinfo->fdarr, 3);
	print_fd(tinfo);

	size_t size = 5;
	struct pollfd newfd[size];
	for(size_t i = 0; i < size; ++i) {
		newfd[i] = (struct pollfd) {.fd = 2*i, .events = POLLIN};
		push_backp(tinfo->fdarr, &newfd[i]);

	}

	print_fd(tinfo);
	swap_back(tinfo->fdarr, 3);
	print_fd(tinfo);
	pop_backp(tinfo->fdarr, NULL);
	print_fd(tinfo);

	struct pollfd tmp; 
	pop_backp(tinfo->fdarr, &tmp);
	printf("poped obj: %d and %d\n", tmp.fd, tmp.events);

	destroy_tinfo(tinfo);
	exit(EXIT_SUCCESS);
}


void test_cpy_arr(void) {
	fdarr_t* arr1= NULL;
	fdarr_t* arr2 = NULL;

	size_t size = 4;

	init_fdarr(&arr1, 10);
	init_fdarr(&arr2, 10);

	for(size_t i = 0; i < size; ++i) {
		push_backf(arr1, i * 2);
	}

	for(size_t i = 0; i < size; ++i) {
		push_backf(arr2, i * 3);
	}

	print_fdarr(arr2);

	copy_arr(arr1, arr2);

	print_fdarr(arr2);

	
	destroy_fdarr(arr1);
	destroy_fdarr(arr2);
	return;
}









