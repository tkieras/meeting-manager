#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

#define MAXINPUTLENGTH 63
#define INPUTFORMAT "%63s"
#define INPUTARRAYSIZE MAXINPUTLENGTH + 1

/* global variable declarations for this module */
extern int g_string_memory;				/* number of bytes used in C-strings */


/* Person related functions needed by main.c and meeting.c */
int Person_comp_f(const void* data_ptr1, const void* data_ptr2);
int find_Person_by_name_f(const void* arg_ptr, const void* data_ptr);
void print_Person_wrapper(void* data_ptr);


/* Various */

char* alloc_init_string(const char* input);
void destroy_string(char* string);

void error(char* message);
void memory_check(void* obj);

#endif
