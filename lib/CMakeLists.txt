
add_library(sga 
    src/file_base.cpp
    src/header.cpp
    src/log.cpp
    src/reader.cpp
    src/sga_file.cpp
    src/sga_manager.cpp 
    src/writer.cpp
    src/endianness.cpp
)

add_subdirectory(external/tiny-aes-c)

target_link_libraries(sga tiny-aes-C)
target_include_directories(sga PUBLIC include)
target_compile_features(sga PUBLIC cxx_std_17)

if(MSVC)
  target_compile_options(sga PRIVATE /W4 /WX)
else()
  target_compile_options(sga PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
