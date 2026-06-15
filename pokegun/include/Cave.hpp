#pragma once
#include <string>
#include <vector>
#include "Monster.hpp"
#include "Items.hpp"

class Cave {
public:
    Cave(int playerLevel);
    ~Cave();

    const std::vector<Monster>& getMonsters() const;
    std::string getName() const;
    bool isCleared() const;
    Item generateReward() const;

private:
    std::string name;
    std::vector<Monster> monsters;

    void generate(int playerLevel);
};