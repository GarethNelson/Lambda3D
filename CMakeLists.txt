cmake_minimum_required (VERSION 3.2)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeFiles")
project (Lambda3D)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(FREETYPE2 REQUIRED freetype2)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)

find_package(OpenGL REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS}
                    ${FREETYPE2_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include)



if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
   set(EXTRA_GL GL)
endif()


file(GLOB engine_SRC
     "src/*.c"
     "src/*.h")
add_executable(lambda ${engine_SRC})
set_target_properties(lambda PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)
target_link_libraries(lambda pthread physfs dl ${OPENGL_LIBRARIES} ${EXTRA_GL} ${FREETYPE2_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY} m dl)

