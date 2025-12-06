#include "game_entities.h"

// --------------------------
// Tower stats
// --------------------------
const TowerStats BASIC_TOWER{ 2.f, 6.f, 25, 300.f };
const TowerStats SNIPER_TOWER{ 2.8f, 2.f, 100, 600.f };
const TowerStats CANNON_TOWER{ 1.4f, 4.f, 75, 180.f };
const TowerStats LASER_TOWER{ 2.f, 600.f, 0.2, 2000.f };

// --------------------------
// Enemy stats
// --------------------------
const EnemyStats FAST_ENEMY{ 100, 80.f, 0.f, 1.2f };    // health, speed, armour, money
const EnemyStats TANK_ENEMY{ 280, 40.f, 0.3f, 1.3f };
const EnemyStats NORMAL_ENEMY{ 160, 60.f, 0.1f, 1.f };

// --------------------------
// Example waves
// --------------------------
EnemyWave wave1{ { FAST_ENEMY, NORMAL_ENEMY, NORMAL_ENEMY } };
EnemyWave wave2{ { TANK_ENEMY, NORMAL_ENEMY, FAST_ENEMY } };