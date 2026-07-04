#include "header.h"
#include "msg.h"
#include "list_tinfo.h"
#include "client_info.h"
#include "serializer.h"

int print_fd(tinfo_t* topic);
int print_fdarr(fdarr_t* arr);
void print_list(tlist_t** root, const char* what);

void test_1(void);
void test_2(void);
void test_fabric_tinfo(void);
void test_tinfo_fdarr(void);
void test_cinfo_ser(void);
void test_checkifexists(void);
void test_cpy_arr(void);
void test_serializer(void);
void test_servinfo(void);

void test_serv_loadtest(void);
void* cons_func(void* arg);
