# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.25.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.25.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock

# Utility rule file for PicoLed_WS2812B_pio_h.

# Include any custom commands dependencies for this target.
include CMakeFiles/PicoLed_WS2812B_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/PicoLed_WS2812B_pio_h.dir/progress.make

CMakeFiles/PicoLed_WS2812B_pio_h: build/WS2812B.pio.h

build/WS2812B.pio.h: PicoLed/WS2812B.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating build/WS2812B.pio.h"
	pioasm/pioasm -o c-sdk /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/PicoLed/WS2812B.pio /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/build/WS2812B.pio.h

PicoLed_WS2812B_pio_h: CMakeFiles/PicoLed_WS2812B_pio_h
PicoLed_WS2812B_pio_h: build/WS2812B.pio.h
PicoLed_WS2812B_pio_h: CMakeFiles/PicoLed_WS2812B_pio_h.dir/build.make
.PHONY : PicoLed_WS2812B_pio_h

# Rule to build all files generated by this target.
CMakeFiles/PicoLed_WS2812B_pio_h.dir/build: PicoLed_WS2812B_pio_h
.PHONY : CMakeFiles/PicoLed_WS2812B_pio_h.dir/build

CMakeFiles/PicoLed_WS2812B_pio_h.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PicoLed_WS2812B_pio_h.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PicoLed_WS2812B_pio_h.dir/clean

CMakeFiles/PicoLed_WS2812B_pio_h.dir/depend:
	cd /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock /Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/CMakeFiles/PicoLed_WS2812B_pio_h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PicoLed_WS2812B_pio_h.dir/depend

