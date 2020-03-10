#include "Ordered_container.h"
#include "Person.h"
#include "Meeting.h"
#include "Room.h"
#include "Utility.h"

#include <stdio.h>

//top level command functions
void print_individual(const struct Ordered_container* people_list_ptr);
void print_room(const struct Ordered_container* room_list_ptr);
void print_meeting(const struct Ordered_container* room_list_ptr);
void print_schedule(const struct Ordered_container* room_list_ptr);
void print_group(const struct Ordered_container* people_list_ptr);
void print_allocation(const struct Ordered_container* people_list_ptr, const struct Ordered_container* room_list_ptr);

void add_individual(struct Ordered_container* people_list_ptr);
void add_room(struct Ordered_container* room_list_ptr);
void add_meeting(struct Ordered_container* room_list_ptr);
void add_participant(const struct Ordered_container* people_list_ptr, struct Ordered_container* room_list_ptr);

void reschedule_meeting(struct Ordered_container* room_list_ptr);

void delete_individual(struct Ordered_container* people_list_ptr, const struct Ordered_container* room_list_ptr);
void delete_room(struct Ordered_container* room_list_ptr);
void delete_meeting(struct Ordered_container* room_list_ptr);
void delete_participant(const struct Ordered_container* people_list_ptr, struct Ordered_container* room_list_ptr);
void delete_schedule(struct Ordered_container* room_list_ptr);
void delete_group(struct Ordered_container* people_list_ptr, const struct Ordered_container* room_list_ptr);
void delete_all(struct Ordered_container* people_list_ptr, struct Ordered_container* room_list_ptr);

void save_data(const struct Ordered_container* people_list_ptr, const struct Ordered_container* room_list_ptr);

void load_data(struct Ordered_container* people_list_ptr, struct Ordered_container* room_list_ptr);

/* user input helper functions */

struct Person* get_Person_ptr_from_input(const struct Ordered_container* people_list_ptr);
struct Room* get_Room_ptr_from_input(const struct Ordered_container* room_list_ptr);
struct Meeting* get_Meeting_ptr_from_input(const struct Room* found_Room);
int get_Meeting_time_from_input(void);
int get_Room_number_from_input(void);
FILE* open_file_from_input(void);

/* Room helper functions */

int Room_comp_f(const void* data_ptr1, const void* data_ptr2);
int find_Room_by_number_f(const void* arg_ptr, const void* data_ptr);
void clear_Room_wrapper(void* data_ptr);
void print_Room_wrapper(void* data_ptr);
void destroy_Room_wrapper(void* data_ptr);
void save_Room_wrapper(void* data_ptr, void* arg_ptr);
int check_participant_Room_f(void* data_ptr, void* arg_ptr);
int check_scheduled_Room_f(void* data_ptr);


/* other (not Room-related) wrappers */

int check_participant_Meeting_wrapper(void* data_ptr, void* arg_ptr);
void save_Person_wrapper(void* data_ptr, void* arg_ptr);
void destroy_Person_wrapper(void* data_ptr);


int main(){
	char c1, c2;
	
	struct Ordered_container* people_list_ptr = OC_create_container(&Person_comp_f);
	struct Ordered_container* room_list_ptr = OC_create_container(&Room_comp_f);

	while(1){
		printf("\nEnter command: ");
		scanf(" %c %c", &c1, &c2);
	
		switch(c1) {
			case 'p':
				switch(c2){
					case 'i': print_individual(people_list_ptr); break;
					case 'r': print_room(room_list_ptr); break;
					case 'm': print_meeting(room_list_ptr); break;
					case 's': print_schedule(room_list_ptr); break;
					case 'g': print_group(people_list_ptr); break;
					case 'a': print_allocation(people_list_ptr, room_list_ptr); break;
					default:
						error("Unrecognized command!");
				}
			break;
			case 'a':
				switch(c2){
					case 'i': add_individual(people_list_ptr); break;
					case 'r': add_room(room_list_ptr); break;
					case 'm': add_meeting(room_list_ptr); break;
					case 'p': add_participant(people_list_ptr, room_list_ptr); break;
					default:
						error("Unrecognized command!");
				}
			break;
			case 'r':
				switch(c2){
					case 'm': reschedule_meeting(room_list_ptr); break;
					default:
						error("Unrecognized command!");
				}
			break;
			case 'd':
				switch(c2){
					case 'i': delete_individual(people_list_ptr, room_list_ptr); break;
					case 'r': delete_room(room_list_ptr); break;
					case 'm': delete_meeting(room_list_ptr); break;
					case 'p': delete_participant(people_list_ptr, room_list_ptr); break;
					case 's': delete_schedule(room_list_ptr); break;
					case 'g': delete_group(people_list_ptr, room_list_ptr); break;
					case 'a': delete_all(people_list_ptr, room_list_ptr); break;
					default:
						error("Unrecognized command!");
				}
			break;
			case 's':
				switch(c2){
					case 'd': save_data(people_list_ptr, room_list_ptr); break;
					default:
						error("Unrecognized command!");
				}
			break;
			case 'l':
				switch(c2){
					case 'd': load_data(people_list_ptr, room_list_ptr); break;
					default:
						error("Unrecognized command!");
				}
			break;
			case 'q':
				switch(c2){
					case 'q':
						delete_all(people_list_ptr, room_list_ptr);
						OC_destroy_container(people_list_ptr);
						OC_destroy_container(room_list_ptr);

			 			printf("Done\n");
			 			return 0;
				default:
					error("Unrecognized command!");
				}
			break;
			default:
				error("Unrecognized command!");
		}
	}

	return 0;
}

void print_individual(const struct Ordered_container* people_list_ptr){
	const struct Person* found_Person = get_Person_ptr_from_input(people_list_ptr);

	print_Person(found_Person);
}

void print_room(const struct Ordered_container* room_list_ptr){
	const struct Room* found_Room = get_Room_ptr_from_input(room_list_ptr);

	print_Room(found_Room);
}
void print_meeting(const struct Ordered_container* room_list_ptr){
	const struct Room* found_Room;
	const struct Meeting* found_Meeting;

	found_Room = get_Room_ptr_from_input(room_list_ptr);
	found_Meeting = get_Meeting_ptr_from_input(found_Room);

	print_Meeting(found_Meeting);
}
void print_schedule(const struct Ordered_container* room_list_ptr){

	if(OC_empty(room_list_ptr) != 0){
		error("List of rooms is empty");
		return;
	}
	printf("Information for %d rooms:\n", OC_get_size(room_list_ptr));

	OC_apply(room_list_ptr, &print_Room_wrapper);
}
void print_group(const struct Ordered_container* people_list_ptr){
	if(OC_empty(people_list_ptr) != 0){
		error("List of people is empty");
		return;
	}

	printf("Information for %d people:\n", OC_get_size(people_list_ptr));
	OC_apply(people_list_ptr, &print_Person_wrapper);

}
void print_allocation(const struct Ordered_container* people_list_ptr, const struct Ordered_container* room_list_ptr){
	printf("Memory allocations:\n");
	printf("C-strings: %d bytes total\n", g_string_memory);
	printf("Person structs: %d\n", OC_get_size(people_list_ptr));
	printf("Meeting structs: %d\n", g_Meeting_memory);
	printf("Room structs: %d\n", OC_get_size(room_list_ptr));
	printf("Containers: %d\n", g_Container_count);
	printf("Container items in use: %d\n", g_Container_items_in_use);
	printf("Container items allocated: %d\n", g_Container_items_allocated);
}

void add_individual(struct Ordered_container* people_list_ptr){
	char firstname[INPUTARRAYSIZE];
	char lastname[INPUTARRAYSIZE];
	char phoneno[INPUTARRAYSIZE];
	struct Person* new_Person;

	scanf(INPUTFORMAT INPUTFORMAT INPUTFORMAT, firstname, lastname, phoneno);

	new_Person = create_Person(firstname, lastname, phoneno);

	if(OC_insert(people_list_ptr, new_Person) == 0){
		error("There is already a person with this last name!");
		destroy_Person(new_Person);
	}
	else{
		printf("Person %s added\n", lastname);
	}
}
void add_room(struct Ordered_container* room_list_ptr){
	int number;
	struct Room* new_Room;

	number = get_Room_number_from_input();
	if(number < 0)
		return;

	new_Room = create_Room(number);

	if(OC_insert(room_list_ptr, new_Room) == 0){
		error("There is already a room with this number!");
		destroy_Room(new_Room);
	}
	else{
		printf("Room %d added\n", get_Room_number(new_Room));
	}
}

void add_meeting(struct Ordered_container* room_list_ptr){
	struct Room* found_Room;
	int time;
	char topic[INPUTARRAYSIZE];
	struct Meeting* new_Meeting;

	found_Room = get_Room_ptr_from_input(room_list_ptr);

	if(!found_Room)
		return;

	time = get_Meeting_time_from_input();
	if(time < 0)
		return;

	scanf(INPUTFORMAT, topic);

	new_Meeting = create_Meeting(time, topic);
	if(add_Room_Meeting(found_Room, new_Meeting) != 0){
		error("There is already a meeting at that time!");
		destroy_Meeting(new_Meeting);
		return;
	}

	printf("Meeting added at %d\n", get_Meeting_time(new_Meeting));

}
void add_participant(const struct Ordered_container* people_list_ptr, struct Ordered_container* room_list_ptr){
	const struct Room* found_Room;
	struct Meeting* found_Meeting;
	const struct Person* found_Person;

	found_Room = get_Room_ptr_from_input(room_list_ptr);
	found_Meeting = get_Meeting_ptr_from_input(found_Room);
	found_Person = get_Person_ptr_from_input(people_list_ptr);

	if(!found_Person || !found_Meeting)
		return;
	
	if(add_Meeting_participant(found_Meeting, found_Person) == 0){
		error("This person is already a participant!");
		return;
	}
	else{
		printf("Participant %s added\n", get_Person_lastname(found_Person));
	}
}

void reschedule_meeting(struct Ordered_container* room_list_ptr){
	struct Room* found_Room1;
	struct Room* found_Room2;
	struct Meeting* found_Meeting;
	int time;

	found_Room1 = get_Room_ptr_from_input(room_list_ptr);
	found_Meeting = get_Meeting_ptr_from_input(found_Room1);
	found_Room2 = get_Room_ptr_from_input(room_list_ptr);

	/* Read in time */

	time = get_Meeting_time_from_input();

	if(time < 0)
		return;

	if(!found_Room1 || !found_Room2 || !found_Meeting)
		return;

	remove_Room_Meeting(found_Room1, found_Meeting);
	set_Meeting_time(found_Meeting, time);
	if(add_Room_Meeting(found_Room2, found_Meeting) != 0){
		error("There is already a meeting at that time!");
		return;
	}
	printf("Meeting rescheduled to room %d at %d\n", get_Room_number(found_Room2), get_Meeting_time(found_Meeting));

}

void delete_individual(struct Ordered_container* people_list_ptr, const struct Ordered_container* room_list_ptr){
	char lastname[INPUTARRAYSIZE];
	void* found_item;
	struct Person* found_Person;

	scanf(INPUTFORMAT, lastname);

	found_item = OC_find_item_arg(people_list_ptr, lastname, find_Person_by_name_f);

	if(!found_item){
		error("No person with that name!");
		return;
	}
	
	found_Person = OC_get_data_ptr(found_item);

	if(OC_apply_if_arg(room_list_ptr, &check_participant_Room_f, found_Person) != 0){
		error("This person is a participant in a meeting!");
		return;
	}

	printf("Person %s deleted\n", get_Person_lastname(found_Person));
	destroy_Person(found_Person);
	OC_delete_item(people_list_ptr, found_item);

}
void delete_room(struct Ordered_container* room_list_ptr){
	int number;
	void* found_item;
	struct Room* found_Room;

	number = get_Room_number_from_input();
	if(number < 0)
		return;

	found_item = OC_find_item_arg(room_list_ptr, &number, find_Room_by_number_f);

	if(!found_item){
		error("No room with that number!");
		return;
	}

	found_Room = OC_get_data_ptr(found_item);

	printf("Room %d deleted\n", get_Room_number(found_Room));
	destroy_Room(found_Room);
	OC_delete_item(room_list_ptr, found_item);
	
}
void delete_meeting(struct Ordered_container* room_list_ptr){
	struct Room* found_Room;
	struct Meeting* found_Meeting;

	found_Room = get_Room_ptr_from_input(room_list_ptr);
	found_Meeting = get_Meeting_ptr_from_input(found_Room);
	
	if(!found_Room || !found_Meeting)
		return;
	
	remove_Room_Meeting(found_Room, found_Meeting);
	printf("Meeting at %d deleted\n", get_Meeting_time(found_Meeting));
	destroy_Meeting(found_Meeting);
	
}
void delete_participant(const struct Ordered_container* people_list_ptr, struct Ordered_container* room_list_ptr){
	const struct Room* found_Room;
	struct Meeting* found_Meeting;
	const struct Person* found_Person;

	found_Room = get_Room_ptr_from_input(room_list_ptr);
	found_Meeting = get_Meeting_ptr_from_input(found_Room);
	found_Person = get_Person_ptr_from_input(people_list_ptr);

	if(!found_Meeting || !found_Person)
		return;
	
	if(remove_Meeting_participant(found_Meeting, found_Person) != 0){
		error("This person is not a participant in the meeting!");
		return;
	}

	printf("Participant %s deleted\n", get_Person_lastname(found_Person));
	
}
void delete_schedule(struct Ordered_container* room_list_ptr){

	OC_apply(room_list_ptr, &clear_Room_wrapper);

	printf("All meetings deleted\n");

}
void delete_group(struct Ordered_container* people_list_ptr, const struct Ordered_container* room_list_ptr){
	if(OC_empty(people_list_ptr) != 0){
		error("List of people is empty");
		return;
	}

	/* Check if scheduled ... */

	if(OC_apply_if(room_list_ptr, &check_scheduled_Room_f) != 0){
		error("Cannot clear people list unless there are no meetings!");
		return;
	}

	OC_apply(people_list_ptr, &destroy_Person_wrapper);
	OC_clear(people_list_ptr);

	printf("All persons deleted\n");

}
void delete_all(struct Ordered_container* people_list_ptr, struct Ordered_container* room_list_ptr){
	OC_apply(room_list_ptr, &destroy_Room_wrapper);
	OC_clear(room_list_ptr);

	OC_apply(people_list_ptr, &destroy_Person_wrapper);
	OC_clear(people_list_ptr);

	printf("All rooms and meetings deleted\n");
	printf("All persons deleted\n");

}

void save_data(const struct Ordered_container* people_list_ptr, const struct Ordered_container* room_list_ptr){
	FILE* outfile = open_file_from_input();
	if(!outfile)
		return;

	fprintf(outfile, "%d\n", OC_get_size(people_list_ptr));
	OC_apply_arg(people_list_ptr, &save_Person_wrapper, outfile);
	fprintf(outfile, "%d\n", OC_get_size(room_list_ptr));
	OC_apply_arg(room_list_ptr, &save_Room_wrapper, outfile);

	printf("Data saved\n");
	fclose(outfile);
}

void load_data(struct Ordered_container* people_list_ptr, struct Ordered_container* room_list_ptr){
	FILE* infile;
	int person_tally;
	int room_tally;
	struct Person* new_Person;
	struct Room* new_Room;

	infile = open_file_from_input();
	if(!infile)
		return;
	
	/* If file is successfully opened, result of function is either:
	a successful load of the people and room list data
	or empty room and people lists due to invalid data file */

	OC_apply(people_list_ptr, &destroy_Person_wrapper);
	OC_clear(people_list_ptr);

	OC_apply(room_list_ptr, &destroy_Room_wrapper);
	OC_clear(room_list_ptr);

	if(fscanf(infile, "%d", &person_tally) <= 0 || person_tally < 0){
		error("Invalid data found in file!");
		return;
	}

	for(int i=0; i<person_tally; ++i){
		new_Person = load_Person(infile);
		if(!new_Person){
			OC_apply(people_list_ptr, &destroy_Person_wrapper);
			OC_clear(people_list_ptr);
			return;	
		}
		OC_insert(people_list_ptr, new_Person);
	}

	if(fscanf(infile, "%d", &room_tally) <= 0 || room_tally < 0){
		error("Invalid data found in file!");
		OC_apply(people_list_ptr, &destroy_Person_wrapper);
		OC_clear(people_list_ptr);
		return;
	}

	for(int i=0; i<room_tally; ++i){
		new_Room = load_Room(infile, people_list_ptr);
		if(!new_Room){
			OC_apply(people_list_ptr, &destroy_Person_wrapper);
			OC_clear(people_list_ptr);
			OC_apply(room_list_ptr, &destroy_Room_wrapper);
			OC_clear(room_list_ptr);
			return;
		}
		OC_insert(room_list_ptr, new_Room);
	}

	printf("Data loaded\n");
	fclose(infile);
}

/* User input helper functions */

struct Person* get_Person_ptr_from_input(const struct Ordered_container* people_list_ptr){
	char lastname[INPUTARRAYSIZE];
	void* found_item;

	scanf(INPUTFORMAT, lastname);

	found_item = OC_find_item_arg(people_list_ptr, lastname, find_Person_by_name_f);

	if(!found_item){
		error("No person with that name!");
		return NULL;
	}
	
	return OC_get_data_ptr(found_item);
}

struct Room* get_Room_ptr_from_input(const struct Ordered_container* room_list_ptr){
	int number;
	void* found_item;

	number = get_Room_number_from_input();
	if(number < 0)
		return NULL;

	found_item = OC_find_item_arg(room_list_ptr, &number, find_Room_by_number_f);

	if(!found_item){
		error("No room with that number!");
		return NULL;
	}

	return OC_get_data_ptr(found_item);
}

struct Meeting* get_Meeting_ptr_from_input(const struct Room* found_Room){
	int time;
	struct Meeting* found_Meeting;

	if(!found_Room)
		return NULL;

	time = get_Meeting_time_from_input();

	if(time < 0)
		return NULL;

	found_Meeting = find_Room_Meeting(found_Room, time);

	if(!found_Meeting){
		error("No meeting at that time!");
		return NULL;
	}

	return found_Meeting;

}

int get_Meeting_time_from_input(void){
	int time;

	if(scanf("%d", &time) == 0){
		error("Could not read an integer value!");
		return -1;
	}

	if(time < 1 || (time >= 5 && time < 9) || time > 12){
		error("Time is not in range!");
		return -1;
	}

	return time;
}
int get_Room_number_from_input(void){
	int number;

	if(scanf("%d", &number) == 0){
		error("Could not read an integer value!");
		return -1;
	}

	if(number <= 0){
		error("Room number is not in range!");
		return -1;
	}

	return number;
}

FILE* open_file_from_input(void){
	FILE* iofile;
	char filename[INPUTARRAYSIZE];

	scanf(INPUTFORMAT, filename);

	iofile = fopen(filename, "rw");

	if(!iofile){
		error("Could not open file!");
		return NULL;
	}

	return iofile;
}

/* Room helper functions */

int Room_comp_f(const void* data_ptr1, const void* data_ptr2){
	/* The arguments are void* that should point to Room objects */
	/* Comparison on Room objects occurs by comparing value of number */

	return get_Room_number(data_ptr1) - get_Room_number(data_ptr2);
}

int find_Room_by_number_f(const void* arg_ptr, const void* data_ptr){
	int number = *(int*)arg_ptr;

	return number - get_Room_number(data_ptr);
}

void clear_Room_wrapper(void* data_ptr){
	clear_Room((struct Room*)data_ptr);
}

void print_Room_wrapper(void* data_ptr){
	print_Room((struct Room*)data_ptr);
}

void destroy_Room_wrapper(void* data_ptr){
	destroy_Room((struct Room*)data_ptr);
}

void save_Room_wrapper(void* data_ptr, void* arg_ptr){
	save_Room((struct Room*)data_ptr, (FILE*)arg_ptr);
}

int check_participant_Room_f(void* data_ptr, void* arg_ptr){
	/* data_ptr is a Room, arg_ptr is a Person */

	return OC_apply_if_arg(get_Room_Meetings(data_ptr), check_participant_Meeting_wrapper, arg_ptr);
}

int check_scheduled_Room_f(void* data_ptr){

	if(OC_empty(get_Room_Meetings(data_ptr)))
		return 0;
	else
		return 1;
}

/* non-Room related helper functions */

int check_participant_Meeting_wrapper(void* data_ptr, void* arg_ptr){
	return is_Meeting_participant_present((const struct Meeting*)data_ptr, arg_ptr);
}
void save_Person_wrapper(void* data_ptr, void* arg_ptr){
	save_Person((const struct Person*) data_ptr, (FILE*)arg_ptr);
}
void destroy_Person_wrapper(void* data_ptr){
	destroy_Person((struct Person*) data_ptr);
}


