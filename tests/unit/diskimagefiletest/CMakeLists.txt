### unit test ###

set(diskimagefiletest_SRCS
    diskimagefiletest.cpp
)

add_executable(diskimagefiletest ${diskimagefiletest_SRCS})
target_link_libraries(diskimagefiletest libfdc765-qt)
target_include_directories(diskimagefiletest PRIVATE ${CMAKE_SOURCE_DIR}/src)
qt5_use_modules(diskimagefiletest Test)

add_test(NAME diskimagefiletest COMMAND diskimagefiletest)
