cmake_minimum_required(VERSION 3.11-3.18)

project ( WinApp )

# Include files
include_directories( ./include ./src )

# Target 
add_executable( winapp ./src/main.cpp )

# External libraries

# Flags
target_compile_features(winapp PUBLIC cxx_std_20)
