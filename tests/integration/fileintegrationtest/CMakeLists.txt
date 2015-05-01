### unit test ###

set(fileintegrationtest_SRCS
    fileintegrationtest.cpp
)

add_executable(fileintegrationtest ${fileintegrationtest_SRCS})
target_link_libraries(fileintegrationtest libfdc765-qt)
target_include_directories(fileintegrationtest PRIVATE ${CMAKE_SOURCE_DIR}/src)
qt5_use_modules(fileintegrationtest Test)

add_test(NAME fileintegrationtest COMMAND fileintegrationtest)
