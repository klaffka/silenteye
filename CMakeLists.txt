IF(WIN32)
  PROJECT(SilentEye)
ELSE(WIN32)
  PROJECT(silenteye)
ENDIF(WIN32)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

IF($ENV{ENABLE_STATIC})
  MESSAGE("** Build in static mode : enable static plugins")
  OPTION(STATIC_BUILD "enable static build support" ON)
  SET(STATIC_LIBRARY_FLAGS TRUE)
ELSE($ENV{ENABLE_STATIC})
  MESSAGE("** Build in shared mode")
ENDIF($ENV{ENABLE_STATIC})

# BIN PATH
IF(NOT "$ENV{BINPATH_VALUE}" STREQUAL "")
  SET(BINPATH_VALUE $ENV{BINPATH_VALUE})
  MESSAGE("** Use custom bin path: ${BINPATH_VALUE}")
  SET(INSTALL_BINPATH ${BINPATH_VALUE}) # default install
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(INSTALL_BINPATH "/usr/bin") # default install dir
    MESSAGE("** Use default bin path: ${INSTALL_BINPATH}")
    SET(BINPATH_VALUE ${INSTALL_BINPATH})
  ELSE(UNIX AND NOT APPLE)
    SET(INSTALL_BINPATH ".")
  ENDIF(UNIX AND NOT APPLE)
ENDIF(NOT "$ENV{BINPATH_VALUE}" STREQUAL "")
# CONF PATH
IF($ENV{CONFPATH_USERHOME})
  ADD_DEFINITIONS(-DCONFPATH_USERHOME)
  MESSAGE("** Use user's home as configuration path for application: $HOME/.silenteye/silenteye.conf")
  IF("$ENV{CONFPATH_VALUE}" STREQUAL "")
    SET(INSTALL_CONFPATH "/etc")
    MESSAGE("** Use default configuration path installation: ${INSTALL_CONFPATH}/silenteye.conf")
  ELSE("$ENV{CONFPATH_VALUE}" STREQUAL "")
    SET(INSTALL_CONFPATH $ENV{CONFPATH_VALUE}) # install dir
    MESSAGE("** Use custom configuration path installation: ${INSTALL_CONFPATH}/silenteye.conf")
  ENDIF("$ENV{CONFPATH_VALUE}" STREQUAL "")
ELSEIF(NOT "$ENV{CONFPATH_VALUE}" STREQUAL "")
  SET(CONFPATH_VALUE $ENV{CONFPATH_VALUE})
  MESSAGE("** Use custom configuration path: ${CONFPATH_VALUE}/silenteye.conf")
  SET(INSTALL_CONFPATH ${CONFPATH_VALUE}) # install dir
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(INSTALL_CONFPATH "/etc") # default install dir
  ELSE(UNIX AND NOT APPLE)
    SET(INSTALL_CONFPATH ".") # default install dir
  ENDIF(UNIX AND NOT APPLE)
  SET(CONFPATH_VALUE ${INSTALL_CONFPATH})
  MESSAGE("** Use default configuration path: ${INSTALL_CONFPATH}/silenteye.conf")
ENDIF($ENV{CONFPATH_USERHOME})
# LIB PATH
IF(NOT "$ENV{LIBPATH_VALUE}" STREQUAL "")
  SET(LIBPATH_VALUE $ENV{LIBPATH_VALUE})
  MESSAGE("** Use custom lib path: ${LIBPATH_VALUE}")
  SET(INSTALL_LIBPATH $ENV{LIBPATH_VALUE}) # install dir
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(INSTALL_LIBPATH "/usr/lib/silenteye") # default install dir
    MESSAGE("** Use default lib path: ${INSTALL_LIBPATH}/modules/*.so")
    SET(LIBPATH_VALUE ${INSTALL_LIBPATH})
  ELSE(UNIX AND NOT APPLE)
    SET(INSTALL_LIBPATH ".")
  ENDIF(UNIX AND NOT APPLE)
ENDIF(NOT "$ENV{LIBPATH_VALUE}" STREQUAL "")
# COMMON PATH
IF(NOT "$ENV{CMNPATH_VALUE}" STREQUAL "")
  SET(CMNPATH_VALUE $ENV{CMNPATH_VALUE})
  MESSAGE("** Use custom common path: ${CMNPATH_VALUE}/version.xml")
  SET(INSTALL_CMNPATH ${CMNPATH_VALUE}) # default install
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(INSTALL_CMNPATH "/usr/share/silenteye") # default install dir
    MESSAGE("** Use default common path: ${INSTALL_CMNPATH}/version.xml")
    SET(CMNPATH_VALUE ${INSTALL_CMNPATH})
  ELSE(UNIX AND NOT APPLE)
    SET(INSTALL_CMNPATH ".")
  ENDIF(UNIX AND NOT APPLE)
ENDIF(NOT "$ENV{CMNPATH_VALUE}" STREQUAL "")
# LOG PATH
IF($ENV{LOGPATH_USERHOME})
  ADD_DEFINITIONS(-DLOGPATH_USERHOME)
  MESSAGE("** Use user's home as log path: $HOME/.silenteye/silenteye.log")
ELSEIF(NOT "$ENV{LOGPATH_VALUE}" STREQUAL "")
  SET(LOGPATH_VALUE $ENV{LOGPATH_VALUE})
  MESSAGE("** Use custom log path: ${LOGPATH_VALUE}/silenteye.log")
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(LOGPATH_VALUE "/var/log")
    MESSAGE("** Use default log path: ${LOGPATH_VALUE}/silenteye.log")
  ENDIF(UNIX AND NOT APPLE)
ENDIF($ENV{LOGPATH_USERHOME})
# DOC PATH
IF($ENV{ENABLE_PACK})
  IF(NOT "$ENV{DOCPATH_VALUE}" STREQUAL "")
    SET(INSTALL_DOCPATH "$ENV{DOCPATH_VALUE}")
    MESSAGE("** Use custom doc path: ${INSTALL_DOCPATH}")
  ELSE(NOT "$ENV{DOCPATH_VALUE}" STREQUAL "")
    SET(INSTALL_DOCPATH "/usr/share/doc/silenteye")
    MESSAGE("** Use default doc path: ${INSTALL_DOCPATH}")
  ENDIF(NOT "$ENV{DOCPATH_VALUE}" STREQUAL "")
ENDIF($ENV{ENABLE_PACK})
# PIX PATH
IF($ENV{ENABLE_PACK})
  IF(NOT "$ENV{PIXPATH_VALUE}" STREQUAL "")
    SET(INSTALL_PIXPATH "$ENV{PIXPATH_VALUE}")
    MESSAGE("** Use custom pixmap path: ${INSTALL_PIXPATH}")
  ELSE(NOT "$ENV{PIXPATH_VALUE}" STREQUAL "")
    SET(INSTALL_PIXPATH "/usr/share/pixmaps")
    MESSAGE("** Use default pixmap path: ${INSTALL_PIXPATH}")
  ENDIF(NOT "$ENV{PIXPATH_VALUE}" STREQUAL "")
  IF($ENV{ENABLE_GNOMEMENU})
    MESSAGE("** Use configuration file install/linux/silenteye.desktop.cmake")
    CONFIGURE_FILE(${CMAKE_BINARY_DIR}/install/linux/silenteye.desktop.cmake ${CMAKE_BINARY_DIR}/silenteye.desktop)
  ENDIF($ENV{ENABLE_GNOMEMENU})
ENDIF($ENV{ENABLE_PACK})

#######################################################
# MANAGE PROJECT COMPILATION
#######################################################

SET(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/src/cmake)

IF(APPLE)
  SET(MACOSX_BUNDLE_INFO_STRING "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
  SET(MACOSX_BUNDLE_ICON_FILE "silenteye.icns")
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "SilentEye")
  SET(MACOSX_BUNDLE_LONG_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "SilentEye")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
  SET(MACOSX_BUNDLE_VERSION "${CPACK_PACKAGE_VERSION}")
  SET(MACOSX_BUNDLE_COPYRIGHT "GNU GPLv3")
ENDIF(APPLE)

# Qt6 framework
find_package(Qt6 COMPONENTS Core Gui Widgets Network Concurrent Multimedia Core5Compat REQUIRED)
find_package(Qca-qt6 2.3 REQUIRED)

# Set the compilation parameters
IF(APPLE AND EXISTS "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/type_traits")
  # Workaround for incomplete CommandLineTools libc++ headers in /usr/include/c++/v1
  SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall -isystem /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1")
ELSE()
  SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall")
ENDIF()

IF($ENV{ENABLE_DEBUG})
  SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall -g -ggdb")
  MESSAGE("** Enabled DebugMode")
ENDIF($ENV{ENABLE_DEBUG})

add_subdirectory(./src/sef) #compile the silent eye framework

# build modules (plugins)
IF($ENV{ENABLE_MODULE})
  MESSAGE("** Enabled modules compilation ")
  add_subdirectory(./src/modules)
ENDIF($ENV{ENABLE_MODULE})

add_subdirectory(./src)

#######################################################
# MANAGE PROJECT PACKAGING
#######################################################

IF($ENV{ENABLE_PACK})
  SET(CPACK_PACKAGE_NAME "silenteye")
  SET(CPACK_PACKAGE_VENDOR "SilentEye")
  SET(CPACK_PACKAGE_CONTACT "Anselme CHOREIN <anselme@chorein.fr>")
  SET(CPACK_PACKAGE_DESCRIPTION "SilentEye: Steganography is yours!")
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SilentEye is a cross-plateform application design for an easy use of steganography, in this case hidding messages into pictures and sounds. It provides a pretty nice interface and an easy integration of new steganography algorithm and cryptography process by using a plug-ins system.")
  SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
  SET(CPACK_PACKAGE_VERSION_MAJOR "0")
  SET(CPACK_PACKAGE_VERSION_MINOR "4")
  SET(CPACK_PACKAGE_VERSION_PATCH "3")
  SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

  IF(APPLE)
    SET(CPACK_GENERATOR "DRAGNDROP")
  ENDIF(APPLE)

  INCLUDE(CPack)
ENDIF($ENV{ENABLE_PACK})