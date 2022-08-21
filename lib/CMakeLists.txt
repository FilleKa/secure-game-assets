
add_library(sga 
    src/sga_manager.cpp 
    src/writer.cpp
    src/reader.cpp
    src/header.cpp
)

target_include_directories(sga PUBLIC include)