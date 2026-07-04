#pragma once
#include <stdint.h>
#include <stddef.h>
#include "header.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HDRDELIM "?"

typedef struct msg {
	uint32_t hsize;
	uint32_t bsize;
	char* header;
	char* body;
	uint8_t flags;
} msg_t;

int init_msg(msg_t* msg, int del);
int serialize(msg_t* src, char* dst, size_t dstsize);
int deserialize(char* src, msg_t* dst);

int deserialize_hdr(char* src, msg_t* msg);
int serialize_hdr(msg_t* src, char* dst, size_t dstsize);

int deserialize_body(char* src, msg_t* dst);
int serialize_body(msg_t* src, char* dst, size_t dstsize);

int print_msgt(msg_t* msg);
