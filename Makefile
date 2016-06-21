# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.5.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.5.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ben/work/games/Listener

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ben/work/games/Listener

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target test
test:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running tests..."
	/usr/local/Cellar/cmake/3.5.2/bin/ctest --force-new-ctest-process $(ARGS)
.PHONY : test

# Special rule for the target test
test/fast: test

.PHONY : test/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/local/Cellar/cmake/3.5.2/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: install/local

.PHONY : install/local/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/3.5.2/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/3.5.2/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/local/Cellar/cmake/3.5.2/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/local/Cellar/cmake/3.5.2/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/ben/work/games/Listener/CMakeFiles /Users/ben/work/games/Listener/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/ben/work/games/Listener/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named rtaudio_external

# Build rule for target.
rtaudio_external: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 rtaudio_external
.PHONY : rtaudio_external

# fast build rule for target.
rtaudio_external/fast:
	$(MAKE) -f CMakeFiles/rtaudio_external.dir/build.make CMakeFiles/rtaudio_external.dir/build
.PHONY : rtaudio_external/fast

#=============================================================================
# Target rules for targets named libpd_external

# Build rule for target.
libpd_external: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 libpd_external
.PHONY : libpd_external

# fast build rule for target.
libpd_external/fast:
	$(MAKE) -f CMakeFiles/libpd_external.dir/build.make CMakeFiles/libpd_external.dir/build
.PHONY : libpd_external/fast

#=============================================================================
# Target rules for targets named Main

# Build rule for target.
Main: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 Main
.PHONY : Main

# fast build rule for target.
Main/fast:
	$(MAKE) -f CMakeFiles/Main.dir/build.make CMakeFiles/Main.dir/build
.PHONY : Main/fast

Audio.o: Audio.cpp.o

.PHONY : Audio.o

# target to build an object file
Audio.cpp.o:
	$(MAKE) -f CMakeFiles/Main.dir/build.make CMakeFiles/Main.dir/Audio.cpp.o
.PHONY : Audio.cpp.o

Audio.i: Audio.cpp.i

.PHONY : Audio.i

# target to preprocess a source file
Audio.cpp.i:
	$(MAKE) -f CMakeFiles/Main.dir/build.make CMakeFiles/Main.dir/Audio.cpp.i
.PHONY : Audio.cpp.i

Audio.s: Audio.cpp.s

.PHONY : Audio.s

# target to generate assembly for a file
Audio.cpp.s:
	$(MAKE) -f CMakeFiles/Main.dir/build.make CMakeFiles/Main.dir/Audio.cpp.s
.PHONY : Audio.cpp.s

Main.o: Main.cpp.o

.PHONY : Main.o

# target to build an object file
Main.cpp.o:
	$(MAKE) -f CMakeFiles/Main.dir/build.make CMakeFiles/Main.dir/Main.cpp.o
.PHONY : Main.cpp.o

Main.i: Main.cpp.i

.PHONY : Main.i

# target to preprocess a source file
Main.cpp.i:
	$(MAKE) -f CMakeFiles/Main.dir/build.make CMakeFiles/Main.dir/Main.cpp.i
.PHONY : Main.cpp.i

Main.s: Main.cpp.s

.PHONY : Main.s

# target to generate assembly for a file
Main.cpp.s:
	$(MAKE) -f CMakeFiles/Main.dir/build.make CMakeFiles/Main.dir/Main.cpp.s
.PHONY : Main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... test"
	@echo "... install/local"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... rtaudio_external"
	@echo "... list_install_components"
	@echo "... install"
	@echo "... libpd_external"
	@echo "... Main"
	@echo "... Audio.o"
	@echo "... Audio.i"
	@echo "... Audio.s"
	@echo "... Main.o"
	@echo "... Main.i"
	@echo "... Main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

