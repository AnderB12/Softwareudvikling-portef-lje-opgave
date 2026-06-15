#include "Cave.hpp"
#include <iostream>
#include <cstdlib>

Cave::Cave(int playerLevel) {
    generate(playerLevel);
}

Cave::~Cave() {}

std::string Cave::getName() const { return name; }

const std::vector<Monster>& Cave::getMonsters() const { return monsters; }

bool Cave::isCleared() const {
    for (const auto& m : monsters) {
        if (m.getHealth() > 0) return false;
    }
    return true;
}

Item Cave::generateReward() const {
    return Item::getRandomReward();
}

void Cave::generate(int playerLevel) {
    if      (playerLevel <= 10) name = "Shallow Cave";
    else if (playerLevel <= 25) name = "Dark Grotto";
    else if (playerLevel <= 50) name = "Cursed Cavern";
    else                        name = "Ancient Abyss";

    int monsterCount = 2 + (playerLevel / 20);
    if (monsterCount > 5) monsterCount = 5;

    std::vector<std::string> names = {
        "Cave Bat", "Stone Golem", "Shadow Wolf",
        "Rock Troll", "Cave Spider", "Dungeon Rat"
    };

    for (int i = 0; i < monsterCount; i++) {
        // Each monster gets a fraction of the player's total power
        int attackPower = (playerLevel / monsterCount) * 0.6 + (rand() % 3);
        int health = 10 + (attackPower * 1) + (rand() % 5);
        std::string monsterName = names[rand() % names.size()];
        monsters.push_back(Monster(monsterName, health, attackPower));
    }
}