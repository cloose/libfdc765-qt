cmake_minimum_required(VERSION 2.8.12)

project(libfdc765-qt)

### Setup Qt moc tool ###

# tell CMake to run moc when necessary
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

### CTest ###

enable_testing()

### Dependencies ###

find_package(Qt5Core REQUIRED)

### Subdirs ###

add_subdirectory(src)
add_subdirectory(tests)
