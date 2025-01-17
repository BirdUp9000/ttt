# CMakeLists.txt in lib/BencodeParser

# Create a static library from the source files
add_library(BencodeParser STATIC
  types/node.cpp
  types/dict.cpp
  types/int.cpp
  types/list.cpp
  types/str.cpp
  parser.cpp
  torrentFileInfo.cpp
)

# Specify include directories for the library
target_include_directories(BencodeParser PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/types
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add AddressSanitizer flags to the library target
target_compile_options(BencodeParser PRIVATE -fsanitize=address -Wall -Wextra -pedantic -g -O0)
target_link_libraries(BencodeParser PRIVATE -fsanitize=address)