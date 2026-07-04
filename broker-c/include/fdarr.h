#pragma once
#include <poll.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct fd_array {
	struct pollfd* fdarr;
	size_t sz;
	size_t cap;
} fdarr_t;

int reserve(fdarr_t* arr, size_t cap);

int init_fdarr(fdarr_t** arr, size_t c);
int destroy_fdarr(fdarr_t* arr);

int push_backp(fdarr_t* arr, struct pollfd* fd);
int push_backf(fdarr_t* arr, int fd);
int pop_backp(fdarr_t* arr, struct pollfd* poped_obj);
int swap_back(fdarr_t* arr, int idx); 


// copy _arr() doesn't appends array,
// fully overrides it
int copy_arr(fdarr_t* src, fdarr_t* dst);

/* find idx of fd inside of arr.
   if fd is not found returns val < 0
   otherwise returns >= 0				*/
int find_fd(fdarr_t* arr, unsigned int fd);

// returns idx of client with revents := (POLLIN || POLLHUP)
// if poll() returned because of timeout 
// poll_fdarr() returns -1
int poll_fdarr(fdarr_t* arr, int poll_timeout);


// removes client[idx] from the list
// returns 0 in case of success
// < 0 in case of failure
int delete_client(fdarr_t* arr, ssize_t idx);


/* 
int push_frontp(fdarr_t* arr, struct pollfd* fd);
int push_frontf(fdarr_t* arr, int fd);
*/ 
