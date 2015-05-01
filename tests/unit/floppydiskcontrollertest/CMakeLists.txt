### unit test ###

set(floppydiskcontrollertest_SRCS
    floppydiskcontrollertest.cpp
)

add_executable(floppydiskcontrollertest ${floppydiskcontrollertest_SRCS})
target_link_libraries(floppydiskcontrollertest libfdc765-qt)
target_include_directories(floppydiskcontrollertest PRIVATE ${CMAKE_SOURCE_DIR}/src)
qt5_use_modules(floppydiskcontrollertest Test)

add_test(NAME floppydiskcontrollertest COMMAND floppydiskcontrollertest)
