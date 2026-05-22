#include "Create.h"

#include <stdexcept>

#include "../classes/Goblin.h"
#include "../classes/Orc.h"
#include "../classes/Slime.h"
#include "../classes/Troll.h"

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
    case MonsterType::Count:
        throw std::invalid_argument("Invalid MonsterType");
    }

    throw std::invalid_argument("Invalid MonsterType");
}
