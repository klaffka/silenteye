PROJECT( silenteye )

cmake_minimum_required(VERSION 3.16)

find_package(Qt6 COMPONENTS Core Gui Widgets Network Concurrent Multimedia Core5Compat REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
set(CMAKE_AUTORCC ON)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/)

SET(silenteye_SRCS
  main.cpp
  modulemanager.cpp
  modulecommander.cpp
  controller.cpp
  mainwindow.cpp
  aboutdialog.h
  optiondialog.cpp
  decodedialog.cpp
  encodedialog.cpp
  propertydialog.cpp
  preferencedialog.cpp
  updatedialog.cpp
  mediawidget.cpp
  )

SET(silenteye_UIS
  ui/mainwindow.ui
  ui/mediawidget.ui
  ui/optiondialog.ui
  ui/propertydialog.ui
  ui/preferencedialog.ui
  ui/aboutdialog.ui
  ui/updatedialog.ui
)

SET(silenteye_RCS
  silenteye.qrc
  )

# custom path configuration (conf.h.in)
MESSAGE("** Use configuration file conf.h.in")
CONFIGURE_FILE(${CMAKE_CURRENT_LIST_DIR}/conf.h.in ${CMAKE_CURRENT_BINARY_DIR}/conf.h)

IF(STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_STATICPLUGIN)
ENDIF(STATIC_LIBRARY_FLAGS)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${sef_SOURCE_DIR}
  )

IF(WIN32)
  ADD_EXECUTABLE(silenteye WIN32 ${silenteye_SRCS} ${silenteye_UIS} ${silenteye_RCS})
ENDIF(WIN32)
IF(APPLE)
  ADD_EXECUTABLE(silenteye MACOSX_BUNDLE ${silenteye_SRCS} ${silenteye_UIS} ${silenteye_RCS})
  add_custom_command(TARGET silenteye PRE_BUILD
       COMMAND mkdir ARGS -pv ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules
       COMMENT "Create ./silenteye.app/Contents/MacOS/modules directory")
ENDIF(APPLE)
IF(UNIX AND NOT APPLE)
  ADD_EXECUTABLE(silenteye ${silenteye_SRCS} ${silenteye_UIS} ${silenteye_RCS})
ENDIF(UNIX AND NOT APPLE)

TARGET_LINK_LIBRARIES(silenteye
    sef
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
    Qt6::Core5Compat
)

IF(STATIC_LIBRARY_FLAGS)
  TARGET_LINK_LIBRARIES(silenteye
    secryptoaes
    secryptoaes256
    seformatbmp
    seformatjpeg
    seformatwav
    )
ENDIF(STATIC_LIBRARY_FLAGS)

############################
# PACKAGING
############################
IF($ENV{ENABLE_PACK})
  IF(APPLE)
    install(TARGETS silenteye
      RUNTIME DESTINATION .
      BUNDLE DESTINATION .
      COMPONENT Application
    )
    file(COPY "img/silenteye.icns" DESTINATION ${CMAKE_BINARY_DIR}/silenteye.app/Contents/Resources/)
  ENDIF(APPLE)
ENDIF($ENV{ENABLE_PACK})