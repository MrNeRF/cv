# set search paths for GLEW Headers
set( _GLEW_HEADER_SEARCH_DIRS
        "/usr/include/"
        "/usr/local/include/"
        "${CMAKE_SOURCE_DIR}/includes/")
# set search paths for GLEW shared headers
set( _GLEW_LIB_SEARCH_DIRS
        "/usr/"
        "/usr/lib/"
        "/usr/lib/x86_64-linux-gnu/"
        "/usr/local/lib")

# Search for the library
FIND_LIBRARY(GLEW_LIBRARY NAMES glew GLEW
        PATHS ${_GLEW_LIB_SEARCH_DIRS})
# Search for the header
FIND_PATH(GLEW_INCLUDE_DIR "GL/glew.h"
        PATHS ${_GLEW_HEADER_SEARCH_DIRS} )
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG
        GLEW_LIBRARY GLEW_INCLUDE_DIR)
