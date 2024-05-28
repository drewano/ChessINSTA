cmake_minimum_required(VERSION 3.10)
project(ChessINSTA)

set(CMAKE_C_STANDARD 99)

# Définir les chemins vers les répertoires include et lib de SDL et SDL_image
set(SDL2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/SDL2/include)
set(SDL2_LIB_DIR ${CMAKE_SOURCE_DIR}/SDL2/lib)
set(SDL2_IMAGE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/SDL2_image/include)
set(SDL2_IMAGE_LIB_DIR ${CMAKE_SOURCE_DIR}/SDL2_image/lib)

# Ajouter les répertoires d'inclusion
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

# Ajouter les répertoires de bibliothèque
link_directories(${SDL2_LIB_DIR} ${SDL2_IMAGE_LIB_DIR})

# Définir les fichiers source
set(SOURCE_FILES main.c)

# Ajouter l'exécutable
add_executable(ChessINSTA ${SOURCE_FILES})

# Ajouter la définition pour utiliser SDL_main avec MinGW
target_compile_definitions(ChessINSTA PRIVATE "-Dmain=SDL_main")

# Lier les bibliothèques SDL2, SDL2_image et SDL2main
target_link_libraries(ChessINSTA SDL2 SDL2_image SDL2main)
