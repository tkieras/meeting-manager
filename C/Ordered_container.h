#ifndef ORDERED_CONTAINER_H
#define ORDERED_CONTAINER_H

/* 
Ordered_container provides a general-purpose ordered container facility, ecapsulated in such a way
that the user does not have to know the internal structure of the container, and cannot
inadvertantly damage its contents. 

This is done by making available in this header file only an incomplete type declaration 
for the container. Consequently, the user cannot create an Ordered_container as an 
local or static variable, but must use the facilities provided that create
Ordered_containers on the heap instead.

An Ordered_container consists of a series of items, each of which contains a void* pointer
to a data object, and with the items in order as specified by the pointed-to data and 
and ordering comparison function supplied when creating the container.

For example, if the Ordered_container is implemented as a linked list,
an item is a node containing a pointer to the data item. If it is implemented as an array, then
an item is a cell of the array containing a pointer to the data item.
Items are referred to only by a void* pointer; the user does not need information about 
the actual type of the item, but simply gets item pointers as returned values and 
supplies them to OC functions as needed.

To add information to the container, the user supplies a pointer to the data object, and the 
insert function creates an item containing that pointer at the proper position in the container. 
An item can be located in the container by a find function, that returns a pointer to the corresponding
item. The get_data_ptr function is then used to get the pointer to the data object from the item.

The user is responsible for creating and destroying all data objects 
pointed to by container items. These functions assume nothing about the pointed-to data,
do not modify the pointed-to data, and neither allocate nor deallocate 
the memory space occupied by the data. To add information to the container, the user must 
first allocate memory (if necessary) to store the data object, and then supply the pointer
to the data to one of the functions that adds an item to the container. Whenever an item
is removed from the container, the item is gone, but the calling code
is responsible for deallocating the pointed-to data object, if necessary, beforehand.

Note that a previously obtained pointer to an item in the container can be invalidated 
if items are then added or removed from the container.

The Ordered_container functions manage the memory for the container object itself and the items; 
the user must never call the free function with either an Ordered_container
pointer or an item pointer.

An Ordered_container is always referred to by a pointer, of type struct Ordered_container*,
which is returned from the OC_create_container function. When finished with the container, 
the user should dispose of it with the OC_destroy_container function.  Most of these functions
must be supplied with the pointer to the container to be operated on. 

In these functions, the container pointer parameter is declared const when the container
will not be modified by the function. Likewise, an item pointer is declared const 
if it is used only as an input to a comparison.

A data pointer parameter is always declared as pointer-to const to show that 
a container function will not modify the pointed-to data. However, since the data pointers 
are stored as void*, these functions will cast the data pointer from const void* to void* 
when storing the data pointer internally. A returned data pointer is always returned as
a void* since the client will have to cast it to the correct type anyway.

In contrast, a client-supplied apply function is allowed to modify the pointed-to data. But
if the apply function modifies the data in a way that disorders the container, trying to 
find the data with OC_find or OC_find_arg afterwards will produce undefined results.

A particular item in the container is referred to with a pointer of type void*.
Various functions in this package are given item pointers, or return item pointers.
To obtain the data in a particular item, the user must call the OC_get_data_ptr, which
will return the void* pointer in the item, which points to the user's data;
the user must cast this pointer to the correct type before using it to access the
pointed-to data.

These functions do not attempt to test for the validity of any of the supplied pointer
values; the calling code is completely responsible for ensuring that these functions
are called with valid pointer arguments. In general, the user must never modify the
Ordered_container or item pointer values returned by these functions, and must never call the
functions with Ordered_container or item pointers that were not previously returned by these
functions.

*/

/* Incomplete type declaration for Ordered_container objects */
struct Ordered_container;

/* These global variables are used to monitor the memory usage of the Ordered_container */
extern int g_Container_count;		/* number of Ordered_containers currently allocated */
extern int g_Container_items_in_use;	/* number of Ordered_container items currently in use */
extern int g_Container_items_allocated;	/* number of Ordered_container items currently allocated */


/* Type of comparison function to specify the order of items in an Ordered_container.
The function takes two arguments to data objects, and returns negative, 0, or positive, 
if first data object should come before, is equal to, or comes after, the second. The
items pointing to the data objects are then placed in the corresponding order in the container.
*/
typedef int (*OC_comp_fp_t) (const void* data_ptr1, const void* data_ptr2);


/*
Functions for the entire container.
*/

/* Create an empty container using the supplied comparison function, and return the pointer to it. */
struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr);

/* Destroy the container and its items; caller is responsible for 
deleting all pointed-to data before calling this function. 
After this call, the container pointer value must not be used again. */
void OC_destroy_container(struct Ordered_container* c_ptr);

/* Delete all the items in the container and initialize it. 
Caller is responsible for deleting any pointed-to data first. */
void OC_clear(struct Ordered_container* c_ptr);

/* Return the number of items currently stored in the container */
int OC_get_size(const struct Ordered_container* c_ptr);

/* Return non-zero (true) if the container is empty, zero (false) if the container is non-empty */
int OC_empty(const struct Ordered_container* c_ptr);

/*
Functions for working with individual items in the container.
*/

/* Get the data object pointer from an item. */
void* OC_get_data_ptr(const void* item_ptr);

/* Delete the specified item.
Caller is responsible for any deletion of the data pointed to by the item. */
void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr);

/*
Functions that search and insert into the container using the supplied comparison function.
*/

/* Create a new item for the specified data pointer and put it in the container in order.
If there is already an item in the container that compares equal to new item according to
the comparison function, the insertion will not take place and 0 is returned to indicate failure.
Otherwise, the insertion is done and non-zero is returned to show success. 
This function will not modify the pointed-to data. */
int OC_insert(struct Ordered_container* c_ptr, const void* data_ptr);

/* Return a pointer to an item that points to data equal to the data object pointed to by data_ptr,
using the ordering function to do the comparison with data_ptr as the first argument.
The data_ptr object is assumed to be of the same type as the data objects pointed to by container items.
NULL is returned if no matching item is found. 
The pointed-to data will not be modified. */
void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr);

/* typedef for a function used by OC_find_item_arg. The function returns negative, 0, or positive, 
if the data pointed to by arg_ptr corresponds to a data object that should come before, is equal to, or comes after, 
the data object pointed to by data_ptr. For example, arg_ptr could point to a datum with the same value
as a member of the sought-for data object. */
typedef int (*OC_find_item_arg_fp_t) (const void* arg_ptr, const void* data_ptr);

/* Return a pointer to the item that points to data that matches the supplied argument given by arg_ptr
according to the supplied function, which compares arg_ptr as the first argument with the data pointer
in each item. This function does not require that arg_ptr be of the same type as the data objects, and
so allows the container to be searched without creating a complete data object first.
NULL is returned if no matching item is found. If more than one matching item is present, it is 
unspecified which one is returned. The comparison function must implement an ordering consistent 
with the ordering produced by the comparison function specified when the container was created; 
if not, the result is undefined. */
void* OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, OC_find_item_arg_fp_t fafp);

/* Functions that traverse the items in the container, processing each item in order. */

/* Type of a function used by OC_apply.
An apply function takes a data pointer as an argument, and returns void. 
It is allowed to modify the data object, but if the ordering information is changed, 
the effects of attempting to search the container afterwards with OC_find are undefined. */
typedef void (*OC_apply_fp_t) (void* data_ptr);

/* Apply the supplied function to the data pointer in each item of the container. 
The contents of the container cannot be modified. */
void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp);

/* Type of a function used by OC_apply_if.
An apply_if function takes a data pointer as an argument, and returns zero or non-zero. 
It is allowed to modify the data object, but if the ordering information is changed, 
the effects of attempting to search the container afterwards with OC_find are undefined. */
typedef int (*OC_apply_if_fp_t) (void* data_ptr);

/* Apply the supplied function to the data pointer in each item in the container. 
If the function returns non-zero, the iteration is terminated, and that value
returned. Otherwise, zero is returned. The contents of the container cannot be modified. */
int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp);

/* Type of a function used by OC_apply_arg.
An OC_apply_arg function takes a data pointer as the first argument, 
a supplied argument pointer as a second argument, and returns void. 
It is allowed to modify the pointed-to argument, or the data object, 
but if the ordering information is changed, the effects of attempting to search 
the container afterwards with OC_find are undefined. */
typedef void (*OC_apply_arg_fp_t) (void* data_ptr, void* arg_ptr);

/* Apply the supplied function to the data pointer in each item in the container;
the function takes a second argument, which is the supplied void pointer. 
The contents of the container cannot be modified. */
void OC_apply_arg(const struct Ordered_container* c_ptr, OC_apply_arg_fp_t afp, void* arg_ptr);

/* Type of a function used by OC_apply_if_arg.
An OC_apply_arg function takes a data pointer as an argument, 
a supplied argument pointer as a second argument, and returns an zero or non-zero. 
It is allowed to modify the pointed-to argument, or the data object, 
but if the ordering information is changed, the effects of attempting to search 
the container afterwards with OC_find are undefined. */
typedef int (*OC_apply_if_arg_fp_t) (void* data_ptr, void* arg_ptr);

/* Apply the supplied function to the data pointer in each item in the container;
the function takes a second argument, which is the supplied void pointer. 
If the function returns non-zero, the iteration is terminated, and that value
returned. Otherwise, zero is returned. The contents of the container cannot be modified */
int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr);

#endif
