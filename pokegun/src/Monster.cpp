#include "Monster.hpp"

//Constructor for the Monster class.
Monster::Monster(std::string name, int health, int attackPower) 
    : name(name), health(health), attackPower(attackPower) {}

//Destructor for the Monster class.
Monster::~Monster() {}

//Getters and setters for the monster's attributes.
std::string Monster::getName() const {
    return name;
}

int Monster::getHealth() const {
    return health;
}

int Monster::getAttackPower() const {
    return attackPower;
}

void Monster::setHealth(int health) {
    this->health = health;
}

void Monster::setAttackPower(int attackPower) {
    this->attackPower = attackPower;
}

void Monster::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0; // Ensure health doesn't go below 0
    }
}