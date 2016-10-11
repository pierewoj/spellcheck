/** @file
  Testy ogólnej spójności biblioteki dictionary

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
#include <stdlib.h>
#include <string.h>
#include "conf.h"
#include "word_list.h"
#include "dictionary.h"

 ///ilość słów w słowniku w nowym środowisku
#define WORDS_PRESENT_COUNT 10
 ///tablica słów w słowniku w nowym środowisku
const wchar_t* words_present[WORDS_PRESENT_COUNT] =
{ L"ąa", L"ab", L"ac", L"ae", L"af", L"aaaą", L"aaab", L"ąćźżęć", L"z", L"y" };

///ilość przykładów słów nie będących w słowniku w nowym środowisku
#define WORDS_NOT_PRESENT_COUNT 5
///tablic przykłądów słów nie będących w słowniku
const wchar_t* words_not_present[WORDS_NOT_PRESENT_COUNT] =
{ L"ą", L"b", L"ęg", L"aaa", L"ź" };

///ilość podpowiedzi dla słowa 1
#define HINTS_WORD1_COUNT 7
///słowo 1 dla którego generowane są podpowiedzi
const wchar_t* word1 = L"a";
///podpowiedzi
const wchar_t* hints_word1[HINTS_WORD1_COUNT] =
{ L"ab", L"ac", L"ae", L"af", L"ąa", L"y", L"z" };

///ilość podpowiedzi dla słowa word2
#define HINTS_WORD2_COUNT 2
///słowo 2 dla którego generowane są podpowiedzi
const wchar_t* word2 = L"ąz";
///podpowiedzi
const wchar_t* hints_word2[HINTS_WORD2_COUNT] =
{ L"ąa", L"z" };

///ilość podpowiedzi dla słowa word3
#define HINTS_WORD3_COUNT 0
///słowo 3 dla którego generowane są podpowiedzi
const wchar_t* word3 = L"aąaa";

/**
 * test sprawdzający poprawność wstawiania/usuwania słów
 */
static void dictionary_simple_add_remove_test(void **state)
{
	struct dictionary *l = dictionary_new();

	/*
	 * wstawianie wszystkich słów, powinno się udać
	 */
	for (int i = 0; i < WORDS_PRESENT_COUNT; i++)
	{
		int result = dictionary_insert(l, words_present[i]);
		assert_int_equal(result, 1);
	}

	/*
	 * wstawianie wszystkich słów, nie powinno się udać
	 */
	for (int i = 0; i < WORDS_PRESENT_COUNT; i++)
	{
		int result = dictionary_insert(l, words_present[i]);
		assert_int_equal(result, 0);
	}

	/*
	 * usuwanie wszystkich słów, powinno się udać
	 */
	for (int i = 0; i < WORDS_PRESENT_COUNT; i++)
	{
		int result = dictionary_delete(l, words_present[i]);
		assert_int_equal(result, 1);
	}

	/*
	 * usuwanie wszystkich słów, nie powinno się udać
	 */
	for (int i = 0; i < WORDS_PRESENT_COUNT; i++)
	{
		int result = dictionary_delete(l, words_present[i]);
		assert_int_equal(result, 0);
	}

	dictionary_done(l);
}

/**
 * ustawienie czystego słownika do testów, zawiera wszystkie słowa z listy
 */
static int dictionary_setup(void **state)
{
	struct dictionary *l = dictionary_new();
	if (!l)
		return -1;

	for (int i = 0; i < WORDS_PRESENT_COUNT; i++)
	{
		int result = dictionary_insert(l, words_present[i]);
		assert_int_equal(result, 1);
	}

	*state = l;
	return 0;
}

/**
 * niszczenie środowiska, delokacja zarezerwowanej pamięci
 */
static int dictionary_teardown(void **state)
{
	struct dictionary *l = *state;
	dictionary_done(l);
	return 0;
}

/**
 * sprawdza czy środowisko zostało ustawione poprawnie
 */
static void dictionary_setup_test(void** state)
{
	struct dictionary *l = *state;

	/*
	 * dodawanie słów powinno się nie udać, słowa są już w słowniku
	 */
	for (int i = 0; i < WORDS_PRESENT_COUNT; i++)
	{
		int result = dictionary_insert(l, words_present[i]);
		assert_int_equal(result, 0);
	}
}

/**
 * sprawdzanie poprawności działania dictionary_find
 */
static void dictionary_find_test(void** state)
{
	struct dictionary *l = *state;

	for (int i = 0; i < WORDS_PRESENT_COUNT; i++)
	{
		int result = dictionary_find(l, words_present[i]);
		assert_int_equal(result, 1);
	}

	for (int i = 0; i < WORDS_NOT_PRESENT_COUNT; i++)
	{
		int result = dictionary_find(l, words_not_present[i]);
		assert_int_equal(result, 0);
	}

	//usuwanie słów ze słownika
	for (int i = 0; i < WORDS_PRESENT_COUNT; i++)
	{
		int result = dictionary_delete(l, words_present[i]);
		assert_int_equal(result, 1);
	}

	//słów nie powinno udać się znaleźć
	for (int i = 0; i < WORDS_PRESENT_COUNT; i++)
	{
		int result = dictionary_find(l, words_present[i]);
		assert_int_equal(result, 0);
	}
}

/**
 * sprawdzanie czy ilość wskazówek oraz ich kolejność dla danych dwóch słów
 * jest prawidłowa
 */
static void dictionary_hint_test(void **state)
{
	struct dictionary *l = *state;
	struct word_list list;

	//word1 hints
	word_list_init(&list);
	dictionary_hints(l, word1, &list);
	assert_int_equal(word_list_size(&list), HINTS_WORD1_COUNT);
	for (int i = 0; i < HINTS_WORD1_COUNT; i++)
		assert_true(wcscmp(word_list_get(&list)[i], hints_word1[i]) == 0);
	word_list_done(&list);

	//word2 hints
	word_list_init(&list);
	dictionary_hints(l, word2, &list);
	assert_int_equal(word_list_size(&list), HINTS_WORD2_COUNT);
	for (int i = 0; i < HINTS_WORD2_COUNT; i++)
		assert_true(wcscmp(word_list_get(&list)[i], hints_word2[i]) == 0);
	word_list_done(&list);

	//word3 hints
	word_list_init(&list);
	dictionary_hints(l, word3, &list);
	assert_int_equal(word_list_size(&list), HINTS_WORD3_COUNT);
	word_list_done(&list);

}


/**
 * główna funkcja uruchamiająca testy
 */
int main(void)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");

	const struct CMUnitTest tests[] =
	{
	cmocka_unit_test(dictionary_simple_add_remove_test),
	cmocka_unit_test_setup_teardown(dictionary_setup_test,
		dictionary_setup, dictionary_teardown),
	cmocka_unit_test_setup_teardown(dictionary_find_test,
			dictionary_setup, dictionary_teardown),
	cmocka_unit_test_setup_teardown(dictionary_hint_test,
			dictionary_setup, dictionary_teardown),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
