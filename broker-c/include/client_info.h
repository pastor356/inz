#pragma once
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <stdbool.h>

#define DELIM_CINFO "/"

// roles
#define CONS 0
#define PROD 1

typedef struct client_info {
	int role;
	char* topic_name;
} cinfo_t;

cinfo_t* create_cinfo(void);
int init_cinfo(cinfo_t* client, char* topic_name, int role);

// serializes client info in format `roleDELIM_CINFOtopic`
// returns len of dst used to serialize cinfo.
// MUST BE USED FOR INTRODUCTION
int serialize_cinfo(cinfo_t* src, size_t srcsize, char* dst);
int deserialize_cinfo(cinfo_t* dst, char* src);

int destroy_cinfo(cinfo_t* client);
