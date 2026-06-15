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
    character->addMonster(Monster("Horse", 35, 30));
    character->addMonster(Monster("Dog", 45, 30));
 
    showMainMenu();
}
 
void Game::showMainMenu() {
    while (true) {
        std::cout << "===================" << std::endl;
        std::cout << "     Main Menu     " << std::endl;
        std::cout << "===================" << std::endl;
        std::cout << " 1. View Character " << std::endl;
        std::cout << " 2. Battle         " << std::endl;
        std::cout << " 3. Exit           " << std::endl;
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
        } else {
            std::cout << "Goodbye!" << std::endl;
            return;
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
    std::vector<Monster> monsters = {
        Monster("Horse", 4, 1),
        Monster("Weak Goblin", 4, 2),
        Monster("Strong Goblin", 8, 3),
        Monster("Strongest Goblin", 10, 4),
        Monster("Giant", 15, 5),
        Monster("Abe king", 30, 4),
        Monster("Beats", 50, 8),
        Monster("Giant", 100, 10),
    };
 
    std::cout << "Choose a monster to battle:" << std::endl;
    for (size_t i = 0; i < monsters.size(); ++i) {
        std::cout << i + 1 << ". " << monsters[i].getName()
                  << " (Health: " << monsters[i].getHealth()
                  << ", Attack Power: " << monsters[i].getAttackPower()
                  << ")" << std::endl;
    }
 
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > static_cast<int>(monsters.size())) {
        std::cout << "Invalid choice. Please enter a number between 1 and "
                  << monsters.size() << "." << std::endl;
        std::cin >> choice;
    }
 
    if (character->getMonsters().empty()) {
        std::cout << "You have no monsters to battle with!" << std::endl;
        return;
    }
 
    Monster enemyMonster = monsters[choice - 1];
    int originalHealth = enemyMonster.getHealth();
    bool playerTurn = rand() % 2 == 0;
    bool playerWon = false;
    int turnCount = 0;
 
    for (size_t i = 0; i < character->getMonsters().size(); ++i) {
        Monster playerMonster = character->getMonsters()[i];
 
        if (i > 0) {
            std::cout << playerMonster.getName() << " is up next!" << std::endl;
        }
 
        while (playerMonster.getHealth() > 0 && enemyMonster.getHealth() > 0) {
            if (turnCount == 0) {
                std::cout << (playerTurn ? "You go first!" : "Enemy goes first!") << std::endl;
            }
            if (playerTurn) {
            playerMonster.tickStatuses();
            if (playerMonster.getHealth() <= 0) break;      

            if (playerMonster.isStunned()) {
                std::cout << playerMonster.getName() 
                          << " is stunned and skips their turn!" << std::endl;
            } else {
                while (!playerTurnAction(playerMonster, enemyMonster));
            }
            } else {
                enemyMonster.tickStatuses();
                if (enemyMonster.getHealth() <= 0) break;
                        
                if (enemyMonster.isStunned()) {
                    std::cout << enemyMonster.getName() 
                              << " is stunned and skips their turn!" << std::endl;
                } else {
                    std::cout << "Enemy " << enemyMonster.getName() << " attacks!" << std::endl;
                    playerMonster.takeDamage(enemyMonster.getAttackPower());
                    std::cout << "Your " << playerMonster.getName()
                              << " has " << playerMonster.getHealth() << " HP left." << std::endl;
                }
            }
            playerTurn = !playerTurn;
            turnCount++;
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
                std::cout << enemyMonster.getName() << " has been added to your team!" << std::endl;
            } else {
                std::cout << "Your team is full! Choose a monster to replace:" << std::endl;
                const auto& teamMonsters = character->getMonsters();
                for (size_t i = 0; i < teamMonsters.size(); ++i) {
                    std::cout << i + 1 << ". " << teamMonsters[i].getName()
                              << " (Health: " << teamMonsters[i].getHealth()
                              << ", Attack Power: " << teamMonsters[i].getAttackPower()
                              << ")" << std::endl;
                }
                int replaceChoice;
                std::cin >> replaceChoice;
                while (replaceChoice < 1 || replaceChoice > static_cast<int>(teamMonsters.size())) {
                    std::cout << "Invalid choice. Please enter a number between 1 and "
                              << teamMonsters.size() << "." << std::endl;
                    std::cin >> replaceChoice;
                }
                character->removeMonster(replaceChoice - 1);
                character->addMonster(enemyMonster);
                std::cout << enemyMonster.getName() << " has been added to your team!" << std::endl;
            }
        }
    } else {
        std::cout << "You lost the battle! All your monsters fainted." << std::endl;
        delete character;
        character = nullptr;
        showStartScreen();
    }
}