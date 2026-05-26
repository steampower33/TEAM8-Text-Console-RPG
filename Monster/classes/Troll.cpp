#include "Troll.h"

#include "../data/MonsterTable.h"
#include "../utils/common.h"

namespace
{
MonsterInfo MakeTrollInfo(int level)
{
    MonsterInfo info = GetMonsterTable().at(MonsterType::Troll);

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

Troll::Troll(int level)
    : Monster(MakeTrollInfo(level))
{
}
