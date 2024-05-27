add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -D MY_PATH=${CMAKE_BINARY_DIR} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
message(STATUS "Copying assets to ${CMAKE_BINARY_DIR}")
add_dependencies(main copy_assets)