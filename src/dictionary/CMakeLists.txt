# dodajemy bibliotekę dictionary, stworzoną na podstawie pliku dictionary.c
# biblioteka będzie dołączana statycznie (czyli przez linkowanie pliku .o)

add_library (dictionary dictionary.c rules.c word_list.c word_tree.c dynamic_array.c)

if (CMOCKA)
    # dodajemy pliki wykonywalne z testami    
    add_executable (word_list_test dynamic_array.c rules.c word_list.c word_list_test.c)
    add_executable (dictionary_test dynamic_array.c rules.c word_list.c word_tree.c dictionary.c dictionary_test.c)
	add_executable (word_tree_test dynamic_array.c rules.c word_list.c word_tree_test.c)
	add_executable (word_tree_io_test dynamic_array.c rules.c word_list.c word_tree_io_test.c utils.h)

    # i linkujemy go z biblioteką do testowania
    target_link_libraries (word_list_test ${CMOCKA})
    target_link_libraries (dictionary_test ${CMOCKA})
    target_link_libraries (word_tree_test ${CMOCKA})
    target_link_libraries (word_tree_io_test ${CMOCKA})

    # wreszcie deklarujemy, że to test
    add_test (word_list_unit_test word_list_test)
    add_test (dictionary_unit_test dictionary_test)
    add_test (word_tree_unit_test word_tree_test)
    add_test (word_tree_io_unit_test word_tree_io_test)
    
endif (CMOCKA)

