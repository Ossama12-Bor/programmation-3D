cmake_minimum_required(VERSION 3.11.0)

project(td3_enseirb)


add_subdirectory(ext/glfw)
add_subdirectory(ext/glbinding)

include_directories(SYSTEM ext/glfw/include)
include_directories(SYSTEM ext/glbinding/include)
include_directories(SYSTEM ext/eigen3)
include_directories(SYSTEM ext/)

set (CMAKE_CXX_STANDARD 11)

set(SRC_FILES
    ext/ObjFormat/ObjFormat.cpp
    ext/ObjFormat/ObjUtil.cpp
    src/main.cpp
    src/viewer.cpp
    src/viewer.h
    src/shader.cpp
    src/shader.h
    src/opengl.h
    src/camera.h
    src/camera.cpp
    src/mesh.h
    src/mesh.cpp
    src/trackball.cpp
    src/trackball.h
)

add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/data")

add_executable(mds3d_glviewer ${SRC_FILES})

target_include_directories(mds3d_glviewer SYSTEM INTERFACE ext/glfw/include)
target_include_directories(mds3d_glviewer SYSTEM INTERFACE ext/glbinding)
target_include_directories(mds3d_glviewer SYSTEM INTERFACE ext/glfw/include)
target_include_directories(mds3d_glviewer SYSTEM INTERFACE ext/glbinding)

target_link_libraries(mds3d_glviewer glfw ${GLFW_LIBRARIES} glbinding)

function(IndicateExternalFile _target)
    foreach(_file IN ITEMS ${ARGN})
        if ((IS_ABSOLUTE "${_file}" AND EXISTS "${_file}") OR
            (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_file}"))
            target_sources(${_target} PRIVATE "${_file}")
        endif()
    endforeach()
endfunction()

file(GLOB SHADERS "data/shaders/*.vert" "data/shaders/*.frag")
IndicateExternalFile("mds3d_glviewer" ${SHADERS})

