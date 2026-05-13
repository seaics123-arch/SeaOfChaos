#include "uimanager.hpp"


// Constructor initializes the UIManager with a font and a sub-image for the HUD
UIManager::UIManager(Font font, Texture2D subImage)
{
    this->font = font;
    this->subImage = subImage;
}


// Draws outlined text
void UIManager::DrawTextOutlined(const char* text, Vector2 pos, float size, float spacing, Color color)
{
    Color outline = { 0, 50, 100, 255 };
    int offset = 2;

    DrawTextEx(font, text, { pos.x - (float)offset, pos.y }, size, spacing, outline);
    DrawTextEx(font, text, { pos.x + (float)offset, pos.y }, size, spacing, outline);
    DrawTextEx(font, text, { pos.x, pos.y - (float)offset }, size, spacing, outline);
    DrawTextEx(font, text, { pos.x, pos.y + (float)offset }, size, spacing, outline);

    DrawTextEx(font, text, pos, size, spacing, color);
}


// Helper function to format numbers with leading zeros for consistent display in the HUD
std::string UIManager::FormatWithLeadingZeros(int number, int width)
{
    std::string text = std::to_string(number);
    int zeros = width - (int)text.length();
    return std::string(zeros > 0 ? zeros : 0, '0') + text;
}

// Draws the main HUD during gameplay, showing score, high score, lives, wave number, and active power-ups
void UIManager::DrawHUD(int score, int highscore, int lives, int wave,
    bool shieldActive, bool doubleActive, bool running)
{
    DrawRectangleRoundedLinesEx({ 10, 10, 780, 780 }, 0.18f, 20, 2, yellow);
    DrawLineEx({ 25, 730 }, { 775, 730 }, 3, yellow);

    float x = 50.0f; // Draw a submarine icon for each remaining life
    for (int i = 0; i < lives; i++) {
        DrawTextureV(subImage, { x, 745 }, WHITE);
        x += 50;
    }


    // Display the current wave number if the game is running, or "GAME OVER" if not
    if (running) {
        std::string waveText = "WAVE " + FormatWithLeadingZeros(wave, 2);
        DrawTextOutlined(waveText.c_str(), { 550, 740 }, 34, 2, yellow);
    }
    else {
        DrawTextOutlined("GAME OVER", { 550, 740 }, 34, 2, yellow);
    }


    // Display the score and high score at the top of the screen
    DrawTextOutlined("SCORE", { 50, 15 }, 34, 2, yellow);
    DrawTextOutlined(FormatWithLeadingZeros(score, 5).c_str(), { 50, 40 }, 34, 2, yellow);

    DrawTextOutlined("HIGH-SCORE", { 550, 15 }, 34, 2, yellow);
    DrawTextOutlined(FormatWithLeadingZeros(highscore, 5).c_str(), { 635, 40 }, 34, 2, yellow);

    float px = 340.0f;
    if (shieldActive) {
        DrawTextOutlined("[S]", { px, 740 }, 28, 2, { 120, 255, 120, 255 });
        px += 60;
    }
    if (doubleActive) {
        DrawTextOutlined("[2]", { px, 740 }, 28, 2, { 80, 200, 255, 255 });
    }
}


// Draws the main menu with the game title, instructions, and controls
void UIManager::DrawMainMenu()
{
    DrawRectangleRoundedLinesEx({ 10, 10, 780, 780 }, 0.18f, 20, 2, yellow);

    DrawTextOutlined("SEA OF CHAOS", { 195, 150 }, 64, 3, yellow);
    DrawTextOutlined("AN UNDERWATER ARCADE GAME", { 195, 228 }, 28, 2, yellow);
    DrawTextOutlined("PRESS ENTER TO START", { 210, 320 }, 34, 2, yellow);
    DrawTextOutlined("PRESS L FOR LEADERBOARD", { 210, 368 }, 34, 2, yellow);
    DrawTextOutlined("ARROW KEYS  -  MOVE", { 235, 430 }, 28, 2, yellow);
    DrawTextOutlined("SPACE       -  FIRE", { 235, 468 }, 28, 2, yellow);
    DrawTextOutlined("P           -  PAUSE", { 235, 506 }, 28, 2, yellow);
    DrawTextOutlined("POWER-UPS:", { 235, 550 }, 24, 2, yellow);
    DrawTextOutlined("[2] DOUBLE TORPEDO", { 235, 578 }, 22, 2, { 80,  200, 255, 255 });
    DrawTextOutlined("[S] SHIELD  (absorbs one hit)", { 235, 602 }, 22, 2, { 120, 255, 120, 255 });
    DrawTextOutlined("[+] EXTRA LIFE", { 235, 626 }, 22, 2, { 255, 100, 150, 255 });
}


// Draws the game over screen with the final score, high score, wave reached, and options to restart, view the leaderboard, or return to the main menu
void UIManager::DrawGameOver(int score, int highscore, int wave)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 160 });
    DrawTextOutlined("GAME OVER", { 220, 200 }, 64, 3, yellow);
    DrawTextOutlined(("SCORE:  " + FormatWithLeadingZeros(score, 5)).c_str(), { 245, 295 }, 34, 2, yellow);
    DrawTextOutlined(("BEST:   " + FormatWithLeadingZeros(highscore, 5)).c_str(), { 245, 335 }, 34, 2, yellow);
    DrawTextOutlined(("WAVE:   " + FormatWithLeadingZeros(wave, 2)).c_str(), { 245, 375 }, 34, 2, yellow);
    DrawTextOutlined("ENTER  -  RESTART", { 210, 440 }, 30, 2, yellow);
    DrawTextOutlined("L      -  LEADERBOARD", { 210, 480 }, 30, 2, yellow);
    DrawTextOutlined("M      -  MAIN MENU", { 210, 520 }, 30, 2, yellow);
}

// Draws the pause menu overlay with options to resume, return to the main menu, or quit the game
void UIManager::DrawPause()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 160 });
    DrawTextOutlined("PAUSED", { 290, 240 }, 64, 3, yellow);
    DrawTextOutlined("P  -  RESUME", { 245, 360 }, 34, 2, yellow);
    DrawTextOutlined("M  -  MAIN MENU", { 245, 410 }, 34, 2, yellow);
    DrawTextOutlined("Q  -  QUIT", { 245, 460 }, 34, 2, yellow);
}


// Draws the wave clear banner between waves, showing the completed wave and the next wave number
void UIManager::DrawWaveClear(int completedWave)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 130 });
    DrawTextOutlined(("WAVE " + FormatWithLeadingZeros(completedWave, 2) + " CLEAR!").c_str(), { 195, 330 }, 48, 3, yellow);
    DrawTextOutlined(("PREPARE FOR WAVE " + FormatWithLeadingZeros(completedWave + 1, 2)).c_str(), { 130, 395 }, 32, 2, yellow);
}


// Draws the leaderboard screen, showing the top scores, player names, wave reached, and options to return to the main menu or go back to the previous screen
void UIManager::DrawLeaderboard(const std::vector<LeaderboardEntry>& entries)
{
    ClearBackground(grey);
    DrawRectangleRoundedLinesEx({ 10, 10, 780, 780 }, 0.18f, 20, 2, yellow);
    DrawTextEx(font, "LEADERBOARD", { 230, 60 }, 52, 3, yellow);

    float y = 160.0f;
    int rank = 1;

    if (entries.empty()) {
        DrawTextEx(font, "NO SCORES YET!", { 280, 360 }, 32, 2, yellow);
    }
    else {
        for (const auto& e : entries) {
            std::string line = std::to_string(rank) + ".  " +
                e.playerName + "   " +
                FormatWithLeadingZeros(e.score, 5) +
                "   WAVE " + FormatWithLeadingZeros(e.wave, 2);
            DrawTextEx(font, line.c_str(), { 80, y }, 28, 2, WHITE);
            y += 50;
            rank++;
            if (rank > 10) break;
        }
    }

    DrawTextOutlined("R  -  RETURN", { 150, 720 }, 26, 2, yellow);
    DrawTextOutlined("M  -  MAIN MENU", { 450, 720 }, 26, 2, yellow);
}