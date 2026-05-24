#pragma once

#include <string>

struct DevItem
{
    std::string name;
    int gold;
};

struct MonsterStats
{
    std::string Name;
    int HP;
    int ATK;
    int DEF;
    int AVD;
};

struct MonsterReward
{
    int Exp;
    DevItem Item;
};

struct MonsterInfo
{
    MonsterStats Stats;
    MonsterReward Reward;
};

enum class MonsterType
{
    Goblin,
    Orc,
    Troll,
    Slime,
    Boss,
    Count,
    
};
