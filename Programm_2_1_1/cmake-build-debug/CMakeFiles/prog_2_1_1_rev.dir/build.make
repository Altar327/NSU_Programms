# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.16

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\CLion\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = D:\CLion\bin\cmake\win\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\User\CLionProjects\Programm_2_1_1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\User\CLionProjects\Programm_2_1_1\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/prog_2_1_1_rev.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/prog_2_1_1_rev.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/prog_2_1_1_rev.dir/flags.make

CMakeFiles/prog_2_1_1_rev.dir/main.c.obj: CMakeFiles/prog_2_1_1_rev.dir/flags.make
CMakeFiles/prog_2_1_1_rev.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\User\CLionProjects\Programm_2_1_1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/prog_2_1_1_rev.dir/main.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\prog_2_1_1_rev.dir\main.c.obj   -c C:\Users\User\CLionProjects\Programm_2_1_1\main.c

CMakeFiles/prog_2_1_1_rev.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/prog_2_1_1_rev.dir/main.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\User\CLionProjects\Programm_2_1_1\main.c > CMakeFiles\prog_2_1_1_rev.dir\main.c.i

CMakeFiles/prog_2_1_1_rev.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/prog_2_1_1_rev.dir/main.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\User\CLionProjects\Programm_2_1_1\main.c -o CMakeFiles\prog_2_1_1_rev.dir\main.c.s

# Object files for target prog_2_1_1_rev
prog_2_1_1_rev_OBJECTS = \
"CMakeFiles/prog_2_1_1_rev.dir/main.c.obj"

# External object files for target prog_2_1_1_rev
prog_2_1_1_rev_EXTERNAL_OBJECTS =

prog_2_1_1_rev.exe: CMakeFiles/prog_2_1_1_rev.dir/main.c.obj
prog_2_1_1_rev.exe: CMakeFiles/prog_2_1_1_rev.dir/build.make
prog_2_1_1_rev.exe: CMakeFiles/prog_2_1_1_rev.dir/linklibs.rsp
prog_2_1_1_rev.exe: CMakeFiles/prog_2_1_1_rev.dir/objects1.rsp
prog_2_1_1_rev.exe: CMakeFiles/prog_2_1_1_rev.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\User\CLionProjects\Programm_2_1_1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable prog_2_1_1_rev.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\prog_2_1_1_rev.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/prog_2_1_1_rev.dir/build: prog_2_1_1_rev.exe

.PHONY : CMakeFiles/prog_2_1_1_rev.dir/build

CMakeFiles/prog_2_1_1_rev.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\prog_2_1_1_rev.dir\cmake_clean.cmake
.PHONY : CMakeFiles/prog_2_1_1_rev.dir/clean

CMakeFiles/prog_2_1_1_rev.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\User\CLionProjects\Programm_2_1_1 C:\Users\User\CLionProjects\Programm_2_1_1 C:\Users\User\CLionProjects\Programm_2_1_1\cmake-build-debug C:\Users\User\CLionProjects\Programm_2_1_1\cmake-build-debug C:\Users\User\CLionProjects\Programm_2_1_1\cmake-build-debug\CMakeFiles\prog_2_1_1_rev.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/prog_2_1_1_rev.dir/depend

