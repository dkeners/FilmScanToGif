file(GLOB_RECURSE ASSETS LIST_DIRECTORIES false RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.xml" "*.png")

if(APPLE)
foreach(ASSET ${ASSETS})
    set_source_files_properties(${ASSET}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources/settings"
    )   
endforeach(ASSET ${ASSETS})
# set_source_files_properties(${ASSETS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/settings")
install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .)
else()
    foreach(ASSET ${ASSETS})
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}" "${CMAKE_CURRENT_BINARY_DIR}/Resources/settings/${ASSET}" COPYONLY)
    endforeach()
    # add_custom_target(
    #     copy_assets
    #     COMMAND ${CMAKE_COMMAND} 
    #     -D MY_PATH=${CMAKE_BINARY_DIR} 
    #     -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
    # )
    message(STATUS "Copying assets to ${CMAKE_BINARY_DIR}")
    # add_dependencies(main copy_assets)
endif()