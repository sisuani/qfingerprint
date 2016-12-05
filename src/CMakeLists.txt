find_package(Qt4 COMPONENTS QtCore REQUIRED)

include(${QT_USE_FILE})

set(qfingerprint_SRCS
    qfingerprint.cpp
)

include_directories(
    .
    ${CMAKE_CURRENT_BINARY_DIR})
add_library(qfingerprint STATIC ${qfingerprint_SRCS})
set_target_properties(qfingerprint PROPERTIES AUTOMOC true)
target_link_libraries(qfingerprint glib-2.0)
target_link_libraries(qfingerprint usb-1.0)
target_link_libraries(qfingerprint nss3)
target_link_libraries(qfingerprint pthread)
target_link_libraries(qfingerprint ${CMAKE_SOURCE_DIR}/libfprint.a)


#target_link_libraries(qfingerprint dl)
#target_link_libraries(qfingerprint gdk_pixbuf-2.0)
#target_link_libraries(qfingerprint gobject-2.0)
