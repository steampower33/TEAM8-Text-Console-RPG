#pragma once

#include <string>
#include <unordered_map>

struct MonsterStats
{
    std::string Name;
    int BaseHP;
    int BaseATK;
    int BaseDEF;
    int BaseAVD;
};

struct MonsterDrop
{
    std::string DropItem;;
};

struct MonsterReward
{
    int Exp;
    MonsterDrop DropItems;
};


enum class MonsterType
{
    Goblin,
    Orc,
    Troll,
    Slime,
    Length,
};

inline const std::unordered_map<MonsterType, MonsterStats>& GetMonsterTable()
{
    static const std::unordered_map<MonsterType, MonsterStats> table = {
        {MonsterType::Goblin, {"Goblin", 50, 10, 2, 2,}},
        {MonsterType::Orc, {"Orc", 80, 15, 10, 0}},
        {MonsterType::Troll, {"Troll", 120, 20, 5, 5}},
        {MonsterType::Slime, {"Slime", 30, 5, 0, 0}},
    };
    return table;
}


inline const MonsterStats& GetMonsterStats(MonsterType type)
{
    return GetMonsterTable().at(type);
}


class Monster
{
public:
    virtual ~Monster() = default;
    virtual std::string GetName() const = 0;
    virtual int GetStatus() const = 0;
    virtual void TakeDamage(int damage) = 0;
    virtual MonsterReward GetReward() = 0;
};

// std::unique_ptr<Monster> CreateMonster(MonsterType type)
// {
//     switch (type)
//     {
//     case MonsterType::Goblin:
//         return std::make_unique<MonsterType::Goblin>();
//     case MonsterType::Orc:
//         return std::make_unique<MonsterType::Orc>();
//     case MonsterType::Troll:
//         return std::make_unique<MonsterType::Troll>();
//     case MonsterType::Slime:
//         return std::make_unique<Slime>();
//     default:
//         return nullptr;
//     }
// }