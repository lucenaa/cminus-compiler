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

# Utility rule file for ddiff.

# Include any custom commands dependencies for this target.
include CMakeFiles/ddiff.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ddiff.dir/progress.make

CMakeFiles/ddiff:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "running detail diff"
	../scripts/rundetaildiff

ddiff: CMakeFiles/ddiff
ddiff: CMakeFiles/ddiff.dir/build.make
.PHONY : ddiff

# Rule to build all files generated by this target.
CMakeFiles/ddiff.dir/build: ddiff
.PHONY : CMakeFiles/ddiff.dir/build

CMakeFiles/ddiff.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ddiff.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ddiff.dir/clean

CMakeFiles/ddiff.dir/depend:
	cd /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156 /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156 /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src /home/gabriel/ITA/lab-compiladores/lab1/labctc41240814_2156/src/CMakeFiles/ddiff.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ddiff.dir/depend

