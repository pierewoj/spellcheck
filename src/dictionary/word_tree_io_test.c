/** @file
 Testy zapisywania słownika do pliku i wczytywania.

 @ingroup dictionary
 @author Jakub Pierewoj
 @date 2015-05-29
 */

///define powodujący zastosowanie atrap funkcji IO
#define UNIT_TESTING

#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <stdbool.h>
#include <assert.h>
#include <locale.h>
#include "utils.h"
#include "word_tree.c"

///maksymalny rozmiar bufora
#define BUF_SIZE 256

///bufor
char buf[BUF_SIZE];

///indeks zapisu bufora
int buf_write_index = 0;

///indeks odczytu bufora
int buf_read_index = 0;

/**
 * funkcja resetująca indeks zapisu bufora, należy ją uruchomić po zakończeniu zapisu
 */
void finish_writing_to_buf()
{
	buf[buf_write_index] = '\0';
	buf_write_index = 0;
}

/**
 * funkcja resetująca indeks odczytu bufora, należy ją uruchomić po zakończeniu odczytu
 */
void finish_reading_buf()
{
	buf_read_index = 0;
}

/**
 * atrapa funkcji fgetwc
 * wczytuje z bufora
 */
wint_t mock_fgetwc(FILE* stream)
{
	wchar_t result;
	//int mbtowc (wchar_t* pwc, const char* pmb, size_t max);
	buf_read_index += mbtowc(&result, buf + buf_read_index,
			BUF_SIZE - buf_read_index);
	if (buf_read_index > BUF_SIZE)
		assert(false);
	return result;
}

/**
 * atrapa funkcji fprintf,
 * zapisuje do bufora
 */
int mock_fprintf(FILE* const file, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	int return_value = vsnprintf(buf + buf_write_index, sizeof(buf), format,
			args);
	buf_write_index += return_value;
	if (buf_write_index >= BUF_SIZE)
		assert(false);
	return return_value;
}

/**
 * ustawienie czystego drzewa do testów, zawiera wszystkie słowa z listy
 */
static int word_tree_setup(void **state)
{
	struct word_tree *tree = malloc(sizeof(struct word_tree));
	word_tree_init(tree);

	finish_reading_buf();
	finish_writing_to_buf();

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
	free(tree);
	return 0;
}

/**
 * prosty test sprawdzający poprawność zapisu do pliku
 */
void word_tree_save_test(void **state)
{
	word_tree_setup(state);
	struct word_tree *tree = *state;
	word_tree_add(tree, L"ąla");
	word_tree_add(tree, L"bla");
	word_tree_add(tree, L"xla");
	word_tree_save(tree, stdout);

	finish_writing_to_buf();
	assert_string_equal(buf, "bla*###xla*###ąla*####");
	word_tree_teardown(state);
}

/**
 * prosty test sprawdzający wczytywanie słownika z pliku
 */
void word_tree_load_test(void **state)
{
	word_tree_setup(state);
	struct word_tree *tree = *state;
	strcpy(buf, "bla*###xla*###ąla*####");
	word_tree_load(stdin, tree);
	finish_reading_buf();

	assert_int_equal(word_tree_find(tree, L"bla"), 1);
	assert_int_equal(word_tree_find(tree, L"xla"), 1);
	assert_int_equal(word_tree_find(tree, L"ąla"), 1);
	assert_int_equal(word_tree_find(tree, L"wla"), 0);

	word_tree_teardown(state);
}

/**
 * test sprawdzający czy słownik to zapisaniu i wczytaniu jest identyczny
 */
void word_tree_complex_io_1(void **state)
{
	const int test_word_count = 6;
	wchar_t *test_words[6] =
	{ L"analiza", L"analityczne", L"analfabeta", L"alfabet", L"anons", L"anonim" };

	//dodawanie słów do drzewa i zapisywanie go do pliku
	word_tree_setup(state);
	struct word_tree *tree = *state;

	for (int i = 0; i < test_word_count; i++)
	{
		word_tree_add(tree, test_words[i]);
	}

	word_tree_save(tree, stdout);
	word_tree_teardown(state);

	//wczytywanie drzewa z pliku i sprawdzanie słów
	word_tree_setup(state);
	tree = *state;

	word_tree_load(stdin, tree);
	for (int i = 0; i < test_word_count; i++)
	{
		assert_int_equal(1, word_tree_find(tree, test_words[i]));
	}

	assert_int_equal(0, word_tree_find(tree, L"zezowaty"));
	word_tree_teardown(state);
}

/**
 * główna funkcja uruchamiająca testy
 */
int main(void)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");

	const struct CMUnitTest tests[] =
	{
	cmocka_unit_test(word_tree_save_test),
	cmocka_unit_test(word_tree_load_test),
	cmocka_unit_test(word_tree_complex_io_1),};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
