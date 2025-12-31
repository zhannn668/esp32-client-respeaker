# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# Export compilation database for IDE IntelliSense (generates build/compile_commands.json)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# (Not part of the boilerplate)
# This example uses an extra component for common functions such as Wi-Fi and Ethernet connection.
# set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)

# Prefer a project-local ESP-ADF (kept under components/third_party/esp-adf so ESP-IDF
# doesn't treat the ESP-ADF root as a component). If present, always use it to ensure
# local modifications are actually compiled (even if an external ADF_PATH is set).
set(_PROJECT_ADF_PATH "${CMAKE_CURRENT_LIST_DIR}/components/third_party/esp-adf")
if(EXISTS "${_PROJECT_ADF_PATH}/CMakeLists.txt")
	set(ENV{ADF_PATH} "${_PROJECT_ADF_PATH}")
elseif(NOT "$ENV{ADF_PATH}" STREQUAL "")
	message(STATUS "Project-local ESP-ADF not found; using external ADF_PATH=$ENV{ADF_PATH}")
else()
	message(FATAL_ERROR "ESP-ADF not found. Expected ${_PROJECT_ADF_PATH} or set ADF_PATH env var.")
endif()

include($ENV{ADF_PATH}/CMakeLists.txt)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(videodoorbell_esp32)
