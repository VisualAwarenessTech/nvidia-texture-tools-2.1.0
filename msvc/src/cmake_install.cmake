# Install script for directory: K:/Development/op3d_active/nvidia-texture-tools-2.1.0/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/NV")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("K:/Development/op3d_active/nvidia-texture-tools-2.1.0/msvc/src/nvcore/cmake_install.cmake")
  INCLUDE("K:/Development/op3d_active/nvidia-texture-tools-2.1.0/msvc/src/nvmath/cmake_install.cmake")
  INCLUDE("K:/Development/op3d_active/nvidia-texture-tools-2.1.0/msvc/src/nvimage/cmake_install.cmake")
  INCLUDE("K:/Development/op3d_active/nvidia-texture-tools-2.1.0/msvc/src/nvthread/cmake_install.cmake")
  INCLUDE("K:/Development/op3d_active/nvidia-texture-tools-2.1.0/msvc/src/nvtt/cmake_install.cmake")
  INCLUDE("K:/Development/op3d_active/nvidia-texture-tools-2.1.0/msvc/src/bc6h/cmake_install.cmake")
  INCLUDE("K:/Development/op3d_active/nvidia-texture-tools-2.1.0/msvc/src/bc7/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

