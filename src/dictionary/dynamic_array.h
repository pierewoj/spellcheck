/** @file
Implementacja tablicy dynamicznej

 @ingroup dictionary
 @author Jakub Pierewoj
 @date 2015-06-01
 */

#ifndef SRC_DICTIONARY_DYNAMIC_ARRAY_H_
#define SRC_DICTIONARY_DYNAMIC_ARRAY_H_

///Tablica dynamiczna przechowująca void*
struct Dynamic_array
{
	void ** arr;
	int size;
	int allocated;
} typedef dynamic_array;

/**
 * Funkcja tworząca nową tablicę dynamiczną. Zaalokowaną pamięć należy zwolnić przy użyciu funckji array_done
 * @return nowa tablica
 */
dynamic_array* array_create();


/**
 * Funkcja zwalniająca pamięć zarezerwowaną na potrzeby tablicy
 * @param arr tablica
 */
void array_done(dynamic_array* arr);


/**
 * Funkcja dodająca element do tablicy
 * @param arr tablica
 * @param el element
 */
void array_add(dynamic_array* arr, void* el);


/**
 * Funkcja dodająca wszystkie elementy jednaj tablicy do drugiej
 * @param destination tablica do której są dodawane elementy
 * @param source tablica z której są kopiowane
 */
void array_add_all(dynamic_array* destination, dynamic_array *source);


/**
 * Funckja zwracająca rozmiar tablicy
 * @param arr tablica
 * @return rozmiar
 */
int array_size(dynamic_array* arr);


/**
 * Zwraca n-ty element tablicy
 * @param arr tablica
 * @param index nr elementu
 * @return szukany element
 */
void* array_get(dynamic_array* arr, int index);


/**
 * Funckja sortująca elementy talbicy
 * @param arr talbica
 * @param comparator funckja porównująca elementy
 */
void sort(dynamic_array* arr, int (*comparator)(const void *, const void *));


/**
 * Funkcja realizująca wyszukiwanie binarne.
 * @param arr tablica
 * @param begin indeks początkowy
 * @param end indeks końcowy
 * @param comparator funckja porównująca
 * @return indeks szukanego elementu, lub -1 jeżeli nie ma
 */
int bin_search(dynamic_array* arr, int begin, int end,
		int (*comparator)(const void *, const void *));



#endif /* SRC_DICTIONARY_DYNAMIC_ARRAY_H_ */
