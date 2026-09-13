PROJECT( testconfig ) # the name of your project

cmake_minimum_required(VERSION 3.16)


find_package(Qt6 COMPONENTS Core Gui Widgets Test REQUIRED)


SET(testconfig_SRCS
  testconfig.cpp
)

SET(testconfig_MOC_HDRS
  testconfig.h
)

INCLUDE_DIRECTORIES( testconfig ${CMAKE_BINARY_DIR} )
include_directories( testconfig ${sef_SOURCE_DIR} )


ADD_EXECUTABLE( testconfig ${testconfig_SRCS} ${testconfig_MOC_HDRS} )

target_link_libraries(testconfig sef Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test)


add_test(NAME testconfig COMMAND testconfig)
set_tests_properties(testconfig PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
