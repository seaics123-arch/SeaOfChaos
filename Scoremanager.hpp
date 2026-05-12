#pragma once
#include <string>
#include <vector>
#include "sqlite3.h"


/// Manages high scores, including file I/O and SQLite database interactions
struct LeaderboardEntry { // Represents a single entry in the leaderboard
    std::string playerName;
    int score = 0;
    int wave = 0;
    std::string timestamp;
};



// ScoreManager handles saving/loading high scores to a text file and an SQLite database
class ScoreManager {
public:
    ScoreManager(const std::string& filename = "highscore.txt");
    ~ScoreManager();

    void SaveScore(int score);
    int LoadHighscore();
    int GetHighscore() const;

    // SQLite methods
    void SaveToDB(const std::string& playerName, int score, int wave);
    std::vector<LeaderboardEntry> GetTopScores(int limit = 10);

private:
    std::string filename;
    int highscore;
    sqlite3* db; // SQLite database connection


    // Helper methods for file I/O and database initialization
    void WriteToFile(int value);
    int ReadFromFile();
    void InitDB();
};