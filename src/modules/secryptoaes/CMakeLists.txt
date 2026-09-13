PROJECT( secryptoaes )
cmake_minimum_required(VERSION 3.16)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qca-qt6 2.3 REQUIRED)

# QCA's CMake package exposes either a plain "qca-qt6" target (Homebrew/MSYS2)
# or a namespaced target depending on the build; resolve it defensively.
IF(TARGET qca-qt6)
  SET(SILENTEYE_QCA_TARGET qca-qt6)
ELSEIF(TARGET Qca-qt6::qca-qt6)
  SET(SILENTEYE_QCA_TARGET Qca-qt6::qca-qt6)
ELSE()
  MESSAGE(FATAL_ERROR "No usable QCA (Qt6) CMake target found")
ENDIF()

set(CMAKE_AUTOMOC ON)

IF(NOT STATIC_LIBRARY_FLAGS)
  SET( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/modules )
ENDIF()

INCLUDE_DIRECTORIES(
  ${sef_SOURCE_DIR}
)

SET(secryptoaes_SRCS
  secryptoaes.cpp
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
  ADD_LIBRARY(secryptoaes STATIC ${secryptoaes_SRCS})
ELSE()
  ADD_LIBRARY(secryptoaes SHARED ${secryptoaes_SRCS})
ENDIF()

TARGET_LINK_LIBRARIES(secryptoaes
  sef
  ${SILENTEYE_QCA_TARGET}
  Qt6::Core
)

IF(APPLE AND NOT STATIC_LIBRARY_FLAGS)
  add_custom_command(TARGET secryptoaes POST_BUILD
    COMMAND mkdir ARGS -pv ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules
    COMMAND cp ARGS $<TARGET_FILE:secryptoaes> ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules/
    COMMENT "copy secryptoaes plug-in into silenteye.app"
  )
ENDIF()

if($ENV{ENABLE_TESTSUITE})
  message("** Enabled secryptoaes testsuite ")
  add_subdirectory( ./testsuite )
endif()