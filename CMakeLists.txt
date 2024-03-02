cmake_minimum_required (VERSION 3.27)

project (badMemAlg)

file (MAKE_DIRECTORY build/results)

aux_source_directory (src SRCS)

add_executable (${PROJECT_NAME} ${SRCS})

set_property (TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

find_package (json-c CONFIG REQUIRED)
target_link_libraries (${PROJECT_NAME} PRIVATE json-c::json-c)

target_compile_options (${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-format-truncation -g)