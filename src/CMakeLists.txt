cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/croppedimgwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exportdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/animationcontrols.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loadedimage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/animator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/bitmaptransforms.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/imagecombinator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/layoutmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/subbmprect.cpp
    )
