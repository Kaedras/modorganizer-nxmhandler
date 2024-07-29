cmake_minimum_required(VERSION 3.16)

project(nxmhandler)

if(UNIX)
    include(FindOpenGL)
endif()

add_subdirectory(src)
