add_library(spdlog INTERFACE IMPORTED GLOBAL)
add_library(eigen INTERFACE IMPORTED GLOBAL)
add_library(fmt INTERFACE IMPORTED GLOBAL)

target_include_directories(spdlog INTERFACE spdlog/include)
target_include_directories(eigen INTERFACE eigen)
target_include_directories(fmt INTERFACE fmt/include)