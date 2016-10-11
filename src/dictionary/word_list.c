/** @file
 Implementacja listy słów.

 @ingroup dictionary
 @author Jakub Pierewoj
 @date 2015-05-23
 */

#include "word_list.h"
#include <stdlib.h>

/** @name Funkcje pomocnicze
 @{
 */

/**
 Wrapper funkcji porównującej do qsort
 @param s1 wskaźnik na pierwsze słowo
 @param s2 wskaźnik nad drugie słowo
 @return 0 jak równe, -1 jak pierwsze mniejsze, 1 wpp
 */
int compare_function(const void *s1, const void *s2)
{
	return wcscoll(*(const wchar_t **) s1, *(const wchar_t **) s2);
}

/**@}*/
/** @name Elementy interfejsu 
 @{
 */

bool word_list_find(struct word_list *list, wchar_t *word)
{
	for (int i = 0; i < list->size; i++)
		if (wcscoll(list->array[i], word) == 0)
			return true;
	return false;
}

void word_list_sort(struct word_list *list)
{
	qsort(list->array, list->size, sizeof(wchar_t *), compare_function);
}

void word_list_init(struct word_list *list)
{
	list->size = 0;
	list->array_size = 3;
	list->array = malloc((list->array_size) * sizeof(wchar_t*));
}

void word_list_done(struct word_list *list)
{
	if (list->array == NULL)
		return;
	for (int i = 0; i < list->size; i++)
		free(list->array[i]);
	free(list->array);
}

int word_list_add(struct word_list *list, const wchar_t *word)
{
	if (list->size >= list->array_size)
	{
		list->array_size *= 2;
		wchar_t **new_array = malloc(list->array_size * sizeof(wchar_t*));
		for (int i = 0; i < list->size; i++)
			new_array[i] = list->array[i];
		free(list->array);
		list->array = new_array;
	}
	list->array[list->size] = malloc((wcslen(word) + 1) * sizeof(wchar_t));
	for (int i = 0; i <= wcslen(word); i++)
		list->array[list->size][i] = word[i];
	list->size++;

	return 1;
}

/**@}*/
