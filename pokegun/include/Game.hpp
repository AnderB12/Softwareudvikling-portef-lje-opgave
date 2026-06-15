#pragma once
#include <string>
#include "Character.hpp"
#include "Cave.hpp"
 
class Game {
public:
    Game();
    ~Game();
 
    void run();
    void runCave();
 
private:
    Character* character;
 
    void showStartScreen();
    void createCharacter();
    void showMainMenu();
    void battle();
};
