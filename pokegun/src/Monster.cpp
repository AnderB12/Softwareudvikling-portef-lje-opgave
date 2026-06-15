#include "Monster.hpp"
#include "StatusEffect.hpp"
#include "Items.hpp"
#include <algorithm>

//Constructor for the Monster class.
Monster::Monster(std::string name, int health, int attackPower) 
    : name(name), health(health), attackPower(attackPower), stunned(false) {}

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
        health = 0;
    }
}

void Monster::heal(int amount) {
    health += amount;
}

void Monster::setStunned(bool stunned) {
    this->stunned = stunned;
}

bool Monster::isStunned() const {
    return stunned;
}

void Monster::addStatus(const StatusEffect& status) {
    statuses.push_back(status);
}

void Monster::tickStatuses() {
    stunned = false;  // reset stun before statuses re-apply it
    for (auto& s : statuses) {
        s.tick(*this);
    }
    // remove expired statuses
    statuses.erase(
        std::remove_if(statuses.begin(), statuses.end(),
            [](const StatusEffect& s) { return s.isExpired(); }),
        statuses.end()
    );
}

void Monster::addItem(const Item& item) {
    items.push_back(item);
}

const std::vector<Item>& Monster::getItems() const {
    return items;
}

void Monster::resetItems() {
    for (auto& item : items) {
        item.resetUsed();
    }
}