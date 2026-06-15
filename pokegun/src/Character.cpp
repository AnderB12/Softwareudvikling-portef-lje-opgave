#include "Character.hpp"

//Constructor/Destructor for the Character class.
Character::Character(std::string name) 
    : name(name) {}

Character::~Character() {}

std::string Character::getName() const {
    return name;
}

//Getters and setters for the character's attributes.
const std::vector<Monster>& Character::getMonsters() const {
    return monsters;
}

//Methods to add/remove monsters from the character's team.
void Character::addMonster(const Monster& monster) {
    if (monsters.size() < 4) {
        monsters.push_back(monster);
    }
}

void Character::removeMonster(int index) {
    if (index >= 0 && index < monsters.size()) {
        monsters.erase(monsters.begin() + index);
    }
}

//Check methods.
bool Character::isFull() const {
    return monsters.size() >= 4;
}

int Character::getPlayerLevel() const {
    int total = 0;
    for (const auto& m : monsters) {
        total += m.getAttackPower();
    }
    return total;
}

void Character::giveItem(const Item& item, int monsterIndex) {
    monsters[monsterIndex].addItem(item);
}