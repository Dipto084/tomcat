# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/runner/work/tomcat/tomcat/docs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/runner/work/tomcat/tomcat/docs/build

# Utility rule file for java_apidocs.

# Include any custom commands dependencies for this target.
include developer/java_api/CMakeFiles/java_apidocs.dir/compiler_depend.make

# Include the progress variables for this target.
include developer/java_api/CMakeFiles/java_apidocs.dir/progress.make

developer/java_api/CMakeFiles/java_apidocs: developer/java_api/Doxyfile.java_apidocs
developer/java_api/CMakeFiles/java_apidocs: ../developer/java_api/README.md
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/runner/work/tomcat/tomcat/docs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating documentation with Doxygen..."
	cd /home/runner/work/tomcat/tomcat/docs/developer/java_api && /usr/local/bin/cmake -E make_directory /home/runner/work/tomcat/tomcat/docs/build/developer/java_api
	cd /home/runner/work/tomcat/tomcat/docs/developer/java_api && /usr/bin/doxygen /home/runner/work/tomcat/tomcat/docs/build/developer/java_api/Doxyfile.java_apidocs

java_apidocs: developer/java_api/CMakeFiles/java_apidocs
java_apidocs: developer/java_api/CMakeFiles/java_apidocs.dir/build.make
.PHONY : java_apidocs

# Rule to build all files generated by this target.
developer/java_api/CMakeFiles/java_apidocs.dir/build: java_apidocs
.PHONY : developer/java_api/CMakeFiles/java_apidocs.dir/build

developer/java_api/CMakeFiles/java_apidocs.dir/clean:
	cd /home/runner/work/tomcat/tomcat/docs/build/developer/java_api && $(CMAKE_COMMAND) -P CMakeFiles/java_apidocs.dir/cmake_clean.cmake
.PHONY : developer/java_api/CMakeFiles/java_apidocs.dir/clean

developer/java_api/CMakeFiles/java_apidocs.dir/depend:
	cd /home/runner/work/tomcat/tomcat/docs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/runner/work/tomcat/tomcat/docs /home/runner/work/tomcat/tomcat/docs/developer/java_api /home/runner/work/tomcat/tomcat/docs/build /home/runner/work/tomcat/tomcat/docs/build/developer/java_api /home/runner/work/tomcat/tomcat/docs/build/developer/java_api/CMakeFiles/java_apidocs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : developer/java_api/CMakeFiles/java_apidocs.dir/depend

