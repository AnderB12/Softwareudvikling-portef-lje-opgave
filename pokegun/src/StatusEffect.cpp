#include "StatusEffect.hpp"
#include "Monster.hpp"
#include <iostream>

StatusEffect::StatusEffect(std::string name, int duration, int magnitude)
    : name(name), duration(duration), magnitude(magnitude) {}

StatusEffect::~StatusEffect() {}

std::string StatusEffect::getName() const    { return name; }
int StatusEffect::getDuration() const        { return duration; }
int StatusEffect::getMagnitude() const       { return magnitude; }
bool StatusEffect::isExpired() const         { return duration <= 0; }

void StatusEffect::tick(Monster& target) {
    if (isExpired()) return;

    if (name == "Poison") {
        std::cout << target.getName() << " takes " << magnitude
                  << " poison damage!" << std::endl;
        target.takeDamage(magnitude);

    } else if (name == "Burn") {
        std::cout << target.getName() << " takes " << magnitude
                  << " burn damage!" << std::endl;
        target.takeDamage(magnitude);

    } else if (name == "Regen") {
        std::cout << target.getName() << " regenerates " << magnitude
                  << " HP!" << std::endl;
        target.heal(magnitude);

    } else if (name == "Stun") {
        std::cout << target.getName() << " is stunned!" << std::endl;
        target.setStunned(true);
    }

    duration--;
}