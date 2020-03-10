#include "Room.h"
#include "Meeting.h"
#include "Ordered_container.h"
#include "Utility.h"

#include <stdlib.h>

static void save_Meeting_wrapper(void* data_ptr, void* arg_ptr);
static void print_Meeting_wrapper(void* data_ptr);
static void destroy_Meeting_wrapper(void* data_ptr);
static int Meeting_find_item_arg_f(const void* arg_ptr, const void* data_ptr);
static int Meeting_comp_f(const void* data_ptr1, const void* data_ptr2);
static int time_to_24(int time); /* used in comparison functions */

/* a Room contains a container of meetings and a room number */
struct Room {
	struct Ordered_container* meetings;	/* a container of pointers to Meeting objects */
	int number;
};

struct Room* create_Room(int number){
	struct Room* new_Room;

	new_Room = malloc(sizeof(struct Room));
	memory_check(new_Room);

	new_Room->number = number;
	new_Room->meetings = OC_create_container(&Meeting_comp_f);

	return new_Room;
}

void destroy_Room(struct Room* room_ptr){
	/* destroy meetings first */
	OC_apply(room_ptr->meetings, &destroy_Meeting_wrapper);
	OC_destroy_container(room_ptr->meetings);

	free(room_ptr);
}

int get_Room_number(const struct Room* room_ptr){
	return room_ptr->number;
}

int add_Room_Meeting(struct Room* room_ptr, const struct Meeting* meeting_ptr){
	if(OC_insert(room_ptr->meetings, meeting_ptr) == 0)
		return 1;
	else
		return 0;
}
struct Meeting* find_Room_Meeting(const struct Room* room_ptr, int time){
	void* found_item;

	found_item = OC_find_item_arg(room_ptr->meetings, &time, &Meeting_find_item_arg_f);

	if(!found_item){
		return NULL;
	}
	else{
		return OC_get_data_ptr(found_item);
	}

}

int remove_Room_Meeting(struct Room* room_ptr, const struct Meeting* meeting_ptr){
	void* found_item;

	found_item = OC_find_item(room_ptr->meetings, meeting_ptr);

	if(!found_item){
		return 1;
	}
	else{
		OC_delete_item(room_ptr->meetings, found_item);
		return 0;
	}
}

void clear_Room(struct Room* room_ptr){
	OC_apply(room_ptr->meetings, &destroy_Meeting_wrapper);
	OC_clear(room_ptr->meetings);
}

const struct Ordered_container* get_Room_Meetings(const struct Room* room_ptr){
	return room_ptr->meetings;
}

void print_Room(const struct Room* room_ptr){
	/* print_Room will do nothing if given NULL 
	because there is no return value */
	
	if(!room_ptr)
		return;

	printf("--- Room %d ---\n", room_ptr->number);

	if(OC_empty(room_ptr->meetings)){
		printf("No meetings are scheduled\n");
	}
	else{
		OC_apply(room_ptr->meetings, &print_Meeting_wrapper);
	}
}
void save_Room(const struct Room* room_ptr, FILE* outfile){
	fprintf(outfile, "%d %d\n", room_ptr->number, OC_get_size(room_ptr->meetings));
	OC_apply_arg(room_ptr->meetings, &save_Meeting_wrapper, outfile);
}

struct Room* load_Room(FILE* infile, const struct Ordered_container* people){
	int number;
	struct Room* new_Room;

	int meeting_list_size;
	struct Meeting* new_Meeting;

	/* Read in Room number & expected number of meetings */

	if(fscanf(infile, "%d %d", &number, &meeting_list_size) != 2){
		error("Invalid data found in file!");
		return NULL;
	}

	new_Room = create_Room(number);

	for(int i=0; i<meeting_list_size; ++i){
		new_Meeting = load_Meeting(infile, people);
		if(!new_Meeting){
			destroy_Room(new_Room);
			return NULL;
		}
		else{
			OC_insert(new_Room->meetings, new_Meeting);
		}
	}

	return new_Room;
}

void save_Meeting_wrapper(void* data_ptr, void* arg_ptr){
	save_Meeting(data_ptr, arg_ptr);
}
void print_Meeting_wrapper(void* data_ptr){
	print_Meeting(data_ptr);
}

void destroy_Meeting_wrapper(void* data_ptr){
	destroy_Meeting(data_ptr);
}

int Meeting_find_item_arg_f(const void* arg_ptr, const void* data_ptr){
	/* arg_ptr points to an integer holding a 12-hr time */
	/* data_ptr points to a Meeting object */
	/* after extracting times, compares like Meeting_comp_f*/

	int time1, time2;

	time1 = *(int*)arg_ptr;
	time2 = get_Meeting_time(data_ptr);

	return (time_to_24(time1) - time_to_24(time2));
}
int Meeting_comp_f(const void* data_ptr1, const void* data_ptr2){
	/* Comparison of meetings occurs by value of time */
	/* per Meeting.h, time returned is 12 hr */	
	int time1, time2;

	time1 = get_Meeting_time(data_ptr1);
	time2 = get_Meeting_time(data_ptr2);

	return (time_to_24(time1) - time_to_24(time2));
}

int time_to_24(int time){
	/* lacking am/pm designation,
	assume that 1<= time > 9 is pm */
	
	if(time < 9 && time > 1)
		time = time + 12;

	return time;
}
