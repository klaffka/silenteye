PROJECT( testcryptoaes ) # the name of your project

cmake_minimum_required(VERSION 3.16)


find_package(Qt6 COMPONENTS Core Gui Widgets Test REQUIRED)


SET(testcryptoaes_SRCS
  testcryptoaes.cpp
)

SET(testcryptoaes_MOC_HDRS
  testcryptoaes.h
)

INCLUDE_DIRECTORIES( testcryptoaes ${CMAKE_BINARY_DIR} )
include_directories( testcryptoaes 	${sef_SOURCE_DIR} )


ADD_EXECUTABLE( testcryptoaes ${testcryptoaes_SRCS} ${testcryptoaes_MOC_HDRS} )

target_link_libraries(testcryptoaes sef Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test)


add_test(NAME testcryptoaes COMMAND testcryptoaes)
set_tests_properties(testcryptoaes PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
