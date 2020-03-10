#include "Ordered_container.h"
#include "Utility.h"

#include <stdlib.h>


int g_Container_count = 0;		/* number of Ordered_containers currently allocated */
int g_Container_items_in_use = 0;	/* number of Ordered_container items currently in use */
int g_Container_items_allocated = 0;	/* number of Ordered_container items currently allocated */

/* struct LL_Node structure declaration. This declaration is local to this file. 
This is a two-way or doubly-linked list. Each node has a pointer to the previous 
node and another pointer to the next node in the list. This means insertions or
removals of a node can be made in constant time, once the location has been
determined. */
struct LL_Node { 
    struct LL_Node* prev;      /* pointer to the previous node */
	struct LL_Node* next;		/* pointer to the next node */
	void* data_ptr; 			/* uncommitted pointer to the data item */
};

/* Declaration for Ordered_container. This declaration is local to this file.  
A pointer is maintained to the last node in the list as well as the first, 
meaning that operations at the end of the list can be made in constant time.
The size member variable contains the number of nodes in the list and is
kept up-to-date, so that the size of the list can be accessed in constant time. */
struct Ordered_container {
	OC_comp_fp_t comp_func;
	struct LL_Node* first;
	struct LL_Node* last;
	int size;
};

static void free_node(struct LL_Node* node_ptr);

struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr){
	struct Ordered_container* new_OC;

	new_OC = malloc(sizeof(struct Ordered_container));
	memory_check(new_OC);

	g_Container_count += 1;

	new_OC->comp_func = f_ptr;
	new_OC->first = NULL;
	new_OC->last = NULL;
	new_OC->size = 0;

	return new_OC;
}

void OC_destroy_container(struct Ordered_container* c_ptr){

	OC_clear(c_ptr);

	free(c_ptr);
	g_Container_count -= 1;
}

void OC_clear(struct Ordered_container* c_ptr){
	struct LL_Node* tmp_ptr;

	for(struct LL_Node* node_ptr = c_ptr->first; node_ptr != NULL; node_ptr = tmp_ptr){
		tmp_ptr = node_ptr->next;
		free_node(node_ptr);
	}

	c_ptr->first = NULL;
	c_ptr->last = NULL;
	c_ptr->size = 0;

}

int OC_get_size(const struct Ordered_container* c_ptr){
	return c_ptr->size;
}

int OC_empty(const struct Ordered_container* c_ptr){
	return (c_ptr->size == 0);
}

void* OC_get_data_ptr(const void* item_ptr){
	return ((struct LL_Node*)item_ptr)->data_ptr;
}

void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr){
	struct LL_Node* old_prev_ptr = ((struct LL_Node*)item_ptr)->prev;
	struct LL_Node* old_next_ptr = ((struct LL_Node*)item_ptr)->next;

	free_node(item_ptr);
	c_ptr->size -= 1;
	
	if(!old_prev_ptr && !old_next_ptr){
		/* deleted first/last, i.e., the only item */
		c_ptr->first = NULL;
		c_ptr->last = NULL;
	}
	else if(!old_prev_ptr && old_next_ptr){
		/* deleted first but not last */
		c_ptr->first = old_next_ptr;
		old_next_ptr->prev = NULL;
	}
	else if(old_prev_ptr && old_next_ptr){
		/* deleted something neither first nor last */
		old_prev_ptr->next = old_next_ptr;
		old_next_ptr->prev = old_prev_ptr;
	}
	else if(old_prev_ptr && !old_next_ptr){
		/* deleted last but not first */
		c_ptr->last = old_prev_ptr;
		old_prev_ptr->next = NULL;
	}
}

int OC_insert(struct Ordered_container* c_ptr, const void* data_ptr){

	struct LL_Node* new_node_ptr;
	struct LL_Node* insertion_point;
	
	int comp_result;

	/* prepare the new_node */
	new_node_ptr = malloc(sizeof(struct LL_Node));
	memory_check(new_node_ptr);
	g_Container_items_in_use += 1;
	g_Container_items_allocated += 1;

	new_node_ptr->data_ptr = (void*)data_ptr;
	
	insertion_point = NULL;
	
	/* search for a node less than the new_node */

	for(struct LL_Node* node_ptr = c_ptr->first; node_ptr != NULL; node_ptr = node_ptr->next){
		comp_result = (*(c_ptr->comp_func))(data_ptr, node_ptr->data_ptr);

		if(comp_result < 0){		
			/* new_node belongs before node_ptr */
			insertion_point = node_ptr;			
			break;
		}
		else if(comp_result == 0){
			/* comp_func returns 0 if identical items are compared; therefore, do not insert. */
			free_node(new_node_ptr);
			return 0;
		}
	}

	if(OC_empty(c_ptr)){
		/* empty list means new node is both first and last */
		c_ptr->first = new_node_ptr;
		c_ptr->last = new_node_ptr;
		new_node_ptr->prev = NULL;
		new_node_ptr->next = NULL;
	}
	else if(!insertion_point){
		/* no insertion point and not empty means
		 insert at end */
		(c_ptr->last)->next = new_node_ptr;
		new_node_ptr->prev = (c_ptr->last);
		new_node_ptr->next = NULL;
		c_ptr->last = new_node_ptr;

	}
	else{
		/* insertion point means
		new_node belongs before insertion_point */
		new_node_ptr->next = insertion_point;
		new_node_ptr->prev = insertion_point->prev;
		insertion_point->prev = new_node_ptr;

		/* and may or may not be first */
		if(new_node_ptr->prev){
			(new_node_ptr->prev)->next = new_node_ptr;
		}
		else{
			c_ptr->first = new_node_ptr;
		}	
	}

	c_ptr->size += 1;
	return 1;	
}

void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr){

	for(struct LL_Node* node_ptr = c_ptr->first; node_ptr != NULL; node_ptr = node_ptr->next){
		if((*(c_ptr->comp_func))(data_ptr, node_ptr->data_ptr) == 0)
			return (void*)node_ptr;	
	}
	return NULL;
}

void* OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, OC_find_item_arg_fp_t fafp){
	
	for(struct LL_Node* node_ptr = c_ptr->first; node_ptr != NULL; node_ptr = node_ptr->next){
		if((*fafp)(arg_ptr, node_ptr->data_ptr) == 0)
			return (void*)node_ptr;	
	}
	return NULL;
}

void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp){
	
	for(struct LL_Node* node_ptr = c_ptr->first; node_ptr != NULL; node_ptr = node_ptr->next){
		(*afp)(node_ptr->data_ptr);
	}
}

int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp){
	int afp_result;

	for(struct LL_Node* node_ptr = c_ptr->first; node_ptr != NULL; node_ptr = node_ptr->next){
		afp_result = (*afp)(node_ptr->data_ptr);
		if(afp_result != 0)
			return afp_result;
	}
	return 0;
}

void OC_apply_arg(const struct Ordered_container* c_ptr, OC_apply_arg_fp_t afp, void* arg_ptr){
	
	for(struct LL_Node* node_ptr = c_ptr->first; node_ptr != NULL; node_ptr = node_ptr->next){
		(*afp)(node_ptr->data_ptr, arg_ptr);
	}
}

int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr){
	int afp_result;

	for(struct LL_Node* node_ptr = c_ptr->first; node_ptr != NULL; node_ptr = node_ptr->next){
		afp_result = (*afp)(node_ptr->data_ptr, arg_ptr);
		if(afp_result != 0)
			return afp_result;
	}
	return 0;
}

static void free_node(struct LL_Node* node_ptr){
	free(node_ptr);
	g_Container_items_in_use -= 1;
	g_Container_items_allocated -= 1;
}

