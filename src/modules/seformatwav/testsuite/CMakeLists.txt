PROJECT( testformatwav ) # the name of your project

cmake_minimum_required(VERSION 3.16)


find_package(Qt6 COMPONENTS Core Gui Widgets Test REQUIRED)


SET(testformatwav_SRCS
  ../audiowav.cpp
  testformatwav.cpp
)

SET(testformatwav_MOC_HDRS
  testformatwav.h
)

INCLUDE_DIRECTORIES(testformatwav ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(testformatwav ${sef_SOURCE_DIR} )


ADD_EXECUTABLE( testformatwav ${testformatwav_SRCS} ${testformatwav_MOC_HDRS} )

target_link_libraries(testformatwav sef Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test)


add_test(NAME testformatwav COMMAND testformatwav)
set_tests_properties(testformatwav PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
