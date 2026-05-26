#include "Slime.h"

#include "../data/MonsterTable.h"
#include "../utils/common.h"

namespace
{
MonsterInfo MakeSlimeInfo(int level)
{
    MonsterInfo info = GetMonsterTable().at(MonsterType::Slime);

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

Slime::Slime(int level)
    : Monster(MakeSlimeInfo(level))
{
}
