# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug

# Include any dependencies generated for this target.
include dictionary/CMakeFiles/word_tree_io_test.dir/depend.make

# Include the progress variables for this target.
include dictionary/CMakeFiles/word_tree_io_test.dir/progress.make

# Include the compile flags for this target's objects.
include dictionary/CMakeFiles/word_tree_io_test.dir/flags.make

dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o: dictionary/CMakeFiles/word_tree_io_test.dir/flags.make
dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o: /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src/dictionary/word_list.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o"
	cd /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/word_tree_io_test.dir/word_list.c.o   -c /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src/dictionary/word_list.c

dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/word_tree_io_test.dir/word_list.c.i"
	cd /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src/dictionary/word_list.c > CMakeFiles/word_tree_io_test.dir/word_list.c.i

dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/word_tree_io_test.dir/word_list.c.s"
	cd /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src/dictionary/word_list.c -o CMakeFiles/word_tree_io_test.dir/word_list.c.s

dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o.requires:
.PHONY : dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o.requires

dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o.provides: dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o.requires
	$(MAKE) -f dictionary/CMakeFiles/word_tree_io_test.dir/build.make dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o.provides.build
.PHONY : dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o.provides

dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o.provides.build: dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o

dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o: dictionary/CMakeFiles/word_tree_io_test.dir/flags.make
dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o: /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src/dictionary/word_tree_io_test.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o"
	cd /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o   -c /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src/dictionary/word_tree_io_test.c

dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.i"
	cd /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src/dictionary/word_tree_io_test.c > CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.i

dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.s"
	cd /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src/dictionary/word_tree_io_test.c -o CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.s

dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o.requires:
.PHONY : dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o.requires

dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o.provides: dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o.requires
	$(MAKE) -f dictionary/CMakeFiles/word_tree_io_test.dir/build.make dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o.provides.build
.PHONY : dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o.provides

dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o.provides.build: dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o

# Object files for target word_tree_io_test
word_tree_io_test_OBJECTS = \
"CMakeFiles/word_tree_io_test.dir/word_list.c.o" \
"CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o"

# External object files for target word_tree_io_test
word_tree_io_test_EXTERNAL_OBJECTS =

dictionary/word_tree_io_test: dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o
dictionary/word_tree_io_test: dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o
dictionary/word_tree_io_test: dictionary/CMakeFiles/word_tree_io_test.dir/build.make
dictionary/word_tree_io_test: /usr/local/lib/libcmocka.so
dictionary/word_tree_io_test: dictionary/CMakeFiles/word_tree_io_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable word_tree_io_test"
	cd /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/word_tree_io_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dictionary/CMakeFiles/word_tree_io_test.dir/build: dictionary/word_tree_io_test
.PHONY : dictionary/CMakeFiles/word_tree_io_test.dir/build

dictionary/CMakeFiles/word_tree_io_test.dir/requires: dictionary/CMakeFiles/word_tree_io_test.dir/word_list.c.o.requires
dictionary/CMakeFiles/word_tree_io_test.dir/requires: dictionary/CMakeFiles/word_tree_io_test.dir/word_tree_io_test.c.o.requires
.PHONY : dictionary/CMakeFiles/word_tree_io_test.dir/requires

dictionary/CMakeFiles/word_tree_io_test.dir/clean:
	cd /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary && $(CMAKE_COMMAND) -P CMakeFiles/word_tree_io_test.dir/cmake_clean.cmake
.PHONY : dictionary/CMakeFiles/word_tree_io_test.dir/clean

dictionary/CMakeFiles/word_tree_io_test.dir/depend:
	cd /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/src/dictionary /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary /home/students/inf/j/jp360641/Dokumenty/spellcheck-jp360641/debug/dictionary/CMakeFiles/word_tree_io_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dictionary/CMakeFiles/word_tree_io_test.dir/depend

