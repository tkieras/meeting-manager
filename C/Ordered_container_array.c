#include "Ordered_container.h"

#include "Utility.h"
#include <stdlib.h>

#define BASE_ARRAY_SIZE 3

static void OC_clear_array(struct Ordered_container* c_ptr);
static void OC_alloc_array(struct Ordered_container* c_ptr, int new_size);
static void OC_grow_array(struct Ordered_container* c_ptr);
static int OC_search(const struct Ordered_container* c_ptr, const void* data_ptr, int (*comp_func) (const void*, const void*), int* found);

int g_Container_count = 0;		/* number of Ordered_containers currently allocated */
int g_Container_items_in_use = 0;	/* number of Ordered_container items currently in use */
int g_Container_items_allocated = 0;	/* number of Ordered_container items currently allocated */

/* A complete type declaration for Ordered_container implemented as an array */
struct Ordered_container {
	OC_comp_fp_t comp_func;	/* pointer to comparison function  */
	void** array;			/* pointer to array of pointers to void */
	int allocation;			/* current size of array */
	int size;				/* number of items currently in the array */
};

struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr){
	struct Ordered_container* new_OC;

	new_OC = malloc(sizeof(struct Ordered_container));
	memory_check(new_OC);
	g_Container_count += 1;

	new_OC->size = 0;
	new_OC->comp_func = f_ptr;

	OC_alloc_array(new_OC, BASE_ARRAY_SIZE);

	return new_OC;
}

void OC_destroy_container(struct Ordered_container* c_ptr){
	OC_clear_array(c_ptr);
	free(c_ptr);
	g_Container_count -= 1;
}

void OC_clear(struct Ordered_container* c_ptr){
	OC_clear_array(c_ptr);
	OC_alloc_array(c_ptr, BASE_ARRAY_SIZE);
}
int OC_get_size(const struct Ordered_container* c_ptr){
	return c_ptr->size;
}
int OC_empty(const struct Ordered_container* c_ptr){
	return(c_ptr->size == 0);
}

void* OC_get_data_ptr(const void* item_ptr){
	/* cast item_ptr to (void**), then dereference it */
	return *(void**)item_ptr;
}

void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr){
	

	/* problem with for loop - how to index the item_ptr? */



	void** end_ptr = &(c_ptr->array)[c_ptr->size-1];
	void** copy_ptr = (void**)item_ptr;

	while(copy_ptr != end_ptr){
		copy_ptr[0] = copy_ptr[1];
		copy_ptr++;
	}

	*copy_ptr = NULL;
	c_ptr->size -= 1;
	g_Container_items_in_use -= 1;
}

int OC_insert(struct Ordered_container* c_ptr, const void* data_ptr){
	int found;
	int search_result_index;
	
	/* ensure there is space for insertion */
	if(c_ptr->size + 1 > c_ptr->allocation){
		OC_grow_array(c_ptr);
	}

	search_result_index = OC_search(c_ptr, data_ptr, c_ptr->comp_func, &found);

	if(found){
		/*insertion fails because equal item present*/
		return 0;
	}

	/* clear a space in array */
	for(int i = c_ptr->size; i > search_result_index; i--){
		c_ptr->array[i] = c_ptr->array[i-1];
	}

	/* insert */
	c_ptr->array[search_result_index] = (void*)data_ptr;
	c_ptr->size += 1;
	g_Container_items_in_use += 1;

	return 1;
}

void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr){
	int found;
	int search_result_index = OC_search(c_ptr, data_ptr, c_ptr->comp_func, &found);
	if(found)
		return &(c_ptr->array[search_result_index]);
	else
		return NULL;
}
void* OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, OC_find_item_arg_fp_t fafp){
	int found;
	int search_result_index = OC_search(c_ptr, arg_ptr, fafp, &found);
	if(found)
		return &(c_ptr->array[search_result_index]);
	else
		return NULL;
}
void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp){
	void** cell_ptr;

	for(int i=0; i<c_ptr->size; ++i){
		cell_ptr = &(c_ptr->array)[i];
		(*afp)(*cell_ptr);
	}
}
int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp){
	void** cell_ptr;
	int afp_result;

	for(int i=0; i<c_ptr->size; ++i){
		cell_ptr = &(c_ptr->array)[i];
		afp_result = (*afp)(*cell_ptr);
		if(afp_result != 0)
			return afp_result;	
	}
	return 0;
}
void OC_apply_arg(const struct Ordered_container* c_ptr, OC_apply_arg_fp_t afp, void* arg_ptr){
	void** cell_ptr;

	for(int i=0; i<c_ptr->size; ++i){
		cell_ptr = &(c_ptr->array)[i];
		(*afp)(*cell_ptr, arg_ptr);
	}
}
int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr){
	void** cell_ptr;
	int afp_result;

	for(int i=0; i<c_ptr->size; ++i){
		cell_ptr = &(c_ptr->array)[i];
		afp_result = (*afp)(*cell_ptr, arg_ptr);
		if(afp_result != 0)
			return afp_result;		
	}
	return 0;
}

static void OC_clear_array(struct Ordered_container* c_ptr){
	/* deallocates an array */
	free(c_ptr->array);
	g_Container_items_in_use -= c_ptr->size;
	g_Container_items_allocated -= c_ptr->allocation;
	c_ptr->size = 0;
}
static void OC_alloc_array(struct Ordered_container* c_ptr, int new_size){
	/* allocates an array of new_size void* */
	c_ptr->allocation = new_size;
	g_Container_items_allocated += new_size;

	c_ptr->array = malloc(sizeof(void*)*(c_ptr->allocation));
	memory_check(c_ptr->array);
}
static void OC_grow_array(struct Ordered_container* c_ptr){
	/* doubles array allocation, maintaining data */
	void** old_array = c_ptr->array;
	int old_allocation = c_ptr->allocation;
	int new_allocation = (old_allocation+1) * 2;

	OC_alloc_array(c_ptr, new_allocation);

	/* copy from old array */
	for(int i=0; i<c_ptr->size; ++i){
		c_ptr->array[i] = old_array[i];
	}

	g_Container_items_allocated -= old_allocation;
	free(old_array);
}
static int OC_search(const struct Ordered_container* c_ptr, const void* data_ptr, int (*comp_func) (const void*, const void*), int* found){
	/* OC_search takes a generic comparison function and returns either the index of the found data (if found) 
	or the index of the cell where the data would belong if inserted */
	int low, mid, high;
	int comp_result;

	for(low = 0, high = c_ptr->size-1, mid = (low+high) / 2; low<=high; mid = (low+high) / 2){

		comp_result = (*(comp_func))(data_ptr, (c_ptr->array)[mid]);

		if(comp_result > 0){
			low = mid + 1;
		}
		else if(comp_result < 0){
			high = mid - 1;
		}
		else{
			*found = 1;
			return mid;
		}
	}

	*found = 0;
	return low;

}
