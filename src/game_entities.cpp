#include "game_entities.h"

// --------------------------
// Tower stats
// --------------------------
const TowerStats BASIC_TOWER{ 1.f, 300.f, 25, 150.f };
const TowerStats SNIPER_TOWER{ 0.5f, 500.f, 50, 300.f };
const TowerStats CANNON_TOWER{ 0.8f, 200.f, 75, 120.f };

// --------------------------
// Enemy stats
// --------------------------
const EnemyStats FAST_ENEMY{ 50, 100.f, 0.f };    // health, speed, armour
const EnemyStats TANK_ENEMY{ 200, 40.f, 0.3f };
const EnemyStats NORMAL_ENEMY{ 100, 60.f, 0.1f };

// --------------------------
// Example waves
// --------------------------
EnemyWave wave1{ { FAST_ENEMY, NORMAL_ENEMY, NORMAL_ENEMY } };
EnemyWave wave2{ { TANK_ENEMY, NORMAL_ENEMY, FAST_ENEMY } };