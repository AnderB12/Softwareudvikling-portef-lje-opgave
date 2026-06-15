#pragma once
#include <string>
#include <vector>
#include "Monster.hpp"

//Character class.
class Character {
public:         

    //Constructor/Destructor for the Character class.
    Character(std::string name);
    
    ~Character();

    //Getters and setters for the character's attributes.
    std::string getName() const;
    const std::vector<Monster>& getMonsters() const;
    int getPlayerLevel() const;

    //methonds to add/remove monsters from the character's team.
    void addMonster(const Monster& monster);
    void removeMonster(int index);
    void giveItem(const Item& item, int monsterIndex);

    //Check methods.
    bool isFull() const;

private:

    std::string name;
    std::vector<Monster> monsters;

};