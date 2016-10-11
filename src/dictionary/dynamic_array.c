/** @file
 Implementacja tablicy dynamicznej

 @ingroup dictionary
 @author Jakub Pierewoj
 @date 2015-06-01
 */
#include "dynamic_array.h"
#include <stdlib.h>

/** @name Funkcje pomocnicze
 @{
 */

/**@}*/
/** @name Elementy interfejsu
 @{
 */
dynamic_array* array_create()
{
	return NULL;
}


void array_done(dynamic_array* arr)
{
}


void array_add(dynamic_array* arr, void* el)
{
}


void array_add_all(dynamic_array* destination, dynamic_array *source)
{
}


int array_size(dynamic_array* arr)
{
	return 0;
}


void* array_get(dynamic_array* arr, int index)
{
	return NULL;
}


void sort(dynamic_array* arr, int (*comparator)(const void *, const void *))
{
}


int bin_search(dynamic_array* arr, int begin, int end,
		int (*comparator)(const void *, const void *))
{
	return 0;
}
/**@}*/
