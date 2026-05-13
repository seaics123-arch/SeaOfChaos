#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include "scoremanager.hpp"

class UIManager { // Handles all drawing of the user interface, including the HUD, menus, and leaderboard
public:
    UIManager(Font font, Texture2D subImage);

    void DrawHUD(int score, int highscore, int lives, int wave,
        bool shieldActive, bool doubleActive, bool running);


    // These are called by main.cpp when in the corresponding game states
    void DrawMainMenu();
    void DrawGameOver(int score, int highscore, int wave);
    void DrawWaveClear(int completedWave);
    void DrawPause();
    void DrawLeaderboard(const std::vector<LeaderboardEntry>& entries);
    void DrawTextOutlined(const char* text, Vector2 pos, float size, float spacing, Color color);



private:
    Font font;
    Texture2D subImage;
    std::string FormatWithLeadingZeros(int number, int width);

    Color yellow = { 243, 216, 63, 255 };
    Color grey = { 29,  29,  27, 255 };
};