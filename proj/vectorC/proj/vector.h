#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGSIZ 5

typedef struct vector {
    void* array;
    size_t size;
    size_t capacity;
    size_t elem_size;
} vector_t;

typedef void(*ElemHandler)(void*);



typedef struct vreg {
    vector_t** reg;
    size_t len;
} vreg_t;

static vreg_t* create_vreg();

void add_to_vreg(vector_t* obj);

void vector_free(vector_t* obj);

void vector_free(vector_t* obj);

void free_vreg();

void reserve(vector_t* obj, size_t n);

void vector_init(vector_t* obj, void* array, size_t n, size_t nSize);

void resize(vector_t* obj, size_t newSize, void* value, size_t val_size);

void processVec(vector_t* obj, ElemHandler handler);
