cmake_minimum_required(VERSION 3.0.0)

project(sga-packer)

add_executable(${PROJECT_NAME} src/main.cpp src/packer.cpp src/log.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)