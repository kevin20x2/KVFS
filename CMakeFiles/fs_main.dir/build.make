# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /opt/cmake-3.9.1/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.9.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/kvfs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/kvfs

# Include any dependencies generated for this target.
include CMakeFiles/fs_main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fs_main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fs_main.dir/flags.make

CMakeFiles/fs_main.dir/fs_main.cpp.o: CMakeFiles/fs_main.dir/flags.make
CMakeFiles/fs_main.dir/fs_main.cpp.o: fs_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/kvfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fs_main.dir/fs_main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fs_main.dir/fs_main.cpp.o -c /root/kvfs/fs_main.cpp

CMakeFiles/fs_main.dir/fs_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fs_main.dir/fs_main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/kvfs/fs_main.cpp > CMakeFiles/fs_main.dir/fs_main.cpp.i

CMakeFiles/fs_main.dir/fs_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fs_main.dir/fs_main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/kvfs/fs_main.cpp -o CMakeFiles/fs_main.dir/fs_main.cpp.s

CMakeFiles/fs_main.dir/fs/adaptor.cpp.o: CMakeFiles/fs_main.dir/flags.make
CMakeFiles/fs_main.dir/fs/adaptor.cpp.o: fs/adaptor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/kvfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/fs_main.dir/fs/adaptor.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fs_main.dir/fs/adaptor.cpp.o -c /root/kvfs/fs/adaptor.cpp

CMakeFiles/fs_main.dir/fs/adaptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fs_main.dir/fs/adaptor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/kvfs/fs/adaptor.cpp > CMakeFiles/fs_main.dir/fs/adaptor.cpp.i

CMakeFiles/fs_main.dir/fs/adaptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fs_main.dir/fs/adaptor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/kvfs/fs/adaptor.cpp -o CMakeFiles/fs_main.dir/fs/adaptor.cpp.s

CMakeFiles/fs_main.dir/fs/config.cpp.o: CMakeFiles/fs_main.dir/flags.make
CMakeFiles/fs_main.dir/fs/config.cpp.o: fs/config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/kvfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/fs_main.dir/fs/config.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fs_main.dir/fs/config.cpp.o -c /root/kvfs/fs/config.cpp

CMakeFiles/fs_main.dir/fs/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fs_main.dir/fs/config.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/kvfs/fs/config.cpp > CMakeFiles/fs_main.dir/fs/config.cpp.i

CMakeFiles/fs_main.dir/fs/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fs_main.dir/fs/config.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/kvfs/fs/config.cpp -o CMakeFiles/fs_main.dir/fs/config.cpp.s

CMakeFiles/fs_main.dir/fs/inode.cpp.o: CMakeFiles/fs_main.dir/flags.make
CMakeFiles/fs_main.dir/fs/inode.cpp.o: fs/inode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/kvfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/fs_main.dir/fs/inode.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fs_main.dir/fs/inode.cpp.o -c /root/kvfs/fs/inode.cpp

CMakeFiles/fs_main.dir/fs/inode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fs_main.dir/fs/inode.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/kvfs/fs/inode.cpp > CMakeFiles/fs_main.dir/fs/inode.cpp.i

CMakeFiles/fs_main.dir/fs/inode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fs_main.dir/fs/inode.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/kvfs/fs/inode.cpp -o CMakeFiles/fs_main.dir/fs/inode.cpp.s

CMakeFiles/fs_main.dir/fs/kvfs.cpp.o: CMakeFiles/fs_main.dir/flags.make
CMakeFiles/fs_main.dir/fs/kvfs.cpp.o: fs/kvfs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/kvfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/fs_main.dir/fs/kvfs.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fs_main.dir/fs/kvfs.cpp.o -c /root/kvfs/fs/kvfs.cpp

CMakeFiles/fs_main.dir/fs/kvfs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fs_main.dir/fs/kvfs.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/kvfs/fs/kvfs.cpp > CMakeFiles/fs_main.dir/fs/kvfs.cpp.i

CMakeFiles/fs_main.dir/fs/kvfs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fs_main.dir/fs/kvfs.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/kvfs/fs/kvfs.cpp -o CMakeFiles/fs_main.dir/fs/kvfs.cpp.s

CMakeFiles/fs_main.dir/util/hash_function.cpp.o: CMakeFiles/fs_main.dir/flags.make
CMakeFiles/fs_main.dir/util/hash_function.cpp.o: util/hash_function.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/kvfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/fs_main.dir/util/hash_function.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fs_main.dir/util/hash_function.cpp.o -c /root/kvfs/util/hash_function.cpp

CMakeFiles/fs_main.dir/util/hash_function.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fs_main.dir/util/hash_function.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/kvfs/util/hash_function.cpp > CMakeFiles/fs_main.dir/util/hash_function.cpp.i

CMakeFiles/fs_main.dir/util/hash_function.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fs_main.dir/util/hash_function.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/kvfs/util/hash_function.cpp -o CMakeFiles/fs_main.dir/util/hash_function.cpp.s

# Object files for target fs_main
fs_main_OBJECTS = \
"CMakeFiles/fs_main.dir/fs_main.cpp.o" \
"CMakeFiles/fs_main.dir/fs/adaptor.cpp.o" \
"CMakeFiles/fs_main.dir/fs/config.cpp.o" \
"CMakeFiles/fs_main.dir/fs/inode.cpp.o" \
"CMakeFiles/fs_main.dir/fs/kvfs.cpp.o" \
"CMakeFiles/fs_main.dir/util/hash_function.cpp.o"

# External object files for target fs_main
fs_main_EXTERNAL_OBJECTS =

fs_main: CMakeFiles/fs_main.dir/fs_main.cpp.o
fs_main: CMakeFiles/fs_main.dir/fs/adaptor.cpp.o
fs_main: CMakeFiles/fs_main.dir/fs/config.cpp.o
fs_main: CMakeFiles/fs_main.dir/fs/inode.cpp.o
fs_main: CMakeFiles/fs_main.dir/fs/kvfs.cpp.o
fs_main: CMakeFiles/fs_main.dir/util/hash_function.cpp.o
fs_main: CMakeFiles/fs_main.dir/build.make
fs_main: CMakeFiles/fs_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/kvfs/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable fs_main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fs_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fs_main.dir/build: fs_main

.PHONY : CMakeFiles/fs_main.dir/build

CMakeFiles/fs_main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fs_main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fs_main.dir/clean

CMakeFiles/fs_main.dir/depend:
	cd /root/kvfs && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/kvfs /root/kvfs /root/kvfs /root/kvfs /root/kvfs/CMakeFiles/fs_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fs_main.dir/depend
