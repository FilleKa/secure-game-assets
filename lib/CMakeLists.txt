
add_library(sga 
    src/header.cpp
    src/log.cpp
    src/reader.cpp
    src/sga_manager.cpp 
    src/writer.cpp
)

add_subdirectory(external/tiny-aes-c)

target_link_libraries(sga tiny-aes-C)
target_include_directories(sga PUBLIC include)
target_compile_features(sga PRIVATE cxx_std_17)