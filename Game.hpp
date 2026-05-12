#pragma once
#include "submarine.hpp"
#include "coralbunker.hpp"
#include "seacreature.hpp"
#include "bluewhale.hpp"
#include "powerup.hpp"
#include "scoremanager.hpp"

class Game { // Manages all game state and logic
public:
    // Constructor initializes game state and loads resources
    Game();
    ~Game();
    void Draw();
    void Update();
    void HandleInput();
    void InitGame();
    void Reset();

    // --- Read by main.cpp ---
    // These are public so main.cpp can read them for HUD display and menu logic
    bool run;
    bool paused;
    int  lives;
    int  score;
    int  wave;
    std::string playerName;
    Music music;
    ScoreManager scoreManager;

    // Exposed so main.cpp can query power-up state for HUD
    // and check creatures.empty() for the wave-clear banner
    Submarine                submarine;
    std::vector<SeaCreature> creatures;

private:
    // --- Wave system ---

    // Loads the next wave of sea creatures, increasing difficulty each time
    void LoadNextWave();

    bool   waveClearPending;
    double waveClearTimer;
    static constexpr double WAVE_CLEAR_DURATION = 2.0;

    // --- Helpers ---

    // Deletes torpedoes that are off-screen or have hit something
    void DeleteInactiveTorpedoes();
    void DeleteInactivePowerUps();

    // Creates the initial set of bunkers at the start of each game
    std::vector<CoralBunker> CreateBunkers();
    std::vector<SeaCreature> CreateCreatures();

    // Moves all creatures
    void MoveCreatures();
    void MoveCreaturesDown(int distance);
    void CreatureFireTorpedo();
    void CheckForCollisions();
    void GameOver();

    // --- Game objects ---

    // The main game objects are stored here
    std::vector<CoralBunker> bunkers;
    std::vector<Torpedo>     enemyTorpedoes;
    std::vector<PowerUp>     powerUps;
    BlueWhale blueWhale;



    int    creaturesDirection; // 1 = moving right, -1 = moving left
    float  creatureSpeedMultiplier;

    // Base interval between enemy shots, reduced each wave to increase difficulty
    constexpr static float BASE_ENEMY_FIRE_INTERVAL = 0.35f;

    // Time of last enemy shot fired, used to enforce firing interval
    float  enemyFireInterval;
    double timeLastEnemyFired;

    float  whaleSpawnInterval;
    double timeLastWhaleSpawn;

    Sound explosionSound;
};