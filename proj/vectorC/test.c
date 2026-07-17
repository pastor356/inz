#include "test.h"

void test_reserve() {
    vector_t obj;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};

    vector_init(&obj, arr, sizeof(arr)/sizeof(int), sizeof(int));

    printf("\n===== RESERVE_TEST =======");
    show_int(&obj);  
    printf("obj->capacity = %zu\nreserve(obj, 33)\n", obj.capacity);
    reserve(&obj, 33);
    printf("obj->capacity = %zu\n", obj.capacity);
    show_int(&obj);  
    printf("===== END_RESERVE_TEST =======\n\n");

    exit(EXIT_SUCCESS);
}


void test_resize() {
    vector_t *obj;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};

    vector_init(obj, arr, sizeof(arr)/sizeof(int), sizeof(int));
    
    printf("\n\n===== RESIZE_TEST =======");
    int value1 = 42;
    int value2 = 24;
    show_int(obj);
    printf("obj->size = %zu\nresize(obj, 32, &value, sizeof(int))\n", obj -> size);

    resize(obj, 32, &value1, sizeof(int));
    printf("obj->size = %zu\n", obj -> size);

    show_int(obj);

    printf("resize(obj, 35, &value2, sizeof(int))\n");
   
    resize(obj, 35, &value2, sizeof(int));
    printf("obj->size = %zu\n", obj -> size);
    
    show_int(obj);

    printf("===== END_RESIZE_TEST =====\n\n");

    exit(EXIT_SUCCESS);
}


void show_int(void* obj) {
    if(obj == NULL || obj == NULL) {
        fprintf(stderr, "show_int(): bad params\n");
        return;
    }

    vector_t* vec = (vector_t*) obj;

    int* arr = (int*) vec -> array;

    printf("\n");
    for(size_t i = 0; i < vec->size; i++) {
        printf("%d | ", arr[i]);
    }
    printf("\n");
}


