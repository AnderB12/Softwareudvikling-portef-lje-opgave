#include "Game.hpp"
#include "Monster.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Items.hpp"
#include "StatusEffect.hpp"
 
Game::Game() : character(nullptr) {}
 
Game::~Game() {
    delete character;
}
 
void Game::run() {
    showStartScreen();
}
 
void Game::showStartScreen() {
    std::cout << "=======================" << std::endl;
    std::cout << "  Welcome to Some Game!  " << std::endl;
    std::cout << "   1. New character    " << std::endl;
    std::cout << "   2. Exit             " << std::endl;
    std::cout << "=======================" << std::endl;
 
    int choice;
    std::cin >> choice;
    while (choice != 1 && choice != 2) {
        std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
        std::cin >> choice;
    }
 
    if (choice == 1) {
        createCharacter();
    } else {
        std::cout << "Goodbye!" << std::endl;
    }
}
 
void Game::createCharacter() {
    std::string name;
    std::cout << "Enter your character's name: ";
    std::cin >> name;
 
    delete character;
    character = new Character(name);
    character->addMonster(Monster("Horse", 8, 2));
    character->addMonster(Monster("Horse", 8, 2));
 
    showMainMenu();
}
 
void Game::showMainMenu() {
    while (true) {
        if (character == nullptr) return;
        
        std::cout << "===================" << std::endl;
        std::cout << "     Main Menu     " << std::endl;
        std::cout << "===================" << std::endl;
        std::cout << " 1. View Character " << std::endl;
        std::cout << " 2. Battle         " << std::endl;
        std::cout << " 3. Cave         " << std::endl;
        std::cout << " 4. Exit           " << std::endl;
        std::cout << "===================" << std::endl;
 
        int choice;
        std::cin >> choice;
 
        if (choice == 1) {
            std::cout << "Character Name: " << character->getName() << std::endl;
            const auto& monsters = character->getMonsters();
            for (size_t i = 0; i < monsters.size(); ++i) {
                std::cout << i + 1 << ". " << monsters[i].getName()
                          << " (Health: " << monsters[i].getHealth()
                          << ", Attack Power: " << monsters[i].getAttackPower()
                          << ")" << std::endl;
            }
        } else if (choice == 2) {
            battle();
        } else if (choice == 3) {
            runCave();
        } else if (choice == 4) {
            std::cout << "Goodbye!" << std::endl;
            return;
        } else {
            std::cout << "Invalid choice. Please enter a number between 1 and 4." << std::endl;
        }
    }
}
 
static bool playerTurnAction(Monster& playerMonster, Monster& enemyMonster) {
    const auto& items = playerMonster.getItems();

    std::cout << "--- Your turn ---" << std::endl;
    std::cout << "1. Attack" << std::endl;
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << i + 2 << ". Use " << items[i].getName()
                  << " - " << items[i].getDescription() << std::endl;
    }

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Your " << playerMonster.getName() << " attacks!" << std::endl;
        enemyMonster.takeDamage(playerMonster.getAttackPower());
        std::cout << "Enemy " << enemyMonster.getName()
                  << " has " << enemyMonster.getHealth() << " HP left." << std::endl;
        return true;
    }

    int itemIndex = choice - 2;
    if (itemIndex >= 0 && itemIndex < static_cast<int>(items.size())) {
        Item itemCopy = items[itemIndex];
        itemCopy.use(playerMonster, enemyMonster);
        return true;
    }

    std::cout << "Invalid choice." << std::endl;
    return false;
}

void Game::battle() {
    std::vector<Monster> enemies = {
        Monster("Horse",            8,   2),
        Monster("Sparrow",          15,  4),
        Monster("Weak Goblin",      25,  7),
        Monster("Strong Goblin",    40,  10),
        Monster("Orc Warrior",      60,  14),
        Monster("Cave Troll",       85,  18),
        Monster("Dark Knight",      120, 24),
        Monster("Ancient Dragon",   200, 35),
    };

    std::cout << "Choose a monster to battle:" << std::endl;
    for (size_t i = 0; i < enemies.size(); ++i) {
        std::cout << i + 1 << ". " << enemies[i].getName()
                  << " (HP: " << enemies[i].getHealth()
                  << ", ATK: " << enemies[i].getAttackPower()
                  << ")" << std::endl;
    }

    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > static_cast<int>(enemies.size())) {
        std::cout << "Invalid. Enter 1-" << enemies.size() << ": ";
        std::cin >> choice;
    }

    if (character->getMonsters().empty()) {
        std::cout << "You have no monsters!" << std::endl;
        return;
    }

    Monster enemyMonster = enemies[choice - 1];
    int originalHealth = enemyMonster.getHealth();
    bool playerTurn = rand() % 2 == 0;
    bool playerWon = false;

    for (size_t i = 0; i < character->getMonsters().size(); ++i) {
        Monster playerMonster = character->getMonsters()[i];
        playerMonster.resetItems();

        if (i > 0) {
            std::cout << playerMonster.getName() << " is up next!" << std::endl;
        }

        bool firstTurn = true;
        while (playerMonster.getHealth() > 0 && enemyMonster.getHealth() > 0) {
            if (playerTurn) {
                playerMonster.tickStatuses();
                if (playerMonster.getHealth() <= 0) break;

                if (firstTurn) { std::cout << "You go first!" << std::endl; firstTurn = false; }

                if (playerMonster.isStunned()) {
                    std::cout << playerMonster.getName() << " is stunned and skips their turn!" << std::endl;
                } else {
                    while (!playerTurnAction(playerMonster, enemyMonster));
                }
            } else {
                enemyMonster.tickStatuses();
                if (enemyMonster.getHealth() <= 0) break;

                if (firstTurn) { std::cout << "Enemy goes first!" << std::endl; firstTurn = false; }

                if (enemyMonster.isStunned()) {
                    std::cout << enemyMonster.getName() << " is stunned and skips their turn!" << std::endl;
                } else {
                    std::cout << "Enemy " << enemyMonster.getName() << " attacks!" << std::endl;
                    playerMonster.takeDamage(enemyMonster.getAttackPower());
                    std::cout << "Your " << playerMonster.getName()
                              << " has " << playerMonster.getHealth() << " HP left." << std::endl;
                }
            }
            playerTurn = !playerTurn;
        }

        if (enemyMonster.getHealth() <= 0) {
            playerWon = true;
            break;
        }
        std::cout << playerMonster.getName() << " fainted!" << std::endl;
    }

    if (playerWon) {
        std::cout << "You won the battle!" << std::endl;

        std::cout << "Do you want to add " << enemyMonster.getName()
                  << " to your team? (y/n): ";
        char addChoice;
        std::cin >> addChoice;

        if (addChoice == 'y' || addChoice == 'Y') {
            enemyMonster.setHealth(originalHealth);

            if (!character->isFull()) {
                character->addMonster(enemyMonster);
                std::cout << enemyMonster.getName() << " added to your team!" << std::endl;
            } else {
                std::cout << "Your team is full! Choose a monster to replace:" << std::endl;
                const auto& teamMonsters = character->getMonsters();
                for (size_t i = 0; i < teamMonsters.size(); ++i) {
                    std::cout << i + 1 << ". " << teamMonsters[i].getName()
                              << " (HP: " << teamMonsters[i].getHealth() << ")" << std::endl;
                }
                int replaceChoice;
                std::cin >> replaceChoice;
                while (replaceChoice < 1 || replaceChoice > static_cast<int>(teamMonsters.size())) {
                    std::cout << "Invalid. Enter 1-" << teamMonsters.size() << ": ";
                    std::cin >> replaceChoice;
                }
                character->removeMonster(replaceChoice - 1);
                character->addMonster(enemyMonster);
                std::cout << enemyMonster.getName() << " added to your team!" << std::endl;
            }
        }
    } else {
        std::cout << "You lost! All your monsters fainted." << std::endl;
        delete character;
        character = nullptr;
        showStartScreen();
        return;
    }
}

void Game::runCave() {
    Cave cave(character->getPlayerLevel());

    std::cout << "You entered the " << cave.getName() << "!" << std::endl;
    std::cout << "There are " << cave.getMonsters().size()
              << " monsters inside." << std::endl;

    std::vector<Monster> caveMonsters = cave.getMonsters();
    for (size_t i = 0; i < caveMonsters.size(); ++i) {
        Monster& enemyMonster = caveMonsters[i];

        std::cout << "\nA " << enemyMonster.getName() << " appears! "
                  << "(HP: " << enemyMonster.getHealth()
                  << ", ATK: " << enemyMonster.getAttackPower()
                  << ")" << std::endl;

        bool playerWon = false;
        bool playerTurn = rand() % 2 == 0;

        for (size_t j = 0; j < character->getMonsters().size(); ++j) {
            Monster playerMonster = character->getMonsters()[j];
            playerMonster.resetItems();

            if (j > 0) {
                std::cout << playerMonster.getName() << " is up next!" << std::endl;
            }

            bool firstTurn = true;
            while (playerMonster.getHealth() > 0 && enemyMonster.getHealth() > 0) {
                if (playerTurn) {
                    playerMonster.tickStatuses();
                    if (playerMonster.getHealth() <= 0) break;

                    if (firstTurn) {
                        std::cout << "You go first!" << std::endl;
                        firstTurn = false;
                    }

                    if (playerMonster.isStunned()) {
                        std::cout << playerMonster.getName()
                                  << " is stunned and skips their turn!" << std::endl;
                    } else {
                        while (!playerTurnAction(playerMonster, enemyMonster));
                    }
                } else {
                    enemyMonster.tickStatuses();
                    if (enemyMonster.getHealth() <= 0) break;

                    if (firstTurn) {
                        std::cout << "Enemy goes first!" << std::endl;
                        firstTurn = false;
                    }

                    if (enemyMonster.isStunned()) {
                        std::cout << enemyMonster.getName()
                                  << " is stunned and skips their turn!" << std::endl;
                    } else {
                        std::cout << "Enemy " << enemyMonster.getName() << " attacks!" << std::endl;
                        playerMonster.takeDamage(enemyMonster.getAttackPower());
                        std::cout << "Your " << playerMonster.getName()
                                  << " has " << playerMonster.getHealth()
                                  << " HP left." << std::endl;
                    }
                }
                playerTurn = !playerTurn;
            }

            if (enemyMonster.getHealth() <= 0) {
                playerWon = true;
                break;
            }
            std::cout << playerMonster.getName() << " fainted!" << std::endl;
        }

        if (!playerWon) {
            std::cout << "You were defeated in the cave!" << std::endl;
            delete character;
            character = nullptr;
            showStartScreen();
            return;
        }

        std::cout << "You defeated the " << enemyMonster.getName() << "!" << std::endl;
    }

    std::cout << "\nYou cleared the " << cave.getName() << "!" << std::endl;
    Item reward = cave.generateReward();
    std::cout << "You found: " << reward.getName()
              << " - " << reward.getDescription() << std::endl;

    std::cout << "Give " << reward.getName() << " to which monster?" << std::endl;
    const auto& team = character->getMonsters();
    for (size_t i = 0; i < team.size(); ++i) {
        std::cout << i + 1 << ". " << team[i].getName() << std::endl;
    }
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > static_cast<int>(team.size())) {
        std::cout << "Invalid. Enter 1-" << team.size() << ": ";
        std::cin >> choice;
    }
    character->giveItem(reward, choice - 1);
    std::cout << reward.getName() << " given to "
              << team[choice - 1].getName() << "!" << std::endl;
}