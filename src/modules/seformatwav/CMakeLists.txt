PROJECT( seformatwav )
cmake_minimum_required(VERSION 3.16)

find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia REQUIRED)

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

SET(seformatwav_SRCS
  seformatwav.cpp
  audiowav.cpp
  optionwidget.cpp
  )

SET(seformatwav_UIS
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
  ADD_LIBRARY(seformatwav STATIC ${seformatwav_SRCS} ${seformatwav_UIS})
ELSE()
  ADD_LIBRARY(seformatwav SHARED ${seformatwav_SRCS} ${seformatwav_UIS})
ENDIF()

TARGET_LINK_LIBRARIES(seformatwav
  sef
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::Multimedia
)

IF(APPLE AND NOT STATIC_LIBRARY_FLAGS)
  add_custom_command(TARGET seformatwav POST_BUILD
    COMMAND mkdir ARGS -pv ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules
    COMMAND cp ARGS $<TARGET_FILE:seformatwav> ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules/
    COMMENT "copy seformatwav plug-in into silenteye.app"
  )
ENDIF()

if($ENV{ENABLE_TESTSUITE})
  message("** Enabled seformatwav testsuite ")
  add_subdirectory( ./testsuite )
endif()