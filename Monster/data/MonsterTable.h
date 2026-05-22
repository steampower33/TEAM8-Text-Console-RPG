#pragma once

#include <unordered_map>
#include "MonsterStructs.h"

inline const std::unordered_map<MonsterType, MonsterInfo>& GetMonsterTable()
{
    static const std::unordered_map<MonsterType, MonsterInfo> table = {
        {MonsterType::Goblin, {{"Goblin", 50, 10, 2, 2}, {10, {"고블린 몽둥이", 32}}}},
        {MonsterType::Orc, {{"Orc", 80, 15, 10, 0}, {20, {"오크 몽둥이", 60}}}},
        {MonsterType::Troll, {{"Troll", 120, 20, 5, 5}, {50, {"트롤 몽둥이", 50}}}},
        {MonsterType::Slime, {{"Slime", 30, 5, 0, 0}, {5, {"슬라임진액", 10}}}},
    };

    return table;
}
