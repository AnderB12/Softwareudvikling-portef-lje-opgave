#include "Items.hpp"
#include "Monster.hpp"
#include "StatusEffect.hpp"
#include <cstdlib>
#include <iostream>


Item::Item(std::string name, std::string description, ItemEffect effect,
           int magnitude, int duration)
    : name(name), description(description), effect(effect),
      magnitude(magnitude), duration(duration) {}

Item::~Item() {}

std::string Item::getName() const        { return name; }
std::string Item::getDescription() const { return description; }
ItemEffect  Item::getEffect() const      { return effect; }
int         Item::getMagnitude() const   { return magnitude; }
int         Item::getDuration() const    { return duration; }

std::vector<Item> Item::getItemList() {
    return {
        //       name                    description                                     effect     mag  dur
        Item("Poison Fang",   "Poisons the target for 3 turns.",          ItemEffect::PoisonTarget,  3,  3),
        Item("Venom Sac",     "Poisons the target for 5 turns.",          ItemEffect::PoisonTarget,  4,  5),
        Item("Ember Shard",   "Burns the target for 2 turns.",            ItemEffect::BurnTarget,    5,  2),
        Item("Inferno Stone", "Burns the target for 4 turns.",            ItemEffect::BurnTarget,    8,  4),
        Item("Stun Spore",    "Stuns the target for 1 turn.",             ItemEffect::StunTarget,    0,  1),
        Item("Thunder Seed",  "Stuns the target for 2 turns.",            ItemEffect::StunTarget,    0,  2),
        Item("Healing Moss",  "Regenerates 5 HP per turn for 3 turns.",   ItemEffect::RegenSelf,     5,  3),
        Item("Vitality Root", "Regenerates 8 HP per turn for 4 turns.",   ItemEffect::RegenSelf,     8,  4),
        Item("Sharp Claw",    "Permanently boosts attack by 3.",          ItemEffect::BoostAttack,   3,  0),
        Item("Dragon Scale",  "Permanently boosts attack by 7.",          ItemEffect::BoostAttack,   7,  0),
    };
}

Item Item::getRandomReward() {
    std::vector<Item> catalogue = getItemList();
    return catalogue[rand() % catalogue.size()];
}

void Item::use(Monster& user, Monster& target) {
    std::cout << user.getName() << " uses " << name << "!" << std::endl;

    switch (effect) {
        case ItemEffect::PoisonTarget:
            target.addStatus(StatusEffect("Poison", duration, magnitude));
            std::cout << target.getName() << " is poisoned for "
                      << duration << " turns!" << std::endl;
            break;

        case ItemEffect::BurnTarget:
            target.addStatus(StatusEffect("Burn", duration, magnitude));
            std::cout << target.getName() << " is burning for "
                      << duration << " turns!" << std::endl;
            break;

        case ItemEffect::StunTarget:
            target.addStatus(StatusEffect("Stun", duration, magnitude));
            std::cout << target.getName() << " is stunned for "
                      << duration << " turns!" << std::endl;
            break;

        case ItemEffect::RegenSelf:
            user.addStatus(StatusEffect("Regen", duration, magnitude));
            std::cout << user.getName() << " starts regenerating for "
                      << duration << " turns!" << std::endl;
            break;

        case ItemEffect::BoostAttack:
            user.setAttackPower(user.getAttackPower() + magnitude);
            std::cout << user.getName() << "'s attack rose by "
                      << magnitude << "!" << std::endl;
            break;
    }
}