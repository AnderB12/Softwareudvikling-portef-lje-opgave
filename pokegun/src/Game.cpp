#include <iostream>
#include <vector>
#include <string>
#include "Game.hpp"


// Forward declarations
void createCharacter();
void showMainMenu(Character& character);
void battle(Character& character);

void showStartScreen() {
    std::cout << "=======================" << std::endl;
    std::cout << "  Welcome to O-Block!  " << std::endl;
    std::cout << "   1. Continue Game    " << std::endl;
    std::cout << "   2. New character    " << std::endl;
    std::cout << "   3. Exit             " << std::endl;
    std::cout << "=======================" << std::endl;
    int choice;
    std::cin >> choice;
    while (choice != 1 && choice != 2 && choice != 3) {
        std::cout << "Invalid choice. Please enter 1, 2 or 3." << std::endl;
        std::cin >> choice;
    }
    
    if (choice == 2) {
        createCharacter();
    } 
    
    else if (choice == 3) {
        std::cout << "Goodbye!" << std::endl;
        exit(0);
    }

}
void createCharacter() {
    std::string name;
    std::cout << "Enter your character's name: ";
    std::cin >> name;
    Character character(name);
    character.addMonster(Monster("Horse", 35, 30));
    character.addMonster(Monster("Dog", 45, 30));
    showMainMenu(character);
}

// Battle function to handle the battle logic between the player's character and other monsters.
void battle(Character& character) {
    // A list of 4 monsters to choose to fight.
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

    // Player chooses a monster to battle.
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > static_cast<int>(monsters.size())) {
        std::cout << "Invalid choice. Please enter a number between 1 and " << monsters.size() << "." << std::endl;
        std::cin >> choice;
    }

    // Safety check.
    if (character.getMonsters().empty()) {
        std::cout << "You have no monsters to battle with!" << std::endl;
        return;
    }

    // Declare enemyMonster ONCE before the loop, and store original health.
    Monster enemyMonster = monsters[choice - 1];
    int originalHealth = enemyMonster.getHealth();
    bool playerTurn = rand() % 2 == 0;
    bool playerWon = false;
    int turnCount = 0;

    //Loop through the player's monsters, switching if one faints.
    for (size_t i = 0; i < character.getMonsters().size(); ++i) {
        Monster playerMonster = character.getMonsters()[i];
        
        if (i > 0) {
            std::cout << playerMonster.getName() << " is up next!" << std::endl;
        }

        //Turn-based battle system.
        while (playerMonster.getHealth() > 0 && enemyMonster.getHealth() > 0) {
            if (turnCount == 0) {
                std::cout << (playerTurn ? "You go first!" : "Enemy goes first!") << std::endl;
            }
            if (playerTurn) {
                std::cout << "Your " << playerMonster.getName() << " attacks!" << std::endl;
                enemyMonster.takeDamage(playerMonster.getAttackPower());
                std::cout << "Enemy " << enemyMonster.getName() << " has " << enemyMonster.getHealth() << " health left." << std::endl;
                turnCount++;
            } else {
                std::cout << "Enemy " << enemyMonster.getName() << " attacks!" << std::endl;
                playerMonster.takeDamage(enemyMonster.getAttackPower());
                std::cout << "Your " << playerMonster.getName() << " has " << playerMonster.getHealth() << " health left." << std::endl;
                turnCount++;
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
        if (!character.isFull()) {
            // Team is not full, ask if they want to add.
            std::cout << "Do you want to add " << enemyMonster.getName() << " to your team? (y/n): ";
            char addChoice;
            std::cin >> addChoice;
            if (addChoice == 'y' || addChoice == 'Y') {
                enemyMonster.setHealth(originalHealth);
                character.addMonster(enemyMonster);
                std::cout << enemyMonster.getName() << " has been added to your team!" << std::endl;
            }
        } else {
            // Team is full, ask if they want to swap.
            std::cout << "Do you want to add " << enemyMonster.getName() << " to your team? (y/n): ";
            char addChoice;
            std::cin >> addChoice;
            if (addChoice == 'y' || addChoice == 'Y') {
                std::cout << "Your team is full! Choose a monster to replace:" << std::endl;
                const auto& teamMonsters = character.getMonsters();
                for (size_t i = 0; i < teamMonsters.size(); ++i) {
                    std::cout << i + 1 << ". " << teamMonsters[i].getName() 
                              << " (Health: " << teamMonsters[i].getHealth() 
                              << ", Attack Power: " << teamMonsters[i].getAttackPower() 
                              << ")" << std::endl;
                }
                int replaceChoice;
                std::cin >> replaceChoice;
                while (replaceChoice < 1 || replaceChoice > static_cast<int>(teamMonsters.size())) {
                    std::cout << "Invalid choice. Please enter a number between 1 and " << teamMonsters.size() << "." << std::endl;
                    std::cin >> replaceChoice;
                }
                character.removeMonster(replaceChoice - 1);
                enemyMonster.setHealth(originalHealth);
                character.addMonster(enemyMonster);
                std::cout << enemyMonster.getName() << " has been added to your team!" << std::endl;
            }
        }
    } else {
        std::cout << "You lost the battle! All your monsters fainted." << std::endl;
        showStartScreen();
    }
}

void showMainMenu(Character& character) {
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
            std::cout << "Character Name: " << character.getName() << std::endl;
            const auto& monsters = character.getMonsters();
            for (size_t i = 0; i < monsters.size(); ++i) {
                std::cout << i + 1 << ". " << monsters[i].getName() 
                          << " (Health: " << monsters[i].getHealth() 
                          << ", Attack Power: " << monsters[i].getAttackPower() 
                          << ")" << std::endl;
            }
        } else if (choice == 2) {
            battle(character);
        } else {
            std::cout << "Goodbye!" << std::endl;
            exit(0);
        }
    }
}
