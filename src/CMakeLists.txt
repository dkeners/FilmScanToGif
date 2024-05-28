cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

target_sources(main PRIVATE
    # ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/croppedimgwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loadedimage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/subbmprect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/bitmaptransforms.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/layoutmanager.cpp
    )

add_subdirectory(settings)