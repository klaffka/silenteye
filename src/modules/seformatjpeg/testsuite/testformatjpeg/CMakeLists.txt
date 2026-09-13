PROJECT( testformatjpeg ) # the name of your project

cmake_minimum_required(VERSION 3.16)


find_package(Qt6 COMPONENTS Core Gui Widgets Test REQUIRED)


SET(testformatjpeg_SRCS
  ../../imagejpeg.cpp
  ../../ycbcr.cpp
  ../../groupedimage.cpp
  ../../stegotable.cpp
  testformatjpeg.cpp
)

SET(testformatjpeg_MOC_HDRS
  testformatjpeg.h
)

INCLUDE_DIRECTORIES(testformatjpeg ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(testformatjpeg ${sef_SOURCE_DIR} )



ADD_EXECUTABLE( testformatjpeg ${testformatjpeg_SRCS} ${testformatjpeg_MOC_HDRS} )

target_link_libraries(testformatjpeg sef Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test)


add_test(NAME testformatjpeg COMMAND testformatjpeg)
set_tests_properties(testformatjpeg PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
