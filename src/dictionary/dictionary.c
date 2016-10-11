/** @file
 Implementacja słownika wykorzystująca drzewo TRIE

 @ingroup dictionary
 @author Jakub Pierewoj
 @date 2015-06-01
 */

#include "dictionary.h"
#include "string.h"
#include "word_tree.h"
#include "word_list.h"
#include "conf.h"
#include "dynamic_array.h"
#include "rules.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#define _GNU_SOURCE
#define MAX_HINT_COUNT 20

/**
 Struktura przechowująca słownik.
 Implementacja przy uzycia drzewa TRIE
 */
struct dictionary
{
	struct word_tree tree; ///< Drzewo TRIE przechowujące słowa

	wchar_t * chars_in_dict; ///< Lista przechowująca znaki znajdujące się w drzewie
	int chars_in_dict_size; ///< ilość znaków na liście chars_in_dict
	int chars_in_dict_allocated; ///< rozmiar zaalokowanej pamięci na chars_in_dict

	dynamic_array* rules;
	int max_rule_cost;
};

/** @name Elementy interfejsu 
 @{
 */

struct dictionary * dictionary_new()
{
	struct dictionary *dict = (struct dictionary *) malloc(
			sizeof(struct dictionary));
	dict->chars_in_dict_size = 0;
	dict->chars_in_dict_allocated = 10;
	dict->chars_in_dict = malloc(
			dict->chars_in_dict_allocated * sizeof(wchar_t));
	word_tree_init(&dict->tree);
	dict->rules = array_create();
	return dict;
}


void dictionary_done(struct dictionary *dict)
{
	word_tree_done(&(dict->tree));
	array_done(dict->rules);
	free(dict->chars_in_dict);
	free(dict);
}


bool dictionary_find(const struct dictionary *dict, const wchar_t* word)
{
	int result = word_tree_find(&dict->tree, word);

	if (result == 1)
		return true;
	else
		return false;
}


int dictionary_insert(struct dictionary *dict, const wchar_t *word)
{
	int result = 0;
	if (dictionary_find(dict, word))
		result = 0;
	else
	{
		word_tree_add(&dict->tree, word);
		for (int i = 0; i < wcslen(word); i++)
			add_char_to_list(word[i], &(dict->chars_in_dict),
					&(dict->chars_in_dict_size),
					&(dict->chars_in_dict_allocated));
		result = 1;
	}

	return result;
}


int dictionary_delete(struct dictionary *dict, const wchar_t *word)
{
	int result = word_tree_delete(&dict->tree, word);
	if (result == 1)
		word_tree_delete_redundant(&dict->tree, word);

	return result;
}


int dictionary_save(const struct dictionary *dict, FILE* stream)
{
	fprintf(stream,"%d\n",array_size(dict->rules));
	for(int i=0;i<array_size(dict->rules);i++)
	{
		rule* rule = array_get(dict->rules, i);
		fprintf(stream,"%ls %ls %d %d\n", rule->lhs, rule->rhs, rule->cost, (int) rule->flag);
	}
	word_tree_save(&dict->tree, stream);
	return 0;
}


struct dictionary * dictionary_load(FILE* stream)
{
	struct dictionary *dict = dictionary_new();
	int rule_count = 0;
	fscanf(stream,"%d\n",&rule_count);
	for(int i=0;i<rule_count;i++)
	{
		rule* rule = rule_create(L"",L"",0,RULE_NORMAL);
		fscanf(stream,"%ls %ls %d %d\n", rule->lhs, rule->rhs, &rule->cost, &rule->flag);
		array_add(dict->rules, rule);
	}
	word_tree_load(stream, &dict->tree);
	add_all_chars_to_list(&dict->tree, &(dict->chars_in_dict),
			&(dict->chars_in_dict_size), &(dict->chars_in_dict_allocated));
	return dict;
}


void dictionary_hints(const struct dictionary *dict, const wchar_t* word,
		struct word_list *list)
{
	int hints_found = 0;
	int K = dict->max_rule_cost;
	int word_len = wcslen(word);

	//rules_for_sufix[i] - tablica tablic pasujących reguł dla danej długości sufixu
	dynamic_array **rules_for_sufix = malloc(
			(word_len + 1) * sizeof(dynamic_array));
	for (int i = 0; i <= word_len; i++)
	{
		rules_for_sufix[word_len - i] = array_create();

		//przeglądanie wszystkich reguł
		for (int j = 0; j < array_size(dict->rules); j++)
		{
			rule* rule = array_get(dict->rules, j);
			if (rule_can_apply(word + i, rule))
				array_add(rules_for_sufix[word_len - i], rule);
		}
	}

	//alokowanie tablicy warstw
	dynamic_array **layers = malloc((K + 1) * sizeof(dynamic_array*));
	for (int i = 0; i <= K; i++)
		layers[i] = array_create();

	//wstaw stan (word, root)
	state* base_state = state_create(word,&dict->tree, NULL);
	array_add(layers[0], base_state);

	for (int k = 0; k <= K; k++) //dla każdego kosztu k
	{
		//krok 1
		for (int i = 1; i <= k; i++) //dla każdej warstwy layers[k-i]
		{
			for (int s = 0; s < array_size(layers[k - i]); s++) //dla każdego stanu
			{
				state* current_state = array_get(layers[k - i], s);
				int sufix_len = wcslen(current_state->sufix);
				dynamic_array* applicable_rules = rules_for_sufix[sufix_len];

				//dla każdej zasady pasującej do sufixu
				for (int r = 0; r < array_size(applicable_rules); r++)
				{
					rule* current_rule = array_get(applicable_rules, r);
					if (current_rule->cost == i) //jeżeli koszt zasady pasuje
					{
						dynamic_array *new_states = rule_apply(current_rule,
								current_state);
						array_add_all(layers[k], new_states);
						array_done(new_states);
					}
				}
			}
		}

		//krok 1.5 - rozwijanie k-tej warstwy
		int former_layer_size = array_size(layers[k]);
		for (int i = 0; i < former_layer_size; i++)
		{
			dynamic_array *new_states = state_expand(array_get(layers[k], i));
			array_add_all(layers[k], new_states);
			array_done(new_states);
		}

		//krok 2
		sort_for_uniq(layers[k]);

		bool *to_be_deleted = malloc(array_size(layers[k]) * sizeof(bool));

		/*
		 * sprawdzanie ktore elementy usunac
		 * można to robić nieco optymalniej,
		 * dla każdej warstwy trzymać index i przesuwać go w prawo
		 * ale będzie to mniej czytelne
		 */
		for (int s = 0; s < array_size(layers[k]); s++)
		{
			state* current_state = array_get(layers[k], s);

			//usuń stan jeżeli jest taki sam w którejś z warstw o niższym koście
			for (int i = 0; i < k; i++)
			{
				if (bin_search_uniq(layers[i], current_state) != -1)
				{
					to_be_deleted[s] = true;
					break;
				}
			}

			//usuń stan jeżeli jest taki sam wcześniej w obecnej warstwie
			if (bin_search_uniq(layers[k], current_state) != s)
				to_be_deleted[s] = true;
		}

		//usuwanie
		dynamic_array *after_uniq = array_create();
		for (int s = 0; s < array_size(layers[k]); s++)
		{
			state* current_state = array_get(layers[k], s);
			if (!to_be_deleted[s])
				array_add(after_uniq, current_state);
			else
				state_done(current_state);
		}
		array_done(layers[k]);
		free(to_be_deleted);

		layers[k] = after_uniq;

		hints_found += count_final_states(layers[k]);

		if (hints_found >= MAX_HINT_COUNT)
			break;
	}

	//generowanie wyniku
	struct word_list *all_hints = malloc(sizeof(struct word_list));
	word_list_init(all_hints);
	word_list_init(list); //wynikowa lista słów

	//dodawanie wszystkich wygenerowanych podpowiedzi do all_hints
	for (int i = 0; i <= K; i++)
	{
		for (int s = 0; s < array_size(layers[i]); s++)
		{
			state* current_state = array_get(layers[i], s);
			if (state_is_final(current_state))
			{
				word_list_add(all_hints, calculate_hint(current_state));
			}
		}
	}

	//sortowanie all_hints leksykograficznie
	word_list_sort(all_hints);

	//kopiowanie pierwszych 20 podpowiedzi
	for (int i = 0; i < word_list_size(all_hints); i++)
	{
		if (i > 20)
			break;
		word_list_add(list, word_list_get(all_hints)[i]);
	}

	word_list_done(all_hints);
	free(all_hints);
	free(rules_for_sufix);
}


int dictionary_lang_list(char **list, size_t *list_len)
{
	/*
	 * implementacja bazuje na jednej z odpowiedziach zawartych w wątkach
	 * http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
	 * http://stackoverflow.com/questions/3536781/accessing-directories-in-c/3536853#3536853
	 */
	*list_len = 0;
	size_t allocated = 10;
	*list = malloc(allocated * sizeof(char));
	(*list)[0] = '\0';

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(CONF_PATH)) != NULL)
	{
		/* dodaj wszystkie pliki z katalogu do listy */
		while ((ent = readdir(dir)) != NULL)
		{
			char* filename = ent->d_name;

			if (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0)
			{
				for (int i = 0; i <= strlen(filename); i++)
				{
					if (*list_len >= allocated)
					{
						allocated *= 2;

						char* new_list = malloc(allocated * sizeof(char));
						for (int j = 0; j < *list_len; j++)
							new_list[j] = (*list)[j];
						free(*list);
						*list = new_list;
					}

					(*list)[*list_len] = filename[i];
					(*list_len)++;
				}
			}
		}
		closedir(dir);
		if (*list_len == 0)
			*list_len = 1;
		return 0;
	}
	else
	{
		/* nie udało się otworzyć katalogu */
		return -1;
	}
}


struct dictionary * dictionary_load_lang(const char *lang)
{
	char* path = malloc((strlen(lang) + strlen(CONF_PATH) + 5) * sizeof(char));
	strcpy(path, CONF_PATH);
	strcat(path, "/");
	strcat(path, lang);
	FILE* f = fopen(path, "r");
	free(path);

	if (f == NULL)
	{
		return NULL;
	}

	struct dictionary *dict = dictionary_new();
	int res = word_tree_load(f, &dict->tree);
	fclose(f);

	if (res == 0)
		return dict;
	else
	{
		dictionary_done(dict);
		return NULL;
	}

}


int dictionary_save_lang(const struct dictionary *dict, const char *lang)
{
//Brak folderu
	struct stat st =
	{ 0 };
	if (stat(CONF_PATH, &st) == -1)
		mkdir(CONF_PATH, 0700);

	char* path = malloc((strlen(lang) + strlen(CONF_PATH) + 5) * sizeof(char));
	strcpy(path, CONF_PATH);
	strcat(path, "/");
	strcat(path, lang);
	FILE* f = fopen(path, "w");
	free(path);

	if (f == NULL)
	{
		return -1;
	}

	dictionary_save(dict, f);
	fclose(f);
	return 0;
}


int dictionary_hints_max_cost(struct dictionary *dict, int new_cost)
{
	int prev_cost = dict->max_rule_cost;
	dict->max_rule_cost = new_cost;

	return prev_cost;
}


void dictionary_rule_clear(struct dictionary *dict)
{
	for(int i=0;i<array_size(dict->rules);i++)
		rule_done(array_get(dict->rules, i));
	array_done(dict->rules);
	dict->rules = array_create();
}


int dictionary_rule_add(struct dictionary *dict, const wchar_t *left,
		const wchar_t *right,
		bool bidirectional, int cost, enum rule_flag flag)
{
	struct rule *new_rule = rule_create(left, right, cost, flag);
	array_add(dict->rules, new_rule);
	if (bidirectional)
		dictionary_rule_add(dict, right, left, false, cost, flag);

	if(bidirectional)
		return 2;
	else return 1;
}

/**@}*/
