/** @file
 Funckje pomocnicze do generowania podpowiedzi.

 @ingroup dictionary
 @author Jakub Pierewoj
 @date 2015-06-01
 */

#ifndef SRC_DICTIONARY_RULES_H_
#define SRC_DICTIONARY_RULES_H_

#include "dynamic_array.h"
#include "dictionary.h"
#include "word_tree.h"
#include <stdbool.h>

/// Reguła do generowania podpowiedzi
struct Rule
{
	wchar_t *lhs; ///< lewa strona reguły
	wchar_t *rhs; ///< prawa strona reguły
	int cost; ///< koszt
	enum rule_flag flag; ///< flaga
}typedef rule;


/// Stan podpowiedzi
struct State
{
	wchar_t* sufix; ///< sufiks słowa dla którego generowana jest podpowiedź
	struct word_tree* node; ///< węzeł drzewa TRIE
	struct State *prev_state; ///< wskaźnik na poprzedni stan przed rozcięciem lub null jeżeli nie ma
}typedef state;


/**
 * Funkcja tworząca stan podpowiedzi
 * @param sufix sufiks
 * @param node węzeł
 * @param prev_state poprzedni stan
 * @return nowy stan
 */
state* state_create(const wchar_t* sufix, const struct word_tree* node, state* prev_state);


/**
 * Funkcja zwalniająca pamięć zaalokowaną dla stanu
 * @param state stan
 */
void state_done(state* state);


/**
 * Funkcja tworząca zasadę
 * @param lhs lewa strona
 * @param rhs prawa strona
 * @param cost koszt
 * @param flag flaga
 * @return nowa zasada
 */
rule* rule_create(const wchar_t* lhs, const wchar_t* rhs, int cost, enum rule_flag flag);


/**
 * Funkcja zwalniająca pamięć zaalokowaną dla zasady
 * @param rule zasada
 */
void rule_done(rule* rule);


/**
 * Funkcja sprawdzająca czy daną regułę można zastosować na danym sufiksie
 * @param sufix sufiks
 * @param rule reguła
 * @return true jeżeli się da, false wpp
 */
bool rule_can_apply(const wchar_t *sufix, rule* rule);


/**
 * Funkcja stosująca regułę na danym stanie
 * @param state stan
 * @param rule reguła
 * @return tablica stanów będących efektem zastosowania reguły
 */
dynamic_array* rule_apply(state* state, rule* rule);


/**
 *	Funkcja stosująca rozwijanie danego stanu
 *	@param state stan
 *	@return tablica stanów będących efektem rozwijania danego stanu
 */
dynamic_array* state_expand(state* state);


/**
 * Funkcja sortująca tablicę stanów w celu usunięcia nadmiarowych stanów
 * @param arr tablica stanów
 */
void sort_for_uniq(dynamic_array* arr);


/**
 * Wyszukiwanie binarne elementów w tablicy stanów posortowanej zgodnie z porządkiem
 * zadanym w opisie algorytmu
 * @param arr tablica
 * @param state stan
 * @return pozycja elementu w tablicy lub -1 gdy nie uda się znaleźć
 */
int bin_search_uniq(dynamic_array* arr, state* state);


/**
 * Funkcja sprawdzająca czy stan jest końcowy
 * @param state stan
 * @return true wtw gdy stan jest końcowy
 */
bool state_is_final(state* state);


/**
 * Funkcja licząca ile stanów w tablicy jest końcowe
 * @param arr tablica
 * @return ilość stanów końcowych
 */
int count_final_states(dynamic_array* arr);


/**
 * Funkcja generująca podpowiedź dla danego stanu końcowego
 * @param state stan końcowy
 * @return podpowiedź
 */
wchar_t* calculate_hint(state* state);

#endif /* SRC_DICTIONARY_RULES_H_ */
