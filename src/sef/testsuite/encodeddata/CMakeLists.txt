PROJECT( testencodeddata ) # the name of your project

cmake_minimum_required(VERSION 3.16)


find_package(Qt6 COMPONENTS Core Gui Widgets Test REQUIRED)


SET(testencodeddata_SRCS
  testencodeddata.cpp
)

SET(testencodeddata_MOC_HDRS
  testencodeddata.h
)

INCLUDE_DIRECTORIES( testencodeddata ${CMAKE_BINARY_DIR} )
include_directories( testencodeddata 	${sef_SOURCE_DIR} )


ADD_EXECUTABLE( testencodeddata ${testencodeddata_SRCS} ${testencodeddata_MOC_HDRS} )

target_link_libraries(testencodeddata sef Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test)


add_test(NAME testencodeddata COMMAND testencodeddata)
set_tests_properties(testencodeddata PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
