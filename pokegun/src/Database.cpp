#include "Database.hpp"
#include "Monster.hpp"
#include "Items.hpp"
#include <iostream>
#include <stdexcept>

Database::Database(const std::string& dbPath) : db(nullptr) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
    }
}

Database::~Database() {
    if (db) sqlite3_close(db);
}

void Database::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string err(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error("SQL error: " + err);
    }
}

void Database::initialize() {
    // Characters table
    execute(R"(
        CREATE TABLE IF NOT EXISTS Characters (
            id      INTEGER PRIMARY KEY AUTOINCREMENT,
            name    TEXT NOT NULL
        );
    )");

    // Monsters belonging to a character
    execute(R"(
        CREATE TABLE IF NOT EXISTS Monsters (
            id           INTEGER PRIMARY KEY AUTOINCREMENT,
            character_id INTEGER NOT NULL,
            name         TEXT NOT NULL,
            health       INTEGER NOT NULL,
            attackPower  INTEGER NOT NULL,
            FOREIGN KEY (character_id) REFERENCES Characters(id)
        );
    )");

    // Items on a monster
    execute(R"(
        CREATE TABLE IF NOT EXISTS MonsterItems (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            monster_id  INTEGER NOT NULL,
            item_name   TEXT NOT NULL,
            FOREIGN KEY (monster_id) REFERENCES Monsters(id)
        );
    )");

    // Global game stats (e.g. total monsters defeated)
    execute(R"(
        CREATE TABLE IF NOT EXISTS GameStats (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            stat_name   TEXT NOT NULL UNIQUE,
            value       INTEGER NOT NULL DEFAULT 0
        );
    )");

    // Insert default global stats if not present
    execute(R"(
        INSERT OR IGNORE INTO GameStats (stat_name, value)
        VALUES ('total_monsters_defeated', 0);
    )");

    // Per player monster usage
    execute(R"(
        CREATE TABLE IF NOT EXISTS PlayerMonsterStats (
            id             INTEGER PRIMARY KEY AUTOINCREMENT,
            character_name TEXT NOT NULL,
            monster_name   TEXT NOT NULL,
            times_used     INTEGER NOT NULL DEFAULT 0,
            UNIQUE(character_name, monster_name)
        );
    )");

    // Per player item usage and kills
    execute(R"(
        CREATE TABLE IF NOT EXISTS PlayerItemStats (
            id                 INTEGER PRIMARY KEY AUTOINCREMENT,
            character_name     TEXT NOT NULL,
            item_name          TEXT NOT NULL,
            times_used         INTEGER NOT NULL DEFAULT 0,
            monsters_defeated  INTEGER NOT NULL DEFAULT 0,
            UNIQUE(character_name, item_name)
        );
    )");
}

void Database::saveCharacter(const Character& character) {
    // Delete existing save for this character name first
    std::string deleteSql = "DELETE FROM Characters WHERE name = '" + character.getName() + "';";
    execute(deleteSql);

    // Insert character
    std::string insertChar = "INSERT INTO Characters (name) VALUES ('" + character.getName() + "');";
    execute(insertChar);

    // Get the new character id
    int charId = (int)sqlite3_last_insert_rowid(db);

    // Insert each monster
    for (const auto& monster : character.getMonsters()) {
        std::string insertMonster =
            "INSERT INTO Monsters (character_id, name, health, attackPower) VALUES (" +
            std::to_string(charId) + ", '" +
            monster.getName() + "', " +
            std::to_string(monster.getHealth()) + ", " +
            std::to_string(monster.getAttackPower()) + ");";
        execute(insertMonster);

        int monsterId = (int)sqlite3_last_insert_rowid(db);

        // Insert each item on the monster
        for (const auto& item : monster.getItems()) {
            std::string insertItem =
                "INSERT INTO MonsterItems (monster_id, item_name) VALUES (" +
                std::to_string(monsterId) + ", '" + item.getName() + "');";
            execute(insertItem);
        }
    }

    std::cout << "Character " << character.getName() << " saved!" << std::endl;
}

std::vector<std::pair<int, std::string>> Database::listCharacters() {
    std::vector<std::pair<int, std::string>> characters;

    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name FROM Characters;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        characters.push_back({id, name});
    }
    sqlite3_finalize(stmt);
    return characters;
}

Character Database::loadCharacter(int characterId) {
    // Load character name
    sqlite3_stmt* stmt;
    std::string sql = "SELECT name FROM Characters WHERE id = " + std::to_string(characterId) + ";";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Character not found");
    }
    std::string charName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);

    Character character(charName);

    // Load monsters
    sql = "SELECT id, name, health, attackPower FROM Monsters WHERE character_id = " + std::to_string(characterId) + ";";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int monsterId   = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int health       = sqlite3_column_int(stmt, 2);
        int attackPower  = sqlite3_column_int(stmt, 3);

        Monster monster(name, health, attackPower);

        // Load items for this monster
        sqlite3_stmt* itemStmt;
        std::string itemSql = "SELECT item_name FROM MonsterItems WHERE monster_id = " + std::to_string(monsterId) + ";";
        if (sqlite3_prepare_v2(db, itemSql.c_str(), -1, &itemStmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(itemStmt) == SQLITE_ROW) {
                std::string itemName = reinterpret_cast<const char*>(sqlite3_column_text(itemStmt, 0));
                // Find matching item from item list
                for (const auto& item : Item::getItemList()) {
                    if (item.getName() == itemName) {
                        monster.addItem(item);
                        break;
                    }
                }
            }
            sqlite3_finalize(itemStmt);
        }

        character.addMonster(monster);
    }
    sqlite3_finalize(stmt);

    std::cout << "Character " << charName << " loaded!" << std::endl;
    return character;
}

void Database::incrementMonstersDefeated(int amount) {
    std::string sql =
        "UPDATE GameStats SET value = value + " + std::to_string(amount) +
        " WHERE stat_name = 'total_monsters_defeated';";
    execute(sql);
}

void Database::incrementItemUsed(const std::string& characterName, const std::string& itemName) {
    std::string sql =
        "INSERT INTO PlayerItemStats (character_name, item_name, times_used, monsters_defeated) "
        "VALUES ('" + characterName + "', '" + itemName + "', 1, 0) "
        "ON CONFLICT(character_name, item_name) DO UPDATE SET times_used = times_used + 1;";
    execute(sql);
}

void Database::incrementMonsterUsed(const std::string& characterName, const std::string& monsterName) {
    std::string sql =
        "INSERT INTO PlayerMonsterStats (character_name, monster_name, times_used) "
        "VALUES ('" + characterName + "', '" + monsterName + "', 1) "
        "ON CONFLICT(character_name, monster_name) DO UPDATE SET times_used = times_used + 1;";
    execute(sql);
}

void Database::incrementItemKill(const std::string& characterName, const std::string& itemName) {
    std::string sql =
        "INSERT INTO PlayerItemStats (character_name, item_name, times_used, monsters_defeated) "
        "VALUES ('" + characterName + "', '" + itemName + "', 0, 1) "
        "ON CONFLICT(character_name, item_name) DO UPDATE SET monsters_defeated = monsters_defeated + 1;";
    execute(sql);
}

int Database::getTotalMonstersDefeated() {
    sqlite3_stmt* stmt;
    std::string sql = "SELECT value FROM GameStats WHERE stat_name = 'total_monsters_defeated';";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    int total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return total;
}

std::string Database::getMostUsedItem(const std::string& characterName) {
    sqlite3_stmt* stmt;
    std::string sql =
        "SELECT item_name FROM PlayerItemStats WHERE character_name = '" + characterName +
        "' ORDER BY times_used DESC LIMIT 1;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    std::string result = "None";
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return result;
}

std::string Database::getMostUsedMonster(const std::string& characterName) {
    sqlite3_stmt* stmt;
    std::string sql =
        "SELECT monster_name FROM PlayerMonsterStats WHERE character_name = '" + characterName +
        "' ORDER BY times_used DESC LIMIT 1;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    std::string result = "None";
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return result;
}

std::string Database::getMostItemKills(const std::string& characterName) {
    sqlite3_stmt* stmt;
    std::string sql =
        "SELECT item_name FROM PlayerItemStats WHERE character_name = '" + characterName +
        "' ORDER BY monsters_defeated DESC LIMIT 1;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    std::string result = "None";
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return result;
}