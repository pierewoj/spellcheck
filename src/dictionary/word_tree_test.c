/** @file
  Testy do implementacji drzewa TRIE i funkcji pomocnicznych

  @ingroup dictionary
  @author Jakub Pierewoj
  @date 2015-05-29
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <locale.h>
#include "word_list.h"
#include "word_tree.c"

///ilość słów będących w drzewie w nowym środowisku
#define WORDS1_COUNT 10
///słowa będące w drzewie w nowym środowisku
const wchar_t *words1[WORDS1_COUNT] =
{ L"a", L"aab", L"aac", L"aaa", L"b", L"ba", L"bb", L"ąą", L"źą", L"ąę" };
///znaki wszystkich słów występujących w słowniku
const wchar_t *words1_chars = L"aąbcźę";

/**
 * funkcja tworząca środowisko zawierające słowa z listy words1
 */
static int word_tree_setup_words1(void **state)
{
	struct word_tree *tree = malloc(sizeof(struct word_tree));
	word_tree_init(tree);

	for (int i = 0; i < WORDS1_COUNT; i++)
	{
		int result = word_tree_add(tree, words1[i]);
		assert_int_equal(result, 1);
	}

	*state = tree;
	return 0;
}

/**
 * funkcja tworząca środowisko z pustym drzewem
 */
static int word_tree_setup_empty(void **state)
{
	struct word_tree *tree = malloc(sizeof(struct word_tree));
	word_tree_init(tree);

	*state = tree;
	return 0;
}

/**
 * niszczenie środowiska, delokacja zarezerwowanej pamięci
 */
static int word_tree_teardown(void **state)
{
	struct word_tree *tree = *state;
	word_tree_done(tree);
	return 0;
}
/**
 * prosty test sprawdzający poprawność wstawiania słów do drzewa
 * uruchamiany z drzewem zawierającym słowa z words1
 */
void word_tree_add_test(void **state)
{
	struct word_tree *tree = *state;

	for (int i = 0; i < WORDS1_COUNT; i++)
	{
		int result = word_tree_add(tree, words1[i]);
		assert_int_equal(result, 0);
	}
}

/**
 * prosty test sprawdzający poprawność usuwania słów z drzewa
 * uruchamiany z drzewem zawierającym słowa z words1
 */
void word_tree_delete_test(void **state)
{
	struct word_tree *tree = *state;

	for (int i = 0; i < WORDS1_COUNT; i++)
	{
		int result = word_tree_delete(tree, words1[i]);
		assert_int_equal(result, 1);
	}

	for (int i = 0; i < WORDS1_COUNT; i++)
	{
		int result = word_tree_delete(tree, words1[i]);
		assert_int_equal(result, 0);
	}
}

/**
 * prosty test sprawdzający poprawność szukania słów w drzewie
 * uruchamiany z drzewem zawierającym słowa z words1
 */
void word_tree_find_test(void **state)
{
	struct word_tree *tree = *state;

	for (int i = 0; i < WORDS1_COUNT; i++)
	{
		int result = word_tree_find(tree, words1[i]);
		assert_int_equal(result, 1);
	}

	for (int i = 0; i < WORDS1_COUNT; i++)
	{
		int result = word_tree_delete(tree, words1[i]);
		assert_int_equal(result, 1);
	}

	for (int i = 0; i < WORDS1_COUNT; i++)
	{
		int result = word_tree_find(tree, words1[i]);
		assert_int_equal(result, 0);
	}
}

/**
 * test sprawdzający poprawność działania funkcji usuwającej nadmiarowe
 * węzły w drzewie
 * uruchamiany z pustym drzewem
 */
void word_tree_delete_redundant_test(void **state)
{
	struct word_tree *tree = *state;
	word_tree_add(tree, L"a");
	word_tree_delete(tree, L"a");
	assert_true(find_next(tree, L'a') != NULL);
	word_tree_delete_redundant(tree, L"a");
	assert_true(find_next(tree, L'a') == NULL);

	word_tree_add(tree, L"ab");
	word_tree_add(tree, L"ac");
	assert_true(find_next(tree, L'a') != NULL);
	word_tree_delete(tree, L"ab");
	word_tree_delete(tree, L"ac");
	assert_true(find_next(tree, L'a') != NULL);
	word_tree_delete_redundant(tree, L"ab");
	word_tree_delete_redundant(tree, L"ac");
	assert_true(find_next(tree, L'a') == NULL);
}

/**
 * test sprawdzający poprawność działania funkcji dodającej
 * wszystkie znaki z drzewa do listy
 * uruchamiany z drzewem zawierającym słowa z words1
 */
void word_tree_add_chars_test(void **state)
{
	struct word_tree *tree = *state;
	int index = 0, size = 10;
	wchar_t *charList = malloc(size * sizeof(wchar_t));
	add_all_chars_to_list(tree, &charList, &index, &size);
	for (int i = 0; i < wcslen(words1_chars); i++)
	{
		wchar_t ch = words1_chars[i];
		int ch_count = 0;
		for (int j = 0; j < index; j++)
		{
			if (ch == charList[j])
				ch_count++;
		}
		assert_int_equal(ch_count, 1);
	}

}

/**
 * test sprawdzający poprawność działania funkcji binsearch
 * uruchamiany bez środowiska
 */
void word_tree_binsearch_test(void **state)
{
	const wchar_t tree_list_chars[5] =
	{ L'a', L'b', L'c', L'd', L'ź' };

	struct word_tree** list = malloc(5 * sizeof(struct word_tree*));

	for (int i = 0; i < 5; i++)
	{
		list[i] = malloc(sizeof(struct word_tree));
		word_tree_init(list[i]);
		list[i]->znak = tree_list_chars[i];
	}
	assert_int_equal(0, bin_search(list, tree_list_chars[0], 0, 4));
	assert_int_equal(1, bin_search(list, tree_list_chars[1], 0, 4));
	assert_int_equal(2, bin_search(list, tree_list_chars[2], 0, 4));
	assert_int_equal(3, bin_search(list, tree_list_chars[3], 0, 4));
	assert_int_equal(4, bin_search(list, tree_list_chars[4], 0, 4));
	assert_int_equal(-1, bin_search(list, L'e', 0, 4));

	for (int i = 0; i < 5; i++)
	{
		free(list[i]);
	}
	free(list);
}

/**
 * test sprawdzający poprawność działania funkcji find_next
 * uruchamiany z pustym drzewem
 */
void word_tree_find_next_test(void **state)
{
	struct word_tree* tree = *state;
	const wchar_t *words[5] =
	{ L"a", L"b", L"c", L"d", L"ą" };
	for (int i = 0; i < 5; i++)
		word_tree_add(tree, words[i]);

	assert_true(find_next(tree, L'a')->znak == L'a');
	assert_true(find_next(tree, L'ą')->znak == L'ą');
	assert_true(find_next(tree, L'b')->znak == L'b');
	assert_true(find_next(tree, L'c')->znak == L'c');
	assert_true(find_next(tree, L'd')->znak == L'd');
	assert_true(find_next(tree, L'y') == NULL);
	assert_true(find_next(tree, L'z') == NULL);
}

/**
 * test sprawdzający poprawność działania funkcji add_next
 * uruchamiany z pustym drzewem
 */
void word_tree_add_next_test(void **state)
{
	struct word_tree* tree = *state;

	struct word_tree* result;
	result = add_next(tree, L'a');
	assert_true(result != NULL);
	assert_true(result->znak == L'a');

	assert_true(find_next(tree, L'a') == result);

	result = add_next(tree, L'ą');
	assert_true(result != NULL);
	assert_true(result->znak == L'ą');
}

/**
 * test sprawdzający poprawność działania funkcji delete_next
 * uruchamiany z pustym drzewem
 */
void word_tree_delete_next_test(void **state)
{
	struct word_tree* tree = *state;

	add_next(tree, L'a');
	add_next(tree, L'b');
	add_next(tree, L'ą');

	assert_true(find_next(tree, L'a') != NULL);
	delete_next(tree, L'a');
	assert_true(find_next(tree, L'a') == NULL);

	assert_true(find_next(tree, L'b') != NULL);
	delete_next(tree, L'b');
	assert_true(find_next(tree, L'b') == NULL);

	assert_true(find_next(tree, L'ą') != NULL);
	delete_next(tree, L'ą');
	assert_true(find_next(tree, L'ą') == NULL);
}

/**
 * główna funkcja uruchamiająca testy
 */
int main(void)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");

	const struct CMUnitTest tests[] =
	{
	cmocka_unit_test_setup_teardown(word_tree_add_test,
			word_tree_setup_words1, word_tree_teardown),
	cmocka_unit_test_setup_teardown(word_tree_delete_test,
			word_tree_setup_words1, word_tree_teardown),
	cmocka_unit_test_setup_teardown(word_tree_find_test,
			word_tree_setup_words1, word_tree_teardown),
	cmocka_unit_test_setup_teardown(word_tree_delete_redundant_test,
			word_tree_setup_empty, word_tree_teardown),
	cmocka_unit_test_setup_teardown(word_tree_add_chars_test,
			word_tree_setup_words1, word_tree_teardown),
	cmocka_unit_test(word_tree_binsearch_test),
	cmocka_unit_test_setup_teardown(word_tree_find_next_test,
			word_tree_setup_empty, word_tree_teardown),
	cmocka_unit_test_setup_teardown(word_tree_add_next_test,
			word_tree_setup_empty, word_tree_teardown),
	cmocka_unit_test_setup_teardown(word_tree_delete_next_test,
			word_tree_setup_empty, word_tree_teardown), };

	return cmocka_run_group_tests(tests, NULL, NULL);
}
