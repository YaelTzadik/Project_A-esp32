# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /cygdrive/c/Users/ASUS/AppData/Local/JetBrains/CLion2020.2/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/ASUS/AppData/Local/JetBrains/CLion2020.2/cygwin_cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/FaceRecognizer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FaceRecognizer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FaceRecognizer.dir/flags.make

CMakeFiles/FaceRecognizer.dir/main.cpp.o: CMakeFiles/FaceRecognizer.dir/flags.make
CMakeFiles/FaceRecognizer.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FaceRecognizer.dir/main.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FaceRecognizer.dir/main.cpp.o -c /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/main.cpp

CMakeFiles/FaceRecognizer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FaceRecognizer.dir/main.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/main.cpp > CMakeFiles/FaceRecognizer.dir/main.cpp.i

CMakeFiles/FaceRecognizer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FaceRecognizer.dir/main.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/main.cpp -o CMakeFiles/FaceRecognizer.dir/main.cpp.s

CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.o: CMakeFiles/FaceRecognizer.dir/flags.make
CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.o: ../FaceRecognizer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.o -c /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/FaceRecognizer.cpp

CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/FaceRecognizer.cpp > CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.i

CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/FaceRecognizer.cpp -o CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.s

CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.o: CMakeFiles/FaceRecognizer.dir/flags.make
CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.o: ../SmileDetector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.o -c /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/SmileDetector.cpp

CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/SmileDetector.cpp > CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.i

CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/SmileDetector.cpp -o CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.s

# Object files for target FaceRecognizer
FaceRecognizer_OBJECTS = \
"CMakeFiles/FaceRecognizer.dir/main.cpp.o" \
"CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.o" \
"CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.o"

# External object files for target FaceRecognizer
FaceRecognizer_EXTERNAL_OBJECTS =

FaceRecognizer.exe: CMakeFiles/FaceRecognizer.dir/main.cpp.o
FaceRecognizer.exe: CMakeFiles/FaceRecognizer.dir/FaceRecognizer.cpp.o
FaceRecognizer.exe: CMakeFiles/FaceRecognizer.dir/SmileDetector.cpp.o
FaceRecognizer.exe: CMakeFiles/FaceRecognizer.dir/build.make
FaceRecognizer.exe: CMakeFiles/FaceRecognizer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable FaceRecognizer.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FaceRecognizer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FaceRecognizer.dir/build: FaceRecognizer.exe

.PHONY : CMakeFiles/FaceRecognizer.dir/build

CMakeFiles/FaceRecognizer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FaceRecognizer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FaceRecognizer.dir/clean

CMakeFiles/FaceRecognizer.dir/depend:
	cd /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/cmake-build-debug /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/cmake-build-debug /cygdrive/c/Users/ASUS/CLionProjects/Semester8Summer/FaceRecognizer/cmake-build-debug/CMakeFiles/FaceRecognizer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FaceRecognizer.dir/depend
