PROJECT( testprocess ) # the name of your project

cmake_minimum_required(VERSION 3.16)


find_package(Qt6 COMPONENTS Core Gui Widgets Test REQUIRED)


SET(testprocess_SRCS
  ../../stegotable.cpp
  ../../ycbcr.cpp
  ../../groupedimage.cpp
  testprocess.cpp
)

SET(testprocess_MOC_HDRS
  testprocess.h
)

INCLUDE_DIRECTORIES(testprocess ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(testprocess ${sef_SOURCE_DIR} )


ADD_EXECUTABLE( testprocess ${testprocess_SRCS} ${testprocess_MOC_HDRS} )

target_link_libraries(testprocess sef Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test)


add_test(NAME testprocess COMMAND testprocess)
set_tests_properties(testprocess PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
