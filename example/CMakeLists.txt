add_executable(sga-example main.cpp)

target_link_libraries(sga-example sga)

add_custom_target(pack_menu_assets
  COMMAND sga-packer resources/menu_assets menu_assets.sga secret_key
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "packs game menu assets"
)

add_custom_target(pack_game_assets
  COMMAND sga-packer resources/game_assets game_assets.sga secret_key
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "packs game level assets"
)

add_dependencies(sga-example pack_menu_assets pack_game_assets)