PROJECT( sef )

cmake_minimum_required(VERSION 3.16)

find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia Concurrent Core5Compat REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(NOT STATIC_LIBRARY_FLAGS)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
endif()

SET(sef_SRCS
    config.cpp
    data.cpp
    encodeddata.cpp
    media.cpp
    image.cpp
    audio.cpp
    video.cpp
    silenteyeexception.cpp
    moduleexception.cpp
    logger.cpp
)

IF(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY( sef STATIC ${sef_SRCS})
ELSE()
  ADD_LIBRARY( sef SHARED ${sef_SRCS})
ENDIF()

TARGET_LINK_LIBRARIES(sef PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Concurrent
    Qt6::Multimedia
    Qt6::Core5Compat
)

if($ENV{ENABLE_TESTSUITE})
  message("** Enabled sef testsuite ")
  add_subdirectory( ./testsuite )
endif()

##############################
# PACKAGING
##############################
IF($ENV{ENABLE_PACK})
  IF(WIN32)
    install(TARGETS sef RUNTIME DESTINATION . COMPONENT Application)
  ELSE()
    install(TARGETS sef DESTINATION ${INSTALL_LIBPATH} COMPONENT Application)
  ENDIF()
ENDIF()