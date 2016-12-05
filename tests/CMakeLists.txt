find_package(Qt4 COMPONENTS QtCore REQUIRED)

include(${QT_USE_FILE})

set(qfingerprint_test_SRCS
    main.cpp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(qfingerprint_test ${qfingerprint_test_SRCS})
set_target_properties(qfingerprint_test PROPERTIES AUTOMOC true)
target_link_libraries(qfingerprint_test ${QT_LIBRARIES})
target_link_libraries(qfingerprint_test qfingerprint)
target_link_libraries(qfingerprint_test ${CMAKE_SOURCE_DIR}/libfprint.a)
