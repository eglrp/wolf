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
include src/examples/CMakeFiles/test_ceres_odom_batch.dir/depend.make

# Include the progress variables for this target.
include src/examples/CMakeFiles/test_ceres_odom_batch.dir/progress.make

# Include the compile flags for this target's objects.
include src/examples/CMakeFiles/test_ceres_odom_batch.dir/flags.make

src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o: src/examples/CMakeFiles/test_ceres_odom_batch.dir/flags.make
src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o: ../src/examples/test_ceres_odom_batch.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o"
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o -c /home/jsola/dev/labrobotica/algorithms/wolf/trunk/src/examples/test_ceres_odom_batch.cpp

src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.i"
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jsola/dev/labrobotica/algorithms/wolf/trunk/src/examples/test_ceres_odom_batch.cpp > CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.i

src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.s"
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jsola/dev/labrobotica/algorithms/wolf/trunk/src/examples/test_ceres_odom_batch.cpp -o CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.s

src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o.requires:
.PHONY : src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o.requires

src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o.provides: src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o.requires
	$(MAKE) -f src/examples/CMakeFiles/test_ceres_odom_batch.dir/build.make src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o.provides.build
.PHONY : src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o.provides

src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o.provides.build: src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o

# Object files for target test_ceres_odom_batch
test_ceres_odom_batch_OBJECTS = \
"CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o"

# External object files for target test_ceres_odom_batch
test_ceres_odom_batch_EXTERNAL_OBJECTS =

../bin/test_ceres_odom_batch: src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o
../bin/test_ceres_odom_batch: src/examples/CMakeFiles/test_ceres_odom_batch.dir/build.make
../bin/test_ceres_odom_batch: ../lib/libwolf.so
../bin/test_ceres_odom_batch: /usr/local/lib/libceres.a
../bin/test_ceres_odom_batch: /usr/local/lib/libglog.so
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/libspqr.so
../bin/test_ceres_odom_batch: /usr/lib/libtbb.so
../bin/test_ceres_odom_batch: /usr/lib/libtbbmalloc.so
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/libcholmod.so
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/libccolamd.so
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/libcamd.so
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/libcolamd.so
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/libamd.so
../bin/test_ceres_odom_batch: /usr/lib/liblapack.so
../bin/test_ceres_odom_batch: /usr/lib/libblas.so
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/libsuitesparseconfig.a
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/librt.so
../bin/test_ceres_odom_batch: /usr/lib/liblapack.so
../bin/test_ceres_odom_batch: /usr/lib/libblas.so
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/libsuitesparseconfig.a
../bin/test_ceres_odom_batch: /usr/lib/x86_64-linux-gnu/librt.so
../bin/test_ceres_odom_batch: src/examples/CMakeFiles/test_ceres_odom_batch.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/test_ceres_odom_batch"
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_ceres_odom_batch.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/examples/CMakeFiles/test_ceres_odom_batch.dir/build: ../bin/test_ceres_odom_batch
.PHONY : src/examples/CMakeFiles/test_ceres_odom_batch.dir/build

src/examples/CMakeFiles/test_ceres_odom_batch.dir/requires: src/examples/CMakeFiles/test_ceres_odom_batch.dir/test_ceres_odom_batch.cpp.o.requires
.PHONY : src/examples/CMakeFiles/test_ceres_odom_batch.dir/requires

src/examples/CMakeFiles/test_ceres_odom_batch.dir/clean:
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples && $(CMAKE_COMMAND) -P CMakeFiles/test_ceres_odom_batch.dir/cmake_clean.cmake
.PHONY : src/examples/CMakeFiles/test_ceres_odom_batch.dir/clean

src/examples/CMakeFiles/test_ceres_odom_batch.dir/depend:
	cd /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jsola/dev/labrobotica/algorithms/wolf/trunk /home/jsola/dev/labrobotica/algorithms/wolf/trunk/src/examples /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples /home/jsola/dev/labrobotica/algorithms/wolf/trunk/build/src/examples/CMakeFiles/test_ceres_odom_batch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/examples/CMakeFiles/test_ceres_odom_batch.dir/depend

