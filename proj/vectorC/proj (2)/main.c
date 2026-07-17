#include "vector.h"
#include "test.h"

void inc_vec(void* elem) {
    int *val = (int*)elem;
    (*val)++;
}

void func(void);

int main() {
    
    
    /* seg. fault przy wywołaniu tych f-cji 
    test_reserve();

    test_resize();
    
    */
     
    vector_t* obj;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};

    vector_init(obj, arr, sizeof(arr)/sizeof(int), sizeof(int));

    printf("\n===== RESERVE_TEST =======");
    show_int(obj);  
    printf("obj->capacity = %zu\nreserve(obj, 33);\n", obj -> capacity);
    reserve(obj, 33);
    printf("obj->capacity = %zu\n", obj -> capacity);
    show_int(obj);  
    printf("===== END_RESERVE_TEST =======\n\n");



    printf("\n\n===== RESIZE_TEST =======");
    int value1 = 42;
    int value2 = 24;
    show_int(obj);
    printf("obj->size = %zu\nresize(obj, 32, &value, sizeof(int));\n", obj->size);

    resize(obj, 32, &value1, sizeof(int));
    printf("obj->size = %zu\n", obj -> size);

    show_int(obj);

    printf("resize(obj, 35, &value2, sizeof(int));\n");
    
    resize(obj, 35, &value2, sizeof(int));
    printf("obj->size = %zu\n", obj -> size);
    
    show_int(obj);

    printf("===== END_RESIZE_TEST =====\n\n");

    
    printf("\n\n===== HANDLER_TEST =====\n");
    ElemHandler handler = inc_vec;

    processVec(obj, handler);
    printf("ElemHandler handler = inc_vec;\nprocessVec(obj, handler);\n"); 
    show_int(obj);

    printf("===== END_HANDLER_TEST =====\n\n");
    
    /*
     * ElemHandler handler = show_int; // show_int => test.h
     * processVec(obj, handler);
     */

    func(); 
    exit(EXIT_SUCCESS);

}


void func(void) {
    printf("\n");
    vector_t *obj;
    double arr[] = {1.2, 3.3, 4.5, 1.6};

    vector_init(obj,arr, sizeof(arr)/sizeof(double), sizeof(double));
    
    double* elem;

    for(size_t i = 0; i < obj -> size; i++){
        elem = (double*)((char*)obj->array + i*obj->elem_size);
        printf("%f | ", *elem);
    }
    printf("\n");
}    
