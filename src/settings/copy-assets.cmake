file(COPY ${CMAKE_CURRENT_LIST_DIR} DESTINATION ${MY_PATH}
    FILES_MATCHING PATTERN "*.xml" PATTERN "*.png")
message(STATUS "Copy ${CMAKE_CURRENT_LIST_DIR} to ${MY_PATH}")