#pragma once
#include <string>
#include <vector>

enum class ItemEffect {
    PoisonTarget,
    BurnTarget,
    StunTarget,
    RegenSelf,
    BoostAttack,
};

class Item {
public:
    Item(std::string name, std::string description, ItemEffect effect,
         int magnitude, int duration);
    ~Item();

    std::string getName() const;
    std::string getDescription() const;
    ItemEffect getEffect() const;
    int getMagnitude() const;
    int getDuration() const;

    static std::vector<Item> getItemList();

    static Item getRandomReward();

private:
    std::string name;
    std::string description;
    ItemEffect effect;
    int magnitude;
    int duration;
};