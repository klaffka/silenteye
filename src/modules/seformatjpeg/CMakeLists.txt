PROJECT( seformatjpeg )
cmake_minimum_required(VERSION 3.16)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

IF(NOT STATIC_LIBRARY_FLAGS)
  SET( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/modules )
ENDIF()

INCLUDE_DIRECTORIES(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${sef_SOURCE_DIR}
  )

SET(seformatjpeg_SRCS
  seformatjpeg.cpp
  imagejpeg.cpp
  optionwidget.cpp
  groupedimage.cpp
  ycbcr.cpp
  stegotable.cpp
  )

SET(seformatjpeg_UIS
  ui/optionwidget.ui
  )

ADD_DEFINITIONS(-DQT_NO_DEBUG)
IF(STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_STATICPLUGIN)
ELSE()
  ADD_DEFINITIONS(-DQT_PLUGIN)
  ADD_DEFINITIONS(-DQT_SHARED)
ENDIF()
ADD_DEFINITIONS(-DQT_DLL)

IF(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY(seformatjpeg STATIC ${seformatjpeg_SRCS} ${seformatjpeg_UIS})
ELSE()
  ADD_LIBRARY(seformatjpeg SHARED ${seformatjpeg_SRCS} ${seformatjpeg_UIS})
ENDIF()

TARGET_LINK_LIBRARIES(seformatjpeg
  sef
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)

IF(APPLE AND NOT STATIC_LIBRARY_FLAGS)
  add_custom_command(TARGET seformatjpeg POST_BUILD
    COMMAND mkdir ARGS -pv ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules
    COMMAND cp ARGS $<TARGET_FILE:seformatjpeg> ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules/
    COMMENT "copy seformatjpeg plug-in into silenteye.app"
  )
ENDIF()

if($ENV{ENABLE_TESTSUITE})
  message("** Enabled seformatjpeg testsuite ")
  add_subdirectory( ./testsuite )
endif()