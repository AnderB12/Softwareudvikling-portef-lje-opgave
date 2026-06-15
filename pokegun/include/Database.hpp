#pragma once
#include <string>
#include <vector>
#include "Character.hpp"
#include <sqlite3.h>

class Database {
public:
    Database(const std::string& dbPath);
    ~Database();

    // Setup
    void initialize();  // creates tables if they don't exist

    // Save / Load
    void saveCharacter(const Character& character);
    Character loadCharacter(int characterId);
    std::vector<std::pair<int, std::string>> listCharacters();  // returns {id, name} pairs

    // Stats
    void incrementMonstersDefeated(int amount = 1);
    void incrementItemUsed(const std::string& characterName, const std::string& itemName);
    void incrementMonsterUsed(const std::string& characterName, const std::string& monsterName);
    void incrementItemKill(const std::string& characterName, const std::string& itemName);

    // Stat queries
    int getTotalMonstersDefeated();
    std::string getMostUsedItem(const std::string& characterName);
    std::string getMostUsedMonster(const std::string& characterName);
    std::string getMostItemKills(const std::string& characterName);

private:
    sqlite3* db;

    void execute(const std::string& sql);  // helper for statements with no return
};