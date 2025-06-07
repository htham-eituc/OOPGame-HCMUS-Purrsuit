cmake_minimum_required(VERSION 3.16)
project(Purrsuit LANGUAGES CXX)

find_program(CMAKE_C_COMPILER NAMES gcc REQUIRED)
find_program(CMAKE_CXX_COMPILER NAMES g++ REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the root src directory
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Set include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${SRC_DIR}/app
    ${SRC_DIR}/app/loader    
    ${SRC_DIR}/app/state
    ${SRC_DIR}/core 
    ${SRC_DIR}/core/assets
    ${SRC_DIR}/domain
    ${SRC_DIR}/domain/entities
    ${SRC_DIR}/domain/entities/character
    ${SRC_DIR}/domain/entities/items
    ${SRC_DIR}/domain/entities/map
    ${SRC_DIR}/domain/value_object
    ${SRC_DIR}/repo
    ${SRC_DIR}/repo/map
    ${SRC_DIR}/ui
    ${SRC_DIR}/ui/renderer
    ${CMAKE_SOURCE_DIR}/external  # e.g., for Tileson header-only
)

# Gather source files
file(GLOB_RECURSE SOURCES
    ${SRC_DIR}/*.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Find SDL2 and SDL_image/ttf/mixer
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Link SDL2 libraries
target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)

# Set assets folder path as compile definition (optional)
# target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets")

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")
