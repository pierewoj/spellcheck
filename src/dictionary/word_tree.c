/** @file
  Implementacja drzewa słów

  @ingroup dictionary
  @author Jakub Pierewoj
  @date 2015-05-23
 */

#include "word_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <wctype.h>
#include "utils.h"

/** @name Funkcje pomocnicze
   @{
 */

/**
 Wyszukiwanie binarne drzewa o danym znkau
 @param list tablica wskaźników na drzewa
 @param ch znak
 @param a indeks początkowy
 @param b indeks końcowy
 @return pozycja szukanego drzewa lub -1 jak nie uda się znaleźć
 */
static int bin_search(struct word_tree** list, wchar_t ch, int a, int b)
{
	if (a > b)
		return -1;

	if (list == NULL)
		return -1;

	if (a == b)
	{
		if (list[a]->znak == ch)
			return a;
		else
			return -1;
	}

	int m = (a + b) / 2;
	wchar_t fm = list[m]->znak;

	if (fm == ch)
		return m;
	else if (list[m]->znak > ch)
		return bin_search(list, ch, a, m);
	else
		return bin_search(list, ch, m + 1, b);
}

/*
 Szukanie potomka o podanym znaku
 @param tree drzewo
 @param ch znak
 @return szukany potomek lub NULL jak się nie uda znaleźć
 */
struct word_tree* find_next(const struct word_tree *tree, const wchar_t ch)
{
	assert(tree != NULL);

	if (tree->list_length == 0)
		return NULL;

	int pos = bin_search(tree->next, ch, 0, tree->list_length - 1);
	if (pos < 0)
		return NULL;
	else
		return tree->next[pos];
}

/*
 Dodawanie potomka o podanym znaku
 Jeżeli potomek o podanym znaku już istnieje zostanie rzucona asercja
 @param tree drzewo
 @param ch znak
 @return nowy potomek lub NULL jak się nie uda dodać
 */
struct word_tree* add_next(struct word_tree *tree, const wchar_t ch)
{
	assert(tree != NULL);

	int pos = bin_search((tree->next), ch, 0, tree->list_length - 1);
	if (pos < 0)
	{
		struct word_tree **new_list = malloc(
				(tree->list_length + 1) * sizeof(struct word_tree*));

		int new_list_index = 0, old_list_index = 0;

		for (old_list_index = 0; old_list_index < tree->list_length;
				old_list_index++, new_list_index++)
		{
			if (tree->next[old_list_index]->znak > ch)
				break;
			new_list[new_list_index] = tree->next[old_list_index];
		}

		new_list[new_list_index] = malloc(sizeof(struct word_tree));
		word_tree_init(new_list[new_list_index]);
		new_list[new_list_index]->parent = tree;
		new_list[new_list_index]->znak = ch;
		new_list_index++;

		for (; old_list_index < tree->list_length; old_list_index++, new_list_index++)
		{
			new_list[new_list_index] = tree->next[old_list_index];
		}

		if (tree->next != NULL)
			free(tree->next);
		tree->next = new_list;
		tree->list_length++;

		return find_next(tree, ch);
	}
	else
	{
		assert(false);
		return NULL;
	}
}

/*
 Usuwanie potomka o podanym znaku
 @param tree drzewo
 @param ch znak
 */
void delete_next(struct word_tree *tree, const wchar_t ch)
{
	assert(tree != NULL);

	int pos = bin_search((tree->next), ch, 0, tree->list_length - 1);
	if (pos < 0)
	{
		assert(false);
	}
	else
	{
		struct word_tree **new_list = malloc(
				(tree->list_length - 1) * sizeof(struct word_tree*));
		int new_list_index = 0;

		for (int i = 0; i < tree->list_length; i++)
		{
			if (i == pos)
				continue;

			new_list[new_list_index] = tree->next[i];
			new_list_index++;
		}
		assert(new_list_index == tree->list_length - 1);

		word_tree_done(tree->next[pos]);
		free(tree->next);

		if (tree->list_length == 1)
		{
			free(new_list);
			tree->next = NULL;
			tree->list_length = 0;
		}
		else
		{
			tree->next = new_list;
			tree->list_length--;
		}
	}
}
/**@}*/
/** @name Elementy interfejsu
   @{
 */

void word_tree_init(struct word_tree *tree)
{
	tree->is_word = false;
	tree->znak = L'\0';
	tree->next = NULL;
	tree->list_length = 0;
	tree->parent = NULL;
}

void word_tree_done(struct word_tree *tree)
{
	if (tree == NULL)
		return;
	for (int i = 0; i < tree->list_length; i++)
		word_tree_done(tree->next[i]);
	free(tree->next);
	if(tree->znak != L'\0')
		free(tree);
}

int word_tree_add(struct word_tree *tree, const wchar_t *word)
{
	if (*word == L'\0')
	{
		if (tree->is_word)
		{
			return 0;
		}
		else
		{
			tree->is_word = true;
			return 1;
		}
	}
	else
	{
		struct word_tree* next_node = find_next(tree, *word);
		if (next_node == NULL)
			return word_tree_add(add_next(tree, *word), word + 1);
		else
			return word_tree_add(next_node, word + 1);
	}
}

bool word_tree_delete_redundant(struct word_tree *tree, const wchar_t *word)
{
	struct word_tree *next;
	if (*word == L'\0')
		next = NULL;
	else
		next = find_next(tree, *word);

	bool next_needs_to_be_deleted = false;
	if (next != NULL)
		next_needs_to_be_deleted = word_tree_delete_redundant(next, word + 1);

	if (next_needs_to_be_deleted)
		delete_next(tree, *word);

	if (tree->list_length == 0 && tree->is_word == false)
	{
		return true;
	}

	return false;
}

int word_tree_delete(struct word_tree *tree, const wchar_t *word)
{
	if (*word == L'\0')
	{
		if (tree->is_word)
		{
			tree->is_word = false;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		struct word_tree* next_node = find_next(tree, *word);
		if (next_node == NULL)
			return 0;
		else
			return word_tree_delete(next_node, word + 1);
	}
}

int word_tree_find(const struct word_tree *tree, const wchar_t *word)
{
	if (*word == L'\0')
	{
		if (tree->is_word)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		struct word_tree* next_node = find_next(tree, *word);
		if (next_node == NULL)
			return 0;
		else
		{
			//assert(next_node->parent == tree);
			return word_tree_find(next_node, word + 1);
		}
	}
}

void word_tree_save(const struct word_tree *tree, FILE* stream)
{
	if(tree->is_word)
		fprintf(stream,"%lc",L'*');
	for(int i=0;i<tree->list_length;i++)
	{
		fprintf(stream,"%lc",tree->next[i]->znak);
		word_tree_save(tree->next[i],stream);
	}
	fprintf(stream,"%lc",L'#');
	return;
}

int word_tree_load(FILE* stream, struct word_tree* tree)
{
	wchar_t c = fgetwc(stream);
	if (c == WEOF || c == L'#')
		return 0;
	if (c == L'*')
		tree->is_word = true;
	else if(iswalpha(c))
	{
		struct word_tree * newTree = add_next(tree, c);
		int res = word_tree_load(stream, newTree);
		if(res != 0)
			return 1;
	}
	else
	{
		return 1;
	}
	return word_tree_load(stream, tree);

}

void add_char_to_list(wchar_t ch, wchar_t **list, int* size, int *allocated)
{
	bool is_on_list = false;
	for (int i = 0; i < *size; i++)
	{
		if (ch == (*list)[i])
		{
			is_on_list = true;
			break;
		}
	}

	if (!is_on_list)
	{
		if (*size >= *allocated)
		{
			(*allocated)++;
			(*allocated) *= 2;
			wchar_t *newList = malloc((*allocated) * sizeof(wchar_t));
			for (int i = 0; i < (*size); i++)
			{
				newList[i] = (*list)[i];
			}
			free((*list));
			(*list) = newList;
		}
		(*list)[*size] = ch;
		(*size)++;
	}
}

void add_all_chars_to_list(const struct word_tree *tree, wchar_t **list,
		int *size, int *allocated)
{
	if (tree->znak != L'\0')
	{
		add_char_to_list(tree->znak, list, size, allocated);
	}

	for (int i = 0; i < tree->list_length; i++)
		add_all_chars_to_list((tree->next)[i], list, size, allocated);
}
/**@}*/
