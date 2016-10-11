/** @defgroup dict-check Moduł dict-check
 Moduł umożliwiający znajdowanie w tekście słów
 które nie znajdują się w słowniku.
 */
/** @file
 Główny plik modułu dict-check
 @ingroup dict-check
 @author Jakub Pierewoj
 @date 2015-05-23
 */

#include "dictionary.h"
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

/** maksymalna długość słowa jakie pojawi się w tekście */
#define MAXWLEN 200

/**
 Otwieranie pliku
 @param[in] path ścieżka do pliku
 @param[in,out] dict słownik
 */
void try_read_dict_file(char *path, struct dictionary **dict)
{
	FILE* f = fopen(path, "r");
	if (f == NULL)
	{
		puts("failed to open dictionary file");
		exit(1);
	}
	*dict = dictionary_load(f);
	if (*dict == NULL)
	{
		puts("failed to parse dictionary from file");
		exit(1);
	}
	fclose(f);
}

/** Zamienia słowo na złożone z małych liter.
 @param[in,out] word Modyfikowane słowo.
 @return 0, jeśli słowo nie jest złożone z samych liter, 1 w p.p.
 */
int make_lowercase(wchar_t *word)
{
	for (wchar_t *w = word; *w; ++w)
		if (!iswalpha(*w))
			return 0;
		else
			*w = towlower(*w);
	return 1;
}

/** Wypisuje podpowiedzi na stderr
 @param word słowo dla którego wypisywane są hinty
 @param dict słownik
 */
void print_hints_to_stderr(wchar_t *word, const struct dictionary *dict)
{
	struct word_list list;
	word_list_init(&list);
	dictionary_hints(dict, word, &list);
	const wchar_t * const *a = word_list_get(&list);
	for (size_t i = 0; i < word_list_size(&list); ++i)
	{
		fprintf(stderr, "%ls", a[i]);
		if (i != word_list_size(&list) - 1)
			fprintf(stderr, " ");
	}
	word_list_done(&list);
}

/**
 Główna funkcja programu dict-check.
 @param argc ilość argumentów
 @param argv argumenty; wywołanie z parametrem -v powoduje wypisywanie hintów na stderr
 */
int main(int argc, char **argv)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");
	struct dictionary *dict = dictionary_new();
	bool printStderr = false;
	if (argc == 2)
	{
		try_read_dict_file(argv[1], &dict);
	}
	else if (argc == 3)
	{
		if (strcmp("-v", argv[1]) == 0)
			printStderr = true;
		try_read_dict_file(argv[2], &dict);
	}
	else
	{
		//Złe argumenty
		exit(69);
	}

	wchar_t c;
	wchar_t word[MAXWLEN], wordLowerCase[MAXWLEN];
	int lineNumber = 1, charNum = 0, charWordBegin = 0, wordLen = 0;

	while (1)
	{
		c = getwchar();
		charNum++;

		if (iswalpha(c))
		{
			if (wordLen == 0)
			{
				charWordBegin = charNum;
			}
			word[wordLen] = c;
			wordLen++;

			if (wordLen >= MAXWLEN)
				exit(1);
		}
		else
		{
			if (wordLen > 0)
			{
				word[wordLen] = L'\0';
				wcscpy(wordLowerCase, word);
				make_lowercase(wordLowerCase);

				if (!dictionary_find(dict, wordLowerCase))
				{
					printf("#");
					if (printStderr)
					{
						fprintf(stderr, "%d,%d %ls: ", lineNumber,
								charWordBegin, word);
						print_hints_to_stderr(wordLowerCase, dict);
						fprintf(stderr, "\n");
					}

				}
				printf("%ls", word);
				wordLen = 0;

			}
			if (c == L'\n')
			{
				lineNumber++;
				charNum = 0;
			}
			printf("%lc", c);
		}

		if (c == EOF)
			break;
	}

	dictionary_done(dict);

	return 0;
}
