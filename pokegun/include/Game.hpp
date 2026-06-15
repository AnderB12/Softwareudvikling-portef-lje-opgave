#pragma once
#include <string>
#include "Character.hpp"
#include "Cave.hpp"
#include "Database.hpp"
 
class Game {
public:
    Game();
    ~Game();
 
    void run();
    void runCave();
 
private:
    Character* character;
    Database* db;
 
    void showStartScreen();
    void createCharacter();
    void showMainMenu();
    void battle();
};
