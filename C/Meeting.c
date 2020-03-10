#include "Meeting.h"
#include "Ordered_container.h"
#include "Person.h"
#include "Utility.h"

#include <stdlib.h>
#include <string.h>

static void save_Person_lastname(void* data_ptr, void* arg_ptr);

int g_Meeting_memory = 0;

/* a Meeting contains a time, a topic, and a container of participants */
struct Meeting {
	char* topic;
	int time; /* time stored as 12 hr time */
	struct Ordered_container* participants;	/* a container of pointers to struct Person objects */
};

struct Meeting* create_Meeting(int time, const char* topic){
	struct Meeting* new_Meeting;

	new_Meeting = malloc(sizeof(struct Meeting));	
	memory_check(new_Meeting);
	g_Meeting_memory += 1;

	new_Meeting->topic = alloc_init_string(topic);
	new_Meeting->participants = OC_create_container(&Person_comp_f);
	new_Meeting->time = time;

	return new_Meeting;
}

void destroy_Meeting(struct Meeting* meeting_ptr){
	
	destroy_string(meeting_ptr->topic);
	OC_destroy_container(meeting_ptr->participants);

	free(meeting_ptr);

	g_Meeting_memory -= 1;

}
int get_Meeting_time(const struct Meeting* meeting_ptr){
	return meeting_ptr->time;
}
void set_Meeting_time(struct Meeting* meeting_ptr, int time){
	meeting_ptr->time = time;
}
int add_Meeting_participant(struct Meeting* meeting_ptr, const struct Person* person_ptr){
	return OC_insert(meeting_ptr->participants, person_ptr);
}
int is_Meeting_participant_present(const struct Meeting* meeting_ptr, const struct Person* person_ptr){

	if(!OC_find_item(meeting_ptr->participants, person_ptr))
		return 0;
	else
		return 1;

}
int remove_Meeting_participant(struct Meeting* meeting_ptr, const struct Person* person_ptr){
	void* found_item;

	found_item = OC_find_item(meeting_ptr->participants, person_ptr);

	if(!found_item)
		return 1;
	else{
		OC_delete_item(meeting_ptr->participants, found_item);
		return 0;
	}
}
void print_Meeting(const struct Meeting* meeting_ptr){ 
	/* print Meeting does nothing if given NULL, because it has no return value */
	if(!meeting_ptr)
		return;

	printf("Meeting time: %d, Topic: %s\nParticipants:", get_Meeting_time(meeting_ptr), meeting_ptr->topic);

	if(OC_empty(meeting_ptr->participants)){
		printf(" None\n");
	}
	else{
		printf("\n");
		OC_apply(meeting_ptr->participants, &print_Person_wrapper);
	}

}
void save_Meeting(const struct Meeting* meeting_ptr, FILE* outfile){
	fprintf(outfile, "%d %s %d\n", get_Meeting_time(meeting_ptr), meeting_ptr->topic, OC_get_size(meeting_ptr->participants));

	if(OC_empty(meeting_ptr->participants) == 0){
		OC_apply_arg(meeting_ptr->participants, &save_Person_lastname, outfile);
	}
}	
struct Meeting* load_Meeting(FILE* input_file, const struct Ordered_container* people){
	struct Meeting* new_Meeting;
	int time;
	char topic[INPUTARRAYSIZE];

	int participant_list_size;
	char lastname[INPUTARRAYSIZE];
	void* found_item;

	/* Read in time, topic, expected number of participants */

	if(fscanf(input_file, "%d" INPUTFORMAT "%d", &time, topic, &participant_list_size) != 3){
		error("Invalid data found in file!");
		return NULL;
	}

	new_Meeting = create_Meeting(time, topic);
	/* Handle each participant lastname */
	/* Up to expected number of participants */
	for(int i=0; i<participant_list_size; ++i){
		/* Read in lastname */
		
		if(fscanf(input_file, INPUTFORMAT, lastname) != 1){
			error("Invalid data found in file!");
			destroy_Meeting(new_Meeting);
			return NULL;
		}
		/* find person in person list */
		found_item = OC_find_item_arg(people, lastname, &find_Person_by_name_f);

		/* validate person's existence */
		if(!found_item){
			error("Invalid data found in file!");
			destroy_Meeting(new_Meeting);
			return NULL;
		}

		/* add person as participant to meeting */
		OC_insert(new_Meeting->participants, OC_get_data_ptr(found_item));

	}

	return new_Meeting;
}

void save_Person_lastname(void* data_ptr, void* arg_ptr){
	/* arg_ptr points to outfile; data_ptr to Person */
	
	fprintf(arg_ptr, "%s\n", get_Person_lastname(data_ptr));
}
