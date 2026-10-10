# Run with WS_DL6LE_BUILD_DIR set to the configured CMake build directory.
if (NOT DEFINED ENV{WS_DL6LE_BUILD_DIR})
  message (FATAL_ERROR "Set WS_DL6LE_BUILD_DIR to the configured build directory")
endif ()
include ("$ENV{WS_DL6LE_BUILD_DIR}/CPackConfig.cmake")
set (DL6LE_UPSTREAM_CPACK_OPTIONS "${CPACK_PROJECT_CONFIG_FILE}")
set (CPACK_PROJECT_CONFIG_FILE "${CMAKE_CURRENT_LIST_DIR}/DL6LECPackOptions.cmake")
