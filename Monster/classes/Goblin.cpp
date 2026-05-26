#include "Goblin.h"

#include "../data/MonsterTable.h"
#include "../utils/common.h"

namespace
{
MonsterInfo MakeGoblinInfo(int level)
{
    MonsterInfo info = GetMonsterTable().at(MonsterType::Goblin);
    if (level <= 0)
    {
        info.Stats.Name += " 0";
        return info;
    }

    info.Stats.HP = (GenRand(20, 30) * level);
    info.Stats.ATK = (GenRand(5, 10) * level);
    info.Stats.DEF += level;

    return info;
}
}

Goblin::Goblin(int level)
    : Monster(MakeGoblinInfo(level))
{
}
