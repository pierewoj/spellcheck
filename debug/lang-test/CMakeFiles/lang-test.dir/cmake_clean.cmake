FILE(REMOVE_RECURSE
  "CMakeFiles/lang-test.dir/lang-test.c.o"
  "lang-test.pdb"
  "lang-test"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/lang-test.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
