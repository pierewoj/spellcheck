/** @file
 Funckje pomocnicze do generowania podpowiedzi.

 @ingroup dictionary
 @author Jakub Pierewoj
 @date 2015-06-01
 */

#include "rules.h"

/** @name Funkcje pomocnicze
 @{
 */

/**@}*/
/** @name Elementy interfejsu
 @{
 */
state* state_create(const wchar_t* sufix, const struct word_tree* node,
		state* prev_state)
{
	return NULL;
}


void state_done(state* state)
{
}


rule* rule_create(const wchar_t* lhs, const wchar_t* rhs, int cost,
		enum rule_flag flag)
{
	return NULL;
}


void rule_done(rule* rule)
{
}


bool rule_can_apply(const wchar_t *sufix, rule* rule)
{
	return NULL;
}


dynamic_array* rule_apply(state* state, rule* rule)
{
	return NULL;
}


dynamic_array* state_expand(state* state)
{
	return NULL;
}


void sort_for_uniq(dynamic_array* arr)
{
}


int bin_search_uniq(dynamic_array* arr, state* state)
{
	return NULL;
}


bool state_is_final(state* state)
{
	return NULL;
}


int count_final_states(dynamic_array* arr)
{
	return NULL;
}


wchar_t* calculate_hint(state* state)
{
	return NULL;
}
/**@}*/
