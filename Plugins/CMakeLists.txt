cmake_minimum_required(VERSION 3.16)

include(SMSDK)

# Load all Plugin subdirectories
FILE(GLOB subdirs RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*Plugin)
FOREACH(subdir ${subdirs})
    IF(EXISTS "${CMAKE_CURRENT_LIST_DIR}/${subdir}/CMakeLists.txt")
        ADD_SUBDIRECTORY(${subdir})
    ENDIF()
ENDFOREACH()
