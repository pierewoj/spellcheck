# deklarujemy plik wykonywalny tworzony na podstawie odpowiedniego pliku źródłowego
add_executable (lang-test lang-test.c)

# przy kompilacji programu należy dołączyć bibliotekę
target_link_libraries (lang-test dictionary)
