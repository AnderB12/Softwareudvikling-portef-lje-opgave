#pragma once
#include <string>
#include <vector>

class Monster;

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

    void use(Monster& user, Monster& target);

    static std::vector<Item> getItemList();
    static Item getRandomReward();

private:
    std::string name;
    std::string description;
    ItemEffect effect;
    int magnitude;
    int duration;
};