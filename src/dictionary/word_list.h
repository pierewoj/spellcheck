/** @file
 Interfejs listy słów.

 @ingroup dictionary
 @author Jakub Pawlewicz <pan@mimuw.edu.pl>
 @copyright Uniwerstet Warszawski
 @date 2015-05-10
 */

#ifndef __WORD_LIST_H__
#define __WORD_LIST_H__

#include <wchar.h>
#include <stdbool.h>

/**
 Struktura przechowująca listę słów.
 Należy używać funkcji operujących na strukturze,
 gdyż jej implementacja może się zmienić.
 */
struct word_list
{
	/// Liczba słów.
	size_t size;
	/// Wielkość tablicy
	size_t array_size;
	/// Tablica słów.
	wchar_t **array;
};

/**
 Szuka słowo w liście
 @param list Lista słów.
 @param word słowo
 @return true wtw gdy słowo znajduje się w liście
 */
bool word_list_find(struct word_list *list, wchar_t *word);

/**
 Stortuje listę słów wg porządku leksykograficznego.
 @param[in,out] list Lista słów.
 */
void word_list_sort(struct word_list *list);

/**
 Inicjuje listę słów.
 @param[in,out] list Lista słów.
 */
void word_list_init(struct word_list *list);

/**
 Destrukcja listy słów.
 @param[in,out] list Lista słów.
 */
void word_list_done(struct word_list *list);

/**
 Dodaje słowo do listy.
 @param[in,out] list Lista słów.
 @param[in] word Dodawane słowo.
 @return 1 jeśli się udało, 0 w p.p.
 */
int word_list_add(struct word_list *list, const wchar_t *word);

/**
 Zwraca liczę słów w liście.
 @param[in] list Lista słów.
 @return Liczba słów w liście.
 */
static inline size_t word_list_size(const struct word_list *list)
{
	return list->size;
}

/**
 Zwraca tablicę słów w liście.
 @param[in] list Lista słów.
 @return Tablica słów.
 */
static inline const wchar_t * const * word_list_get(
		const struct word_list *list)
{
	return (const wchar_t * const *) list->array;
}

#endif /* __WORD_LIST_H__ */
