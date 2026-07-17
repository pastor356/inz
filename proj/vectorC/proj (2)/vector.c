#include "vector.h"

static vreg_t* create_vreg() {
    static vreg_t* reg = NULL;

    if(reg == NULL) {
        reg = malloc(sizeof(vreg_t));
        reg -> reg = malloc(REGSIZ * sizeof(vector_t*));
        atexit(free_vreg);
        reg -> len = 0;
        printf("vreg created\n");
    }

    if(reg == NULL || reg -> reg == NULL) {
        exit(EXIT_FAILURE);
    }

    return reg;
}

void add_to_vreg(vector_t* obj) {
    if(obj == NULL || obj -> array == NULL) {
        exit(EXIT_FAILURE);
    }

    vreg_t* reg = create_vreg();
    
    if(reg -> len <= REGSIZ) {
        reg -> reg[reg->len] = obj;
        reg -> len++;
        printf("\t%p added to vreg\n", reg -> reg[reg -> len - 1]);
    } else {
        fprintf(stderr, "buff is full\n");
        return;
    }
}


void vector_free(vector_t* obj) {
    if(obj == NULL || obj -> array == NULL) {
        fprintf(stderr, "vector_free(): bad params\n");
        return;
    }
    
    free(obj -> array);
    obj -> array = NULL;
    //obj = NULL;
    obj -> capacity = 0;
    obj -> size = 0;
    obj -> elem_size = 0;
}



void free_vreg() {
    vreg_t* reg = create_vreg();

    if(reg == NULL || reg -> reg == NULL) {
        fprintf(stderr, "free_vreg(): reg is null\n");
        return;
    }
    printf("\n\nfree_vreg():\n");
    printf("\t%zu vectors to free\n", reg -> len); 
    for(size_t i = 0; i < reg -> len; i++){
        vector_free(reg -> reg[i]);
        printf("\t%p vec is freed\n", reg -> reg[i]);
    }
    
    free(reg -> reg);
    free(reg);
    printf("vreg is freed\n");
    reg = NULL;
}


void reserve(vector_t* obj, size_t n) {
    if(obj == NULL || n <= obj -> capacity) {
        fprintf(stderr, "reserve(): bad params\n");
        return;
    }

    if(obj -> array == NULL) {
        obj -> array = calloc(n, obj -> elem_size);
    } else {
        void* newArray;
        
        if((newArray = calloc(n, obj -> elem_size)) == NULL) {
            fprintf(stderr, "reserve(): bad alloc\n");
            return;
        }
        
        memcpy(newArray, obj->array, (obj->size) * (obj->elem_size));
        
        /*
        void* dest;
        for(size_t i = 0; i < obj->size; i++){
            dest = (char*)newArray + i*obj->elem_size;
            memcpy(dest, obj->array, obj->elem_size);
        }
        */

        free(obj -> array);
        obj -> array = newArray;
    }

    obj -> capacity = n;

    return;
}


void vector_init(vector_t* obj, void* array, size_t n, size_t nSize) {
    if(obj == NULL || array == NULL || n <= 0 || nSize <= 0) {
        fprintf(stderr, "vector_init(): bad params\n");
        return;
    }
    obj -> array = NULL;
    obj -> size = n;
    obj -> elem_size = nSize;
    obj -> capacity = 0;

    reserve(obj, 2*n);
    
    add_to_vreg(obj);
    
    /*
    void* dest;
    for(size_t i = 0; i < obj -> size; i++){
        dest = (char*)obj->array + i * obj->elem_size;
        memcpy(dest, array, obj->elem_size);
    }
    */

    memcpy(obj->array, array, obj->size*obj->elem_size);

}
    

void resize(vector_t* obj, size_t newSize, void* value, size_t val_size) {
    if(obj==NULL || obj->array == NULL || newSize <= 0 || val_size != obj->elem_size) {
        fprintf(stderr, "resize(): bad params\n");
        return;
    }

    if(newSize > obj->capacity) {
        reserve(obj, 2*newSize);
        if(obj == NULL || obj -> array == NULL) {
            fprintf(stderr, "resize() (reserve()): obj or array is NULL\n");
            return;
        }
    }

    void* dest;
    for(size_t i = obj->size; i < newSize; i++){
        dest = (char*)obj->array + i*(obj->elem_size);
        memcpy(dest, value, obj->elem_size);          
        obj->size++;  
    }
}






void processVec(vector_t* obj, ElemHandler handler) {
    if(obj == NULL || handler == NULL) {
        fprintf(stderr, "processVec(): bad params\n");
        return;
    }
    
    void* elem;
    for(size_t i = 0; i < obj->size; i++){
        elem = (char*)obj->array + i*obj->elem_size;
        handler(elem);
    }

}




    

