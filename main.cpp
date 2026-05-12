#include "game.hpp"
#include <iostream>


//constructor and destructor
Game::Game()
    : scoreManager("highscore.txt")
{
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
    InitGame();
}

Game::~Game()
{
    SeaCreature::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

// ---------------------------------------------------------------------------
// Update
// ---------------------------------------------------------------------------

void Game::Update()
{
    if (!run) return;

    // --- wave-clear pause ---
    if (waveClearPending) {
        waveClearTimer -= GetFrameTime();
        if (waveClearTimer <= 0.0) {
            waveClearPending = false;
            LoadNextWave();
        }
        return;
    }

    double currentTime = GetTime();

    // Spawn blue whale periodically
    if (currentTime - timeLastWhaleSpawn > whaleSpawnInterval) {
        blueWhale.Spawn();
        timeLastWhaleSpawn = GetTime();
        whaleSpawnInterval = GetRandomValue(10, 20);
    }

    // Update submarine (ticks power-up timers)
    submarine.Update();

    for (auto& torpedo : submarine.torpedoes) {
        torpedo.Update();
    }

    //update falling power-ups
    for (auto& pu : powerUps) {
        pu.Update();
    }

    MoveCreatures();
    CreatureFireTorpedo();

    for (auto& torpedo : enemyTorpedoes) {
        torpedo.Update();
    }

    DeleteInactiveTorpedoes();
    DeleteInactivePowerUps();

    blueWhale.Update();
    CheckForCollisions();

    //check if all creatures are gone -> start wave-clear pause
    if (creatures.empty() && !waveClearPending) {
        waveClearPending = true;
        waveClearTimer = WAVE_CLEAR_DURATION;
    }
}

// ---------------------------------------------------------------------------
// Draw
// ---------------------------------------------------------------------------

void Game::Draw()
{
    submarine.Draw();

    for (auto& torpedo : submarine.torpedoes) {
        torpedo.Draw();
    }

    for (auto& bunker : bunkers) {
        bunker.Draw();
    }

    for (auto& creature : creatures) {
        creature.Draw();
    }

    for (auto& torpedo : enemyTorpedoes) {
        torpedo.Draw();
    }

    for (auto& pu : powerUps) {
        pu.Draw();
    }

    blueWhale.Draw();
}

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------

void Game::HandleInput()
{
    if (!run || waveClearPending || paused) return;

    if (IsKeyDown(KEY_LEFT))       submarine.MoveLeft();
    else if (IsKeyDown(KEY_RIGHT)) submarine.MoveRight();
    else if (IsKeyDown(KEY_SPACE)) submarine.FireTorpedo();
}

// ---------------------------------------------------------------------------
//  Wave system
// ---------------------------------------------------------------------------

void Game::LoadNextWave()
{
    wave++;

    creatureSpeedMultiplier += 0.15f;
    if (creatureSpeedMultiplier > 2.5f) creatureSpeedMultiplier = 2.5f;

    enemyFireInterval -= 0.03f;
    if (enemyFireInterval < 0.12f) enemyFireInterval = 0.12f;

    // Respawn blue whale more frequently as waves progress
    creatures = CreateCreatures();
    creaturesDirection = 1;
    enemyTorpedoes.clear();
    powerUps.clear();
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void Game::DeleteInactiveTorpedoes()
{
    for (auto it = submarine.torpedoes.begin(); it != submarine.torpedoes.end();) {
        it = it->active ? ++it : submarine.torpedoes.erase(it);
    }
    for (auto it = enemyTorpedoes.begin(); it != enemyTorpedoes.end();) {
        it = it->active ? ++it : enemyTorpedoes.erase(it);
    }
}

void Game::DeleteInactivePowerUps()
{
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        it = it->active ? ++it : powerUps.erase(it);
    }
}

// Creates the initial set of bunkers at the start of each game, spaced evenly across the screen
std::vector<CoralBunker> Game::CreateBunkers()
{
    std::vector<CoralBunker> result;
    int   bunkerWidth = CoralBunker::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * bunkerWidth)) / 5.0f;

    for (int i = 0; i < 4; i++) {
        float offsetX = (i + 1) * gap + i * bunkerWidth;
        result.push_back(CoralBunker({ offsetX, float(GetScreenHeight() - 200) }));
    }
    return result;
}


// Creates the initial set of sea creatures for a new wave, positioned in a grid near the top of the screen
std::vector<SeaCreature> Game::CreateCreatures()
{
    std::vector<SeaCreature> result;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 11; col++) {
            int type;
            if (row == 0)      type = 3;
            else if (row == 1) type = 2;
            else if (row == 2) type = 4;
            else               type = 1;
            float x = 75 + col * 55;
            float y = 110 + row * 55;
            result.push_back(SeaCreature(type, { x, y }));
        }
    }
    return result;
}

// Moves all creatures horizontally, changing direction and moving down when they hit the screen edge
void Game::MoveCreatures()
{
    int step = (int)(creatureSpeedMultiplier);
    if (step < 1) step = 1;

    for (auto& creature : creatures) {
        Vector2 pos = creature.GetPosition();
        if (pos.x + creature.creatureImages[creature.type - 1].width > GetScreenWidth() - 25) {
            creaturesDirection = -1;
            MoveCreaturesDown(4);
        }
        if (pos.x < 25) {
            creaturesDirection = 1;
            MoveCreaturesDown(4);
        }
        creature.Move(creaturesDirection * step);
    }
}

// Moves all creatures down by a certain distance (called when they hit the screen edge)
void Game::MoveCreaturesDown(int distance)
{
    for (auto& creature : creatures) {
        creature.MoveDown(distance);
    }
}

// Each creature has a chance to fire a torpedo at regular
void Game::CreatureFireTorpedo()
{
    double currentTime = GetTime();
    if (currentTime - timeLastEnemyFired >= enemyFireInterval && !creatures.empty()) {
        int randomIndex = 0;
        if (!creatures.empty()) {
            randomIndex = GetRandomValue(0, (int)creatures.size() - 1);
        }

        // Fire a torpedo from the selected creature
        SeaCreature& c = creatures[randomIndex];
        Vector2 cPos = c.GetPosition();
        enemyTorpedoes.push_back(Torpedo(
            { cPos.x + c.creatureImages[c.type - 1].width / 2.0f,
             cPos.y + c.creatureImages[c.type - 1].height },
            6
        ));
        timeLastEnemyFired = GetTime();
    }
}

// ---------------------------------------------------------------------------
// Collision detection
// ---------------------------------------------------------------------------

void Game::CheckForCollisions()
{
    for (auto& torpedo : submarine.torpedoes) {
        if (!torpedo.active) continue;


        // Check collision with sea creatures
        for (auto it = creatures.begin(); it != creatures.end();) {
            if (CheckCollisionRecs(it->GetBounds(), torpedo.GetBounds())) {
                PlaySound(explosionSound);

                int pts = (it->type == 1) ? 100 : (it->type == 2) ? 200 : (it->type == 4) ? 150 : 300;
                score += pts;
                scoreManager.SaveScore(score);


                // 25% chance to drop a power-up when a creature is destroyed
                if (GetRandomValue(0, 3) == 0) {
                    int r = GetRandomValue(0, 2);
                    PowerUp::Type puType = (r == 0) ? PowerUp::DOUBLE_TORPEDO
                        : (r == 1) ? PowerUp::SHIELD
                        : PowerUp::EXTRA_LIFE;
                    powerUps.push_back(PowerUp(it->GetPosition(), puType));
                }

                it = creatures.erase(it);
                torpedo.active = false;
            }
            else {
                ++it;
            }
        }


        // Check collision with bunkers
        for (auto& bunker : bunkers) {
            for (auto it = bunker.blocks.begin(); it != bunker.blocks.end();) {
                if (CheckCollisionRecs(it->GetBounds(), torpedo.GetBounds())) {
                    it = bunker.blocks.erase(it);
                    torpedo.active = false;
                }
                else {
                    ++it;
                }
            }
        }

        // Check collision with blue whale
        if (CheckCollisionRecs(blueWhale.GetBounds(), torpedo.GetBounds())) {
            blueWhale.alive = false;
            torpedo.active = false;
            score += 150;
            scoreManager.SaveScore(score);
            PlaySound(explosionSound);
        }
    }

    for (auto& torpedo : enemyTorpedoes) { // Check if any enemy torpedo hits the submarine
        if (!torpedo.active) continue;

        if (CheckCollisionRecs(torpedo.GetBounds(), submarine.GetBounds())) {
            torpedo.active = false;

            if (submarine.shieldActive) {
                submarine.shieldActive = false;
            }
            else {
                lives--;
                if (lives <= 0) GameOver();
            }
        }

        for (auto& bunker : bunkers) { // Enemy torpedoes can also hit bunkers, but they don't destroy the torpedo (it keeps going)
            for (auto it = bunker.blocks.begin(); it != bunker.blocks.end();) {
                if (CheckCollisionRecs(it->GetBounds(), torpedo.GetBounds())) {
                    it = bunker.blocks.erase(it);
                    torpedo.active = false;
                }
                else {
                    ++it;
                }
            }
        }
    }

    for (auto& creature : creatures) { // Check if any creature collides with the submarine (game over)
        for (auto& bunker : bunkers) {
            for (auto it = bunker.blocks.begin(); it != bunker.blocks.end();) {
                if (CheckCollisionRecs(it->GetBounds(), creature.GetBounds())) {
                    it = bunker.blocks.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
        if (CheckCollisionRecs(creature.GetBounds(), submarine.GetBounds())) {
            GameOver();
        }
    }

    for (auto& pu : powerUps) { // Check if submarine collects any power-ups
        if (!pu.active) continue;
        if (CheckCollisionRecs(pu.GetBounds(), submarine.GetBounds())) {
            pu.ApplyTo(submarine, lives);
        }
    }
}

// ---------------------------------------------------------------------------
// Game state
// ---------------------------------------------------------------------------

void Game::GameOver() // Ends the game and saves score to file and database
{
    run = false;
    paused = false;
    scoreManager.SaveScore(score);
    scoreManager.SaveToDB(playerName, score, wave);
}

void Game::InitGame() // Initializes game state for a new game
{
    paused = false;
    wave = 1;
    lives = 3;
    score = 0;
    run = true;
    creaturesDirection = 1;
    creatureSpeedMultiplier = 1.0f;
    enemyFireInterval = BASE_ENEMY_FIRE_INTERVAL;
    timeLastEnemyFired = 0.0;
    timeLastWhaleSpawn = 0.0;
    waveClearPending = false;
    waveClearTimer = 0.0;
    whaleSpawnInterval = GetRandomValue(10, 20);

    bunkers = CreateBunkers();
    creatures = CreateCreatures();
}

void Game::Reset() // Resets game state to start a new game
{
    submarine.Reset();
    creatures.clear();
    enemyTorpedoes.clear();
    bunkers.clear();
    powerUps.clear();
    blueWhale.alive = false;
}