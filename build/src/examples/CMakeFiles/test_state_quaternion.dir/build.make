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
CMAKE_SOURCE_DIR = /home/jsola/dev/labrobotica/algorithms/wolf/trunk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build

# Include any dependencies generated for this target.
include src/examples/CMakeFiles/test_state_quaternion.dir/depend.make

# Include the progress variables for this target.
include src/examples/CMakeFiles/test_state_quaternion.dir/progress.make

# Include the compile flags for this target's objects.
include src/examples/CMakeFiles/test_state_quaternion.dir/flags.make

src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o: src/examples/CMakeFiles/test_state_quaternion.dir/flags.make
src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o: ../src/examples/test_state_quaternion.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o"
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o -c /home/jsola/dev/labrobotica/algorithms/wolf/trunk/src/examples/test_state_quaternion.cpp

src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.i"
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jsola/dev/labrobotica/algorithms/wolf/trunk/src/examples/test_state_quaternion.cpp > CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.i

src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.s"
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jsola/dev/labrobotica/algorithms/wolf/trunk/src/examples/test_state_quaternion.cpp -o CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.s

src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o.requires:
.PHONY : src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o.requires

src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o.provides: src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o.requires
	$(MAKE) -f src/examples/CMakeFiles/test_state_quaternion.dir/build.make src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o.provides.build
.PHONY : src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o.provides

src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o.provides.build: src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o

# Object files for target test_state_quaternion
test_state_quaternion_OBJECTS = \
"CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o"

# External object files for target test_state_quaternion
test_state_quaternion_EXTERNAL_OBJECTS =

../bin/test_state_quaternion: src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o
../bin/test_state_quaternion: src/examples/CMakeFiles/test_state_quaternion.dir/build.make
../bin/test_state_quaternion: ../lib/libwolf.so
../bin/test_state_quaternion: /usr/local/lib/libceres.a
../bin/test_state_quaternion: /usr/local/lib/libglog.so
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/libspqr.so
../bin/test_state_quaternion: /usr/lib/libtbb.so
../bin/test_state_quaternion: /usr/lib/libtbbmalloc.so
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/libcholmod.so
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/libccolamd.so
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/libcamd.so
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/libcolamd.so
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/libamd.so
../bin/test_state_quaternion: /usr/lib/liblapack.so
../bin/test_state_quaternion: /usr/lib/libblas.so
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/libsuitesparseconfig.a
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/librt.so
../bin/test_state_quaternion: /usr/lib/liblapack.so
../bin/test_state_quaternion: /usr/lib/libblas.so
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/libsuitesparseconfig.a
../bin/test_state_quaternion: /usr/lib/x86_64-linux-gnu/librt.so
../bin/test_state_quaternion: src/examples/CMakeFiles/test_state_quaternion.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/test_state_quaternion"
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_state_quaternion.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/examples/CMakeFiles/test_state_quaternion.dir/build: ../bin/test_state_quaternion
.PHONY : src/examples/CMakeFiles/test_state_quaternion.dir/build

src/examples/CMakeFiles/test_state_quaternion.dir/requires: src/examples/CMakeFiles/test_state_quaternion.dir/test_state_quaternion.cpp.o.requires
.PHONY : src/examples/CMakeFiles/test_state_quaternion.dir/requires

src/examples/CMakeFiles/test_state_quaternion.dir/clean:
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && $(CMAKE_COMMAND) -P CMakeFiles/test_state_quaternion.dir/cmake_clean.cmake
.PHONY : src/examples/CMakeFiles/test_state_quaternion.dir/clean

src/examples/CMakeFiles/test_state_quaternion.dir/depend:
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jsola/dev/labrobotica/algorithms/wolf/trunk /home/jsola/dev/labrobotica/algorithms/wolf/trunk/src/examples /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples/CMakeFiles/test_state_quaternion.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/examples/CMakeFiles/test_state_quaternion.dir/depend

