PROJECT( testformatbmp ) # the name of your project

cmake_minimum_required(VERSION 3.16)


find_package(Qt6 COMPONENTS Core Gui Widgets Test REQUIRED)


SET(testformatbmp_SRCS
  ../imagebmp.cpp
  testformatbmp.cpp
)

SET(testformatbmp_MOC_HDRS
  testformatbmp.h
)

INCLUDE_DIRECTORIES(testformatbmp ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(testformatbmp ${sef_SOURCE_DIR} )


ADD_EXECUTABLE( testformatbmp ${testformatbmp_SRCS} ${testformatbmp_MOC_HDRS} )

target_link_libraries(testformatbmp sef Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test)


add_test(NAME testformatbmp COMMAND testformatbmp)
set_tests_properties(testformatbmp PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
