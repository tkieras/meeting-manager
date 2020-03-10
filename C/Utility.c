#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Person.h"
#include "Room.h"
#include "Meeting.h"
#include "Utility.h"

int g_string_memory = 0;				/* number of bytes used in C-strings */


/* Person helper functions that are needed by Person, main, Meeting */

int Person_comp_f(const void* data_ptr1, const void* data_ptr2){
	/* arguments to this function are void* that should point to Person objects */
	/* comparison on Persons is alphabetical by lastname */

	return strcmp(get_Person_lastname(data_ptr1), get_Person_lastname(data_ptr2));
}

int find_Person_by_name_f(const void* arg_ptr, const void* data_ptr){
	/* arg_ptr points to a string assumed to be a lastname */
	/* function returns relative value of arg_ptr and data_ptr's lastname*/

	return strcmp(arg_ptr, get_Person_lastname(data_ptr));
}

void print_Person_wrapper(void* data_ptr){
	print_Person(data_ptr);
}

/* Various */

char* alloc_init_string(const char* input){
	size_t str_len;
	char* new_string;

	str_len = strlen(input) + 1;

	new_string = malloc(str_len);
	memory_check(new_string);

	g_string_memory += (str_len);

	strncpy(new_string, input, str_len);
	return new_string;
}

void destroy_string(char* string){
	size_t str_len;
	str_len = strlen(string) + 1;

	g_string_memory -= (str_len);
	free(string);
}

void error(char* message){ 
	printf("%s\n", message); 
	while( getchar() != '\n' );
}
void memory_check(void* obj){
	if(!obj){
		printf("Memory allocation error.\n");
		exit(1);
	}
}

