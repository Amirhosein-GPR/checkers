cmake_minimum_required(VERSION 3.0)
project(CHECKERS)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)
add_executable(checkers src/main.cpp src/core/Engine.cpp src/core/Object.cpp src/core/System.cpp src/core/Timer.cpp src/objects/Piece.cpp src/objects/BluePiece.cpp src/objects/RedPiece.cpp src/objects/Selector.cpp)
target_link_libraries(checkers SDL2 SDL2_image SDL2_ttf SDL2_mixer)