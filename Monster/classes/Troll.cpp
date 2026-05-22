#include "Troll.h"

#include "../data/MonsterTable.h"

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

    info.Stats.HP += level;
    info.Stats.ATK += level;
    info.Stats.DEF += level;

    return info;
}
}

Troll::Troll(int level)
    : Monster(MakeTrollInfo(level))
{
}
