# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lin/pl0compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lin/pl0compiler/build

# Include any dependencies generated for this target.
include syntax/CMakeFiles/tax.dir/depend.make

# Include the progress variables for this target.
include syntax/CMakeFiles/tax.dir/progress.make

# Include the compile flags for this target's objects.
include syntax/CMakeFiles/tax.dir/flags.make

syntax/CMakeFiles/tax.dir/pl0_tax.c.o: syntax/CMakeFiles/tax.dir/flags.make
syntax/CMakeFiles/tax.dir/pl0_tax.c.o: ../syntax/pl0_tax.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lin/pl0compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object syntax/CMakeFiles/tax.dir/pl0_tax.c.o"
	cd /home/lin/pl0compiler/build/syntax && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tax.dir/pl0_tax.c.o   -c /home/lin/pl0compiler/syntax/pl0_tax.c

syntax/CMakeFiles/tax.dir/pl0_tax.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tax.dir/pl0_tax.c.i"
	cd /home/lin/pl0compiler/build/syntax && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lin/pl0compiler/syntax/pl0_tax.c > CMakeFiles/tax.dir/pl0_tax.c.i

syntax/CMakeFiles/tax.dir/pl0_tax.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tax.dir/pl0_tax.c.s"
	cd /home/lin/pl0compiler/build/syntax && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lin/pl0compiler/syntax/pl0_tax.c -o CMakeFiles/tax.dir/pl0_tax.c.s

syntax/CMakeFiles/tax.dir/pl0_tax.c.o.requires:

.PHONY : syntax/CMakeFiles/tax.dir/pl0_tax.c.o.requires

syntax/CMakeFiles/tax.dir/pl0_tax.c.o.provides: syntax/CMakeFiles/tax.dir/pl0_tax.c.o.requires
	$(MAKE) -f syntax/CMakeFiles/tax.dir/build.make syntax/CMakeFiles/tax.dir/pl0_tax.c.o.provides.build
.PHONY : syntax/CMakeFiles/tax.dir/pl0_tax.c.o.provides

syntax/CMakeFiles/tax.dir/pl0_tax.c.o.provides.build: syntax/CMakeFiles/tax.dir/pl0_tax.c.o


# Object files for target tax
tax_OBJECTS = \
"CMakeFiles/tax.dir/pl0_tax.c.o"

# External object files for target tax
tax_EXTERNAL_OBJECTS =

syntax/libtax.a: syntax/CMakeFiles/tax.dir/pl0_tax.c.o
syntax/libtax.a: syntax/CMakeFiles/tax.dir/build.make
syntax/libtax.a: syntax/CMakeFiles/tax.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lin/pl0compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libtax.a"
	cd /home/lin/pl0compiler/build/syntax && $(CMAKE_COMMAND) -P CMakeFiles/tax.dir/cmake_clean_target.cmake
	cd /home/lin/pl0compiler/build/syntax && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tax.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
syntax/CMakeFiles/tax.dir/build: syntax/libtax.a

.PHONY : syntax/CMakeFiles/tax.dir/build

syntax/CMakeFiles/tax.dir/requires: syntax/CMakeFiles/tax.dir/pl0_tax.c.o.requires

.PHONY : syntax/CMakeFiles/tax.dir/requires

syntax/CMakeFiles/tax.dir/clean:
	cd /home/lin/pl0compiler/build/syntax && $(CMAKE_COMMAND) -P CMakeFiles/tax.dir/cmake_clean.cmake
.PHONY : syntax/CMakeFiles/tax.dir/clean

syntax/CMakeFiles/tax.dir/depend:
	cd /home/lin/pl0compiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lin/pl0compiler /home/lin/pl0compiler/syntax /home/lin/pl0compiler/build /home/lin/pl0compiler/build/syntax /home/lin/pl0compiler/build/syntax/CMakeFiles/tax.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : syntax/CMakeFiles/tax.dir/depend

