add_executable(sga-example main.cpp)

target_link_libraries(sga-example sga)

add_custom_target(pack
  COMMAND sga-packer Resources assets.sga secret_key
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "packs resource folder to assets.sga"
)

add_dependencies(sga-example pack)