cmake_minimum_required(VERSION 3.0.0)


add_compile_definitions(AES256)

project(sga-packer)

add_executable(${PROJECT_NAME} src/main.cpp src/packer.cpp)
target_link_libraries(${PROJECT_NAME} sga)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

add_custom_command(
    OUTPUT
        ${CMAKE_BINARY_DIR}/assets.sga
    COMMAND
        ${CMAKE_BINARY_DIR}/sga-packer
    DEPENDS
        sga-packer
    VERBATIM
)
