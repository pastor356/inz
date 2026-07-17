#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFINE_ARR(T, NAME) \
typedef struct {    \
    T* arr;         \
    size_t sz;      \
    size_t cap;     \
} NAME##_t;         \
					\
static int reserve_##NAME(NAME##_t *a, size_t nelems) { \
    if(a == NULL || nelems == 0) { \
		printf("%s: bad params\n", __func__);	\
        return(-1);								\
    }											\
												\
    T* newarr = (T*) malloc(nelems * sizeof(T));\
    if(!newarr) {								\
        perror("malloc() in reserve()");		\
    }											\
												\
    if(a -> arr != NULL) {						\
        if(nelems < a -> sz) return(-2);		\
        for(size_t i = 0; i < a -> sz; ++i) {	\
            newarr[i] = a -> arr[i];			\
        }										\
												\
    }											\
												\
    a -> cap = nelems;							\
	free(a -> arr);								\
    a -> arr = newarr;							\
    return(0);									\
}												\
												\
static int init_arr_##NAME(NAME##_t* a, T* srcarr, size_t arrsize) { \
	if(a == NULL || srcarr == NULL) { \
		printf("%s: bad params\n", __func__); \
		return(-1); \
	} \
	\
	a -> arr = NULL; \
	a -> sz = arrsize; \
	reserve_##NAME(a, 2*arrsize); \
	for(size_t i = 0; i < arrsize; ++i) { \
		a -> arr[i] = srcarr[i]; \
	} \
	\
	return(0); \
}\
\
\
static int push_back_##NAME(NAME##_t* a, T elem) { \
	if(a == NULL) { \
		printf("%s: bad params\n", __func__); \
		return(-1); \
	} \
	\
	if(a -> sz == a -> cap) { \
		if(reserve_##NAME(a, 2*a->cap) < 0) {	\
			printf("%s: malloc() failed in reserve()\n", __func__); \
		} \
	} \
	\
	a -> arr[a->sz++] = elem; \
	\
	return(0); \
} \
\
\
static int pop_back_##NAME(NAME##_t* a) { \
	if(a == NULL) { \
		printf("%s: bad params\n", __func__); \
		return(-1);		\
	}					\
	a -> sz--;			\
	return(0);			\
} \
\
\
static int swap_back_##NAME(NAME##_t* a, int idx) { \
	if(a == NULL || idx < 0 || a->arr == NULL) { \
		printf("%s: bad params\n", __func__); \
		return(-1); \
	} \
	\
	a->arr[idx] = a->arr[a->sz - 1] ^ a->arr[idx]; \
	a->arr[a->sz - 1] = a->arr[a->sz - 1] ^ a->arr[idx]; \
	a->arr[idx] = a->arr[a->sz - 1] ^ a->arr[idx]; \
	\
	return(0); \
} \
\
\
static int destroy_arr_##NAME(NAME##_t* a) { \
	if(a == NULL || a->arr == NULL) { \
		printf("%s: bad params\n", __func__); \
		return(-1); \
	} \
	\
	free(a->arr); \
	a -> arr = NULL; \
	a->sz = 0; \
	a->cap = 0;\
	\
	return(0); \
}

#define reserve(NAME) reserve_##NAME
#define push_back(NAME) push_back_##NAME
#define pop_back(NAME) pop_back_##NAME
#define swap_back(NAME) swap_back_##NAME
#define destroy_arr(NAME) destroy_arr_##NAME


/*		   T, NAME */
DEFINE_ARR(int, int);
DEFINE_ARR(char, char);
/* NAME is used to create siganature of generated funcs */

int main(void) {
	int arr[5] = {1, 2, 3, 4, 5};	
	int_t a;
	init_arr_int(&a, arr, 5);
	for(size_t i = 0; i < a.sz; ++i) {
		printf("%d | ", a.arr[i]);
	}
	printf("\n");

	int val = 33;
	push_back_int(&a, val);
	for(size_t i = 0; i < 30; ++i) {
		push_back_int(&a, val);
	}

	printf("NEW MACRO\n");
	push_back(int)(&a, 8);
	for(size_t i = 0; i < a.sz; ++i) {
		printf("%d | ", a.arr[i]);
	}
	printf("\n");
	printf("NEW MACRO\n");


	for(size_t i = 0; i < a.sz; ++i) {
		printf("%d | ", a.arr[i]);
	}
	printf("\n");

	pop_back_int(&a);
	for(size_t i = 0; i < a.sz; ++i) {
		printf("%d | ", a.arr[i]);
	}

	push_back_int(&a, 312);

	printf("\n");
	for(size_t i = 0; i < a.sz; ++i) {
		printf("%d | ", a.arr[i]);
	}
	printf("\n");

	swap_back_int(&a, 3);
	for(size_t i = 0; i < a.sz; ++i) {
		printf("%d | ", a.arr[i]);
	}
	printf("\n");


	char* str = "blalba";
	char_t ba;
	init_arr_char(&ba, str, strlen(str) + 1);

	printf("\n%s is char*\n", ba.arr);

	destroy_arr(int)(&a);
	destroy_arr_char(&ba);
	exit(EXIT_FAILURE);
}
