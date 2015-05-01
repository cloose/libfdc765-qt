### libfdc765-qt library ###

set(libfdc765-qt_SRCS
    diskimagefile.cpp
    floppydiskcontroller.cpp
)

add_library(libfdc765-qt SHARED ${libfdc765-qt_SRCS})
qt5_use_modules(libfdc765-qt Core)
