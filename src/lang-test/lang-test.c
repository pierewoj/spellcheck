/*
 * lang_test.c
 *
 *  Created on: 30 maj 2015
 *      Author: kuba
 */

#include "dictionary.h"
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

int main()
{
	setlocale(LC_ALL, "pl_PL.UTF-8");
	char* langs;
	size_t len;

	struct dictionary *dict = dictionary_new();
	dictionary_insert(dict, L"a");
	dictionary_insert(dict, L"aa");
	dictionary_insert(dict, L"ab");
	dictionary_save_lang(dict, "pl");

	return 0;
}
