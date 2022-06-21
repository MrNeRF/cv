set(glfw3_HEADER_SEARCH_DIRS
        "/usr/include/"
        "/usr/local/include/"
        "${CMAKE_SOURCE_DIR}/includes/")

set(glfw3_LIB_SEARCH_DIRS
        "/usr/"
        "/usr/lib/"
        "/usr/lib/x86_64-linux-gnu/"
        "/usr/local/lib")

# Search for the library
FIND_LIBRARY(GLFW3_LIBRARY NAMES glfw glfw3
        PATHS ${glfw3_LIB_SEARCH_DIRS})
# Search for the header
FIND_PATH(GLFW3_INCLUDE_DIR "GLFW/glfw3.h"
        PATHS ${glfw3_HEADER_SEARCH_DIRS})
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG
        GLFW3_LIBRARY GLFW3_INCLUDE_DIR)