#include <string>
#include <vector>

//Monster class.
class Monster {
public:

    //Constructor/Destructor for the Monster class.
    Monster(std::string name, int health, int attackpower);
    
    ~Monster();
    

    //Getters and setters for the monster's attributes.
    std::string getName() const;
    int getHealth() const;
    int getAttackPower() const;
    void setHealth(int health);
    void setAttackPower(int attackPower);
    void takeDamage(int damage);

private:

    //Attributes for the monster's name, health, attack power etc.
    std::string name;
    int health;
    int attackPower;

};
