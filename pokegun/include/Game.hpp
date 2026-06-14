#pragma once
#include <string>
#include "Character.hpp"
 
class Game {
public:
    Game();
    ~Game();
 
    // Entry point — call this from main()
    void run();
 
private:
    Character* character;
 
    void showStartScreen();
    void createCharacter();
    void showMainMenu();
    void battle();
};
