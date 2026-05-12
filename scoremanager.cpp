#include "scoremanager.hpp"
#include <fstream>
#include <iostream>

// Constructor initializes the score manager
ScoreManager::ScoreManager(const std::string& filename)
{
    this->filename = filename;
    highscore = ReadFromFile();
    db = nullptr;
    InitDB();
}

ScoreManager::~ScoreManager()
{
    if (db) sqlite3_close(db); // Clean up the database connection when the ScoreManager is destroyed
}


// Initializes the SQLite database and creates the leaderboard table if it doesn't exist
void ScoreManager::InitDB()
{
    int rc = sqlite3_open("seaofchaos.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
        return;
    }

    // Create the leaderboard table if it doesn't exist
    const char* sql =
        "CREATE TABLE IF NOT EXISTS leaderboard ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "score INTEGER NOT NULL,"
        "wave INTEGER NOT NULL,"
        "timestamp TEXT DEFAULT CURRENT_TIMESTAMP"
        ");";


    // Execute the SQL statement to create the table, and check for errors
    char* errMsg = nullptr;
    sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (errMsg) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Saves a player's score to the SQLite database with their name, score, wave, and timestamp
void ScoreManager::SaveToDB(const std::string& playerName, int score, int wave)
{
    if (!db) {
        std::cerr << "Database not initialized!" << std::endl;
        return;
    }

    const char* sql = "INSERT INTO leaderboard (name, score, wave) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt; // Prepared statement for inserting a new score


    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }


    // Bind the player name, score, and wave to the prepared statement parameters
    sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, score);
    sqlite3_bind_int(stmt, 3, wave);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);// Clean up the prepared statement after execution
}


// Retrieves the top scores from the SQLite database
std::vector<LeaderboardEntry> ScoreManager::GetTopScores(int limit)
{
    std::vector<LeaderboardEntry> entries;
    if (!db) return entries;

    const char* sql = "SELECT name, score, wave, timestamp FROM leaderboard ORDER BY score DESC LIMIT ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare select statement: " << sqlite3_errmsg(db) << std::endl;
        return entries;
    }

    sqlite3_bind_int(stmt, 1, limit);


    // Execute the query and populate the entries vector with the results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LeaderboardEntry entry;
        entry.playerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        entry.score = sqlite3_column_int(stmt, 1);
        entry.wave = sqlite3_column_int(stmt, 2);
        entry.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        entries.push_back(entry);
    }

    sqlite3_finalize(stmt);
    return entries;
}

// Saves the score to the file if it's a new high score, and updates the in-memory high score
void ScoreManager::SaveScore(int score)
{
    if (score > highscore) {
        highscore = score;
        WriteToFile(highscore);
    }
}

int ScoreManager::LoadHighscore()
{
    highscore = ReadFromFile();
    return highscore;
}

// Returns the current high score stored in memory
int ScoreManager::GetHighscore() const
{
    return highscore;
}

// Writes the high score to the file
void ScoreManager::WriteToFile(int value)
{
    std::ofstream file(filename);
    if (file.is_open()) {
        file << value;
        file.close();
    }
}


// Reads the high score from the file
int ScoreManager::ReadFromFile()
{
    int loaded = 0;
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> loaded;
        file.close();
    }
    return loaded;
}
