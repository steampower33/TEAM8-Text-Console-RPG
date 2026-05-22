#pragma once

#include <memory>

#include "../Monster.h"

std::unique_ptr<Monster> CreateRandomMonster(int level);
std::unique_ptr<Monster> CreateMonster(MonsterType type, int level);
