#include "Create.h"

#include "../classes/Goblin.h"
#include "../classes/Orc.h"
#include "../classes/Slime.h"
#include "../classes/Troll.h"
#include "../classes/Boss.h"
#include "common.h"

std::unique_ptr<Monster> CreateRandomMonster(int level)
{
    
    // static std::random_device rd; // AI 쪼아용 
    // static std::mt19937 gen(rd());
    //
    // std::uniform_int_distribution<int> dist(
    //     0,
    //     static_cast<int>(MonsterType::RandomLength) - 1
    // );
 
    int RandMonster = GenRand(0,static_cast<int>(MonsterType::RandomLength) - 1);
    MonsterType randomType = static_cast<MonsterType>(RandMonster);

    return CreateMonster(randomType, level);
}

std::unique_ptr<Monster> CreateMonster(MonsterType type, int level)
{
    switch (type)
    {
    case MonsterType::Goblin:
        return std::make_unique<Goblin>(level);
    case MonsterType::Orc:
        return std::make_unique<Orc>(level);
    case MonsterType::Troll:
        return std::make_unique<Troll>(level);
    case MonsterType::Slime:
        return std::make_unique<Slime>(level);
    case MonsterType::RandomLength:
        throw std::invalid_argument("value is RandomLength");
    }
    throw std::invalid_argument("Invalid MonsterType");
}



std::unique_ptr<Monster> CreateBoss(int level)
{
    return std::make_unique<Boss>(level);
}



// 몬스터 소환 .
