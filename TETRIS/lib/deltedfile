# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(Tetris VERSION 1.0)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find SDL2 and its components
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Add subdirectories for different components
add_subdirectory(src)
add_subdirectory(tests)

# Specify include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

# Create executable
add_executable(${PROJECT_NAME} src/main.c)

# Link against all the libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    tetris_core
    tetris_game
    tetris_audio
    tetris_graphics
    tetris_ui
    tetris_utils
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

# Copy assets to build directory
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# src/CMakeLists.txt
add_subdirectory(core)
add_subdirectory(game)
add_subdirectory(audio)
add_subdirectory(graphics)
add_subdirectory(ui)
add_subdirectory(utils)
add_subdirectory(system)

# src/core/CMakeLists.txt
add_library(tetris_core
    tetromino/blocks.c
    tetromino/rotation.c
    board/board.c
    collision/collision.c
)

target_include_directories(tetris_core
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

# src/game/CMakeLists.txt
add_library(tetris_game
    game.c
    modes/endless.c
    modes/level.c
    scoring/score.c
)

target_link_libraries(tetris_game
    PRIVATE
    tetris_core
    tetris_utils
)

# src/audio/CMakeLists.txt
add_library(tetris_audio
    audio_manager.c
    music.c
    sfx.c
)

target_link_libraries(tetris_audio
    PRIVATE
    ${SDL2_MIXER_LIBRARIES}
)

# src/graphics/CMakeLists.txt
add_library(tetris_graphics
    renderer.c
    animations/particles.c
    textures/texture_manager.c
)

target_link_libraries(tetris_graphics
    PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

# src/ui/CMakeLists.txt
add_library(tetris_ui
    menu/main_menu.c
    menu/pause_menu.c
    hud/score_display.c
    hud/next_preview.c
    settings/settings_menu.c
)

target_link_libraries(tetris_ui
    PRIVATE
    tetris_graphics
    tetris_utils
)

# src/utils/CMakeLists.txt
add_library(tetris_utils
    timer.c
    config.c
    save_load.c
)

# src/system/CMakeLists.txt
add_library(tetris_system
    sdl_init.c
)

target_link_libraries(tetris_system
    PRIVATE
    ${SDL2_LIBRARIES}
)

# tests/CMakeLists.txt
enable_testing()

add_executable(test_game test_game.c)
target_link_libraries(test_game tetris_game)
add_test(NAME test_game COMMAND test_game)

add_executable(test_blocks test_blocks.c)
target_link_libraries(test_blocks tetris_core)
add_test(NAME test_blocks COMMAND test_blocks)
