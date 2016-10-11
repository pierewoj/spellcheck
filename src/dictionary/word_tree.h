/** @file
 Interfejs drzewa słów

 @ingroup dictionary
 @author Jakub Pierewoj
 @copyright Jakub Pierewoj
 @date 2015-05-23
 */

#ifndef SRC_DICTIONARY_WORD_TREE_H_
#define SRC_DICTIONARY_WORD_TREE_H_

#include <wchar.h>
#include <stdbool.h>

/**
 Struktura przechowująca slowa, drzewo TRIE.
 */
struct word_tree
{
	wchar_t znak; ///< znak znajdujący się w węźle lub '\0' gdy jest to korzeń
	bool is_word; ///< czy w węźle kończy się słowo
	struct word_tree** next; ///< lista dzieci
	int list_length; ///< długość listy dzieci
	struct word_tree* parent;
};

/**
 Inicjuje drzewo slow.
 @param[in,out] tree Drzewo slow.
 */
void word_tree_init(struct word_tree *tree);

/**
 Destrukcja drzewa słów.
 @param[in,out] tree Drzewo słów.
 */
void word_tree_done(struct word_tree *tree);

/**
 Dodaje słowo do drzewa.
 @param[in,out] tree Drzewo słów.
 @param[in] word Dodawane słowo.
 @return 1 jeśli się udało, 0 w p.p.
 */
int word_tree_add(struct word_tree *tree, const wchar_t *word);

/**
 Usuwa slowo z drzewa. Po tej operacji mogą pozostać w drzewie nadmiarowe węzły.
 Należy je usunąć używając funkcji word_tree_delete_redundant.
 @param[in,out] tree Drzewo słów.
 @param[in] word Usuwane słowo.
 @return 1 jeśli się udało, 0 w p.p.
 */
int word_tree_delete(struct word_tree *tree, const wchar_t *word);

/**
 Usuwa nadmiarowe węzły z drzewa powstałe w wyniku dodawania słowa word
 @param[in,out] tree Drzewo słów.
 @param[in] word Usuwane słowo.
 @return true jeżeli aktualny węzeł nie posiada dzieci oraz nie kończy się w nim słowo
 */
bool word_tree_delete_redundant(struct word_tree *tree, const wchar_t *word);

/**
 Sprawdza czy slowo znajduje sie w drzewie
 @param[in,out] tree Drzewo słów.
 @param[in] word Szukane slowo
 @return 1 jeśli się udało znalezc, 0 w p.p.
 */
int word_tree_find(const struct word_tree *tree, const wchar_t *word);

/**
 Zapisuje drzewo slow do pliku
 @param[in] tree drzewo
 @param[in,out] stream Strumień, gdzie ma być zapisane drzewo.
 @return <0 jeśli operacja się nie powiedzie, 0 w p.p.
 */
void word_tree_save(const struct word_tree *tree, FILE* stream);

/**
 Wczytuje drzewo
 @param[in,out] stream Strumień, skąd ma być wczytane drzewo.
 @param tree wskaźnik do drzewa, które ma być wczytane
 @return 0 gdy się udało, 1 wpp
 */
int word_tree_load(FILE* stream, struct word_tree* tree);

/**
 Dodaje do listy jeden znak, jeżeli nie ma go na liście
 @param[in] ch znak
 @param[in,out] list lista znaków
 @param[in,out] size ilość znaków na liście
 @param[in,out] allocated ilość zaalokowanej pamięci
 */
void add_char_to_list(wchar_t ch, wchar_t **list, int* size, int *allocated);

/**
 Dodaje do listy wszystkie znaki jakie wystąpiły w danym drzewie
 @param[in] tree drzewo
 @param[in,out] list lista znaków
 @param[in,out] size ilość znaków na liście
 @param[in,out] allocated ilość zaalokowanej pamięci
 */
void add_all_chars_to_list(const struct word_tree *tree, wchar_t **list,
		int *size, int *allocated);

#endif /* SRC_DICTIONARY_WORD_TREE_H_ */
