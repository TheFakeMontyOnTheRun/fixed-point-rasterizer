cmake_minimum_required(VERSION 3.8)
project(timedemo)

find_package(SDL REQUIRED)

set(CMAKE_CXX_STANDARD 14)
include_directories(include)
include_directories(stb)
include_directories(fixed_point/include)
include_directories(core/noudar-core/include)
include_directories(EASTL/include )
include_directories(EASTL/test/packages/EABase/include/Common)

set(SOURCE_FILES
        SDLVersion/CSDLRenderer.cpp
        SDLVersion/main.cpp
        src/CRenderer.cpp
        src/RasterizerCommon.cpp
        src/NativeBitmap.cpp
        src/LoadPNG.cpp
        include/CRenderer.h
        include/RasterizerCommon.h
)

find_package(OpenGL REQUIRED)

include (FindPkgConfig)

add_executable(timedemo ${SOURCE_FILES})

target_link_libraries(timedemo ${OPENGL_LIBRARIES} ${SDL_LIBRARY}   )