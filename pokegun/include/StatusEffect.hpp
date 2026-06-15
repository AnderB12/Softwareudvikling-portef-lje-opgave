#pragma once
#include <string>

class Monster;

class StatusEffect {
public:
    StatusEffect(std::string name, int duration, int magnitude);
    ~StatusEffect();

    std::string getName() const;
    int getDuration() const;
    int getMagnitude() const;

    void tick(Monster& target);
    bool isExpired() const;

private:
    std::string name;
    int duration;
    int magnitude;
};