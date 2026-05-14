#include <raylib.h>
#include "game.hpp"
#include "uimanager.hpp"

int main()
{
    // --- Initialization ---
    Color grey = { 29, 29, 27, 255 };
    int offset = 50;
    int windowWidth = 750;
    int windowHeight = 700;


    // Initialize game resources
    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Sea of Chaos");
    InitAudioDevice();

    Font      font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D subImage = LoadTexture("Graphics/submarine.png");
    Texture2D background = LoadTexture("Graphics/background.png");
    Texture2D menuBackground = LoadTexture("Graphics/menu_background.png");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    UIManager ui(font, subImage); //
    Game game;

    bool appRunning = true;

    while (!WindowShouldClose() && appRunning) {

        // ---------------------------------------------------------------
        // Main menu loop
        // ---------------------------------------------------------------
        bool inMenu = true;
        while (!WindowShouldClose() && inMenu) {

            // L key — show leaderboard from main menu
            if (IsKeyPressed(KEY_L)) {
                auto entries = game.scoreManager.GetTopScores(10);
                bool showingBoard = true;
                bool firstFrame = true;
                while (!WindowShouldClose() && showingBoard) {
                    if (!firstFrame) {
                        if (IsKeyPressed(KEY_R)) showingBoard = false; // R key — return to main menu
                        if (IsKeyPressed(KEY_M)) showingBoard = false; // M key — return to main menu and exit game loop
                    }
                    firstFrame = false;
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawTexturePro(
                        menuBackground,
                        { 0, 0, (float)menuBackground.width, (float)menuBackground.height },
                        { 0, 0, (float)(windowWidth + offset), (float)(windowHeight + 2 * offset) },
                        { 0, 0 }, 0.0f, WHITE
                    );
                    ui.DrawLeaderboard(entries);
                    EndDrawing(); // Draw the leaderboard while in the leaderboard loop
                }
            }

            if (IsKeyPressed(KEY_ENTER)) inMenu = false; // Enter key — start game

            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                menuBackground,
                { 0, 0, (float)menuBackground.width, (float)menuBackground.height },
                { 0, 0, (float)(windowWidth + offset), (float)(windowHeight + 2 * offset) },
                { 0, 0 }, 0.0f, WHITE
            );
            ui.DrawMainMenu();
            EndDrawing(); // Draw the main menu while in the main menu loop
        }

        // ---------------------------------------------------------------
        // Name entry loop
        // ---------------------------------------------------------------
        std::string playerName = ""; // Player name input
        bool nameDone = false;

        while (!WindowShouldClose() && !nameDone) { // Loop for entering player name before starting the game
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 125 && playerName.size() < 16)
                    playerName += (char)key;
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty())
                playerName.pop_back();
            if (IsKeyPressed(KEY_ENTER) && !playerName.empty())
                nameDone = true;

            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                menuBackground,
                { 0, 0, (float)menuBackground.width, (float)menuBackground.height },
                { 0, 0, (float)(windowWidth + offset), (float)(windowHeight + 2 * offset) },
                { 0, 0 }, 0.0f, WHITE
            );
            DrawTextEx(font, "ENTER YOUR NAME", { 220, 300 }, 40, 2, YELLOW);
            DrawTextEx(font, playerName.c_str(), { 220, 360 }, 36, 2, WHITE);
            DrawTextEx(font, "PRESS ENTER TO START", { 200, 430 }, 28, 2, YELLOW);
            EndDrawing();// Draw the name entry screen while in the name entry loop
        }

        game.Reset();
        game.InitGame();
        game.playerName = playerName;

        // ---------------------------------------------------------------
        // Game loop
        // ---------------------------------------------------------------
        bool inGame = true;
        while (!WindowShouldClose() && inGame) {
            UpdateMusicStream(game.music);

            // Pause toggle
            if (game.run && IsKeyPressed(KEY_P))
                game.paused = !game.paused;

            if (!game.paused) {
                game.HandleInput();
                game.Update();
            }

            bool skipDraw = false;

            // --- Pause menu ---
            if (game.paused) {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(background,
                    { 0, 0, (float)background.width, (float)background.height },
                    { 0, 0, (float)(windowWidth + offset), (float)(windowHeight + 2 * offset) },
                    { 0, 0 }, 0.0f, WHITE); // Draw the game in the background of the pause menu
                game.Draw();
                ui.DrawHUD(game.score, game.scoreManager.GetHighscore(),
                    game.lives, game.wave,
                    game.submarine.shieldActive,
                    game.submarine.doubleTorpedoTimer > 0.0,
                    game.run);
                ui.DrawPause();
                EndDrawing();


                if (IsKeyPressed(KEY_Q)) { appRunning = false; inGame = false; }
                if (IsKeyPressed(KEY_M)) { game.paused = false; inGame = false; }
                skipDraw = true; // Skip the normal draw since we already drew the pause menu
            }

            // --- Game over menu ---
            if (!game.run && !skipDraw) {
                if (IsKeyPressed(KEY_ENTER)) { // Enter key — restart game from game over screen
                    game.Reset();
                    game.InitGame();
                    game.playerName = playerName;
                }
                else if (IsKeyPressed(KEY_L)) { // L key — show leaderboard from game over screen
                    auto entries = game.scoreManager.GetTopScores(10);
                    bool showingBoard = true;
                    bool firstFrame = true;
                    while (!WindowShouldClose() && showingBoard) {
                        if (!firstFrame) {
                            if (IsKeyPressed(KEY_R)) showingBoard = false;
                            if (IsKeyPressed(KEY_M)) { showingBoard = false; inGame = false; }
                        }
                        firstFrame = false;
                        BeginDrawing();
                        ClearBackground(BLACK);
                        ui.DrawLeaderboard(entries);
                        EndDrawing();
                    }
                    skipDraw = true;
                }
                else if (IsKeyPressed(KEY_M)) { // M key — return to main menu from game over screen
                    inGame = false;
                    skipDraw = true;
                }
            }

            // --- Normal draw ---

            if (!skipDraw) {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(background,
                    { 0, 0, (float)background.width, (float)background.height },
                    { 0, 0, (float)(windowWidth + offset), (float)(windowHeight + 2 * offset) },
                    { 0, 0 }, 0.0f, WHITE); // Draw the background in the main game loop

                game.Draw();

                if (game.run && game.creatures.empty())
                    ui.DrawWaveClear(game.wave);

                if (!game.run)
                    ui.DrawGameOver(game.score, game.scoreManager.GetHighscore(), game.wave);

                ui.DrawHUD(game.score, game.scoreManager.GetHighscore(),
                    game.lives, game.wave,
                    game.submarine.shieldActive,
                    game.submarine.doubleTorpedoTimer > 0.0,
                    game.run);

                EndDrawing(); // Draw the main game screen while in the main game loop
            }
        }
    }


    // Cleanup resources
    UnloadFont(font);
    UnloadTexture(subImage);
    UnloadTexture(background);
    UnloadTexture(menuBackground);
    CloseWindow();
    CloseAudioDevice();

    return 0;
}