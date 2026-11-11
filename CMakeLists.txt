cmake_minimum_required(VERSION 3.15)

project(VoxelEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(VoxelEngine ${SOURCES})

target_include_directories(VoxelEngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(WIN32)
    target_link_directories(VoxelEngine PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    )

    target_link_libraries(VoxelEngine PRIVATE
        opengl32
        glfw3
        assimp
        user32
        gdi32
        shell32
    )
endif()