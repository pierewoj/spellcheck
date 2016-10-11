/** @file
 Definicje atrap stosowanych do testowania IO przy pomocy CMocka


 @ingroup dictionary
 @author Jakub Pierewoj
 @date 2015-05-29
 */

#include<stdio.h>
#include<assert.h>
#include<stdbool.h>
#include<wchar.h>

#ifndef UTILS_H
#define UTILS_H

#ifdef UNIT_TESTING

///atrapa fprintf
#define fprintf mock_fprintf

///atrapa fgetwc
#define fgetwc mock_fgetwc

///dekleracja atrapy, definicja znajduje się w word_tree_io_test.c
extern wint_t mock_fgetwc(FILE* stream);

///dekleracja atrapy, definicja znajduje się w word_tree_io_test.c
extern int mock_fprintf(FILE* const file, const char *format, ...);

#endif /* UNIT_TESTING */
#endif /* UTILS_H*/

