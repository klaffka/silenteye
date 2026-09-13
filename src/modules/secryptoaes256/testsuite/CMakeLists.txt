PROJECT( testcryptoaes256 ) # the name of your project

cmake_minimum_required(VERSION 3.16)


find_package(Qt6 COMPONENTS Core Gui Widgets Test REQUIRED)


SET(testcryptoaes256_SRCS
  testcryptoaes256.cpp
)

SET(testcryptoaes256_MOC_HDRS
  testcryptoaes256.h
)

INCLUDE_DIRECTORIES( testcryptoaes256 ${CMAKE_BINARY_DIR} )
include_directories( testcryptoaes256 	${sef_SOURCE_DIR} )


ADD_EXECUTABLE( testcryptoaes256 ${testcryptoaes256_SRCS} ${testcryptoaes256_MOC_HDRS} )

target_link_libraries(testcryptoaes256 sef Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test)


add_test(NAME testcryptoaes256 COMMAND testcryptoaes256)
set_tests_properties(testcryptoaes256 PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
