# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src

# Utility rule file for lexdiff.

# Include any custom commands dependencies for this target.
include CMakeFiles/lexdiff.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lexdiff.dir/progress.make

CMakeFiles/lexdiff:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "running lex diff (no syntax errors)"
	../scripts/runLEXdiff

lexdiff: CMakeFiles/lexdiff
lexdiff: CMakeFiles/lexdiff.dir/build.make
.PHONY : lexdiff

# Rule to build all files generated by this target.
CMakeFiles/lexdiff.dir/build: lexdiff
.PHONY : CMakeFiles/lexdiff.dir/build

CMakeFiles/lexdiff.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lexdiff.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lexdiff.dir/clean

CMakeFiles/lexdiff.dir/depend:
	cd /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156 /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156 /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src/CMakeFiles/lexdiff.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/lexdiff.dir/depend

