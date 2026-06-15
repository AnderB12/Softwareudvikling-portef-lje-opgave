cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

find_package(SQLite3 REQUIRED)

add_executable(SomeGame
    src/main.cpp
    src/Monster.cpp
    src/Character.cpp
    src/Game.cpp
    src/Items.cpp
    src/StatusEffect.cpp
    src/Cave.cpp
    src/Database.cpp
    
)

target_link_libraries(SomeGame SQLite::SQLite3)