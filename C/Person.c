#include "Person.h"
#include "Utility.h"

#include <stdlib.h>

/* a Person consist of pointers to C-strings for names and phone number */
struct Person {
	char* firstname;
	char* lastname;
	char* phoneno;
};

struct Person* create_Person(const char* firstname, const char* lastname, const char* phoneno){
	struct Person* new_Person;

	new_Person = malloc(sizeof(struct Person));
	memory_check(new_Person);

	new_Person->firstname = alloc_init_string(firstname);
	new_Person->lastname = alloc_init_string(lastname);
	new_Person->phoneno = alloc_init_string(phoneno);
	
	return new_Person;
}

void destroy_Person(struct Person* person_ptr){

	destroy_string(person_ptr->firstname);
	destroy_string(person_ptr->lastname);
	destroy_string(person_ptr->phoneno);

	free(person_ptr);
}

const char* get_Person_lastname(const struct Person* person_ptr){
	return person_ptr->lastname;
}

void print_Person(const struct Person* person_ptr){
	/* print_Person will do nothing if given NULL 
	because there is no return value */

	if(!person_ptr)
		return;

	printf("%s %s %s\n", person_ptr->firstname, person_ptr->lastname, person_ptr->phoneno);
}

void save_Person(const struct Person* person_ptr, FILE* outfile){
	fprintf(outfile, "%s %s %s\n", person_ptr->firstname, person_ptr->lastname, person_ptr->phoneno);
}

struct Person* load_Person(FILE* infile){
	char firstname[INPUTARRAYSIZE];
	char lastname[INPUTARRAYSIZE];
	char phoneno[INPUTARRAYSIZE];

	if(fscanf(infile, INPUTFORMAT INPUTFORMAT INPUTFORMAT, firstname, lastname, phoneno) != 3){
		error("Invalid data found in file!");
		return NULL;
	}

	return create_Person(firstname, lastname, phoneno);
}
