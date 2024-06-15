cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(FilmScanToGif LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxBUILD_SHARED OFF)

message(STATUS "Fetching wxWidgets v3.2.5...")

FetchContent_Declare(
   wxWidgets
   GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
   GIT_TAG v3.2.5
   GIT_SHALLOW ON
)
FetchContent_MakeAvailable(wxWidgets)

# add_subdirectory(src)
set(ENTRY src/main.cpp)

# add the resource files
set(RESOURCE_FILES
    rss/settings/layouts.xml
    rss/settings/Nimslo3DLayout.xml
    rss/settings/Nimslo3DLayout.png)

if(APPLE)
    # create bundle on apple compiles
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${ENTRY} ${RESOURCE_FILES})

    # target_link_libraries(${PROJECT_NAME} PRIVATE wxcore wxnet wxxml heymath mul)

    # Set a custom plist file for the app bundle - needed for Mac OS Retina display
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.dkeners.${PROJECT_NAME}"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        # RESOURCE "${RESOURCE_FILES}"
        )

    set_source_files_properties(${RESOURCE_FILES} 
        PROPERTIES 
        MACOSX_PACKAGE_LOCATION "Resources/settings" )

    install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .)
else()
    # the WIN32 is needed for Windows in order for it to look for WinMain
    # instead of the main function. This is ignored on other systems,
    # so it works on all platforms
    add_executable(${PROJECT_NAME} WIN32 ${ENTRY} main.exe.manifest)
    # target_link_libraries(${PROJECT_NAME} PRIVATE wxcore wxnet wxxml)
endif()

# Recursively add all files in the src directory to the executable
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE wxcore wxnet wxxml)