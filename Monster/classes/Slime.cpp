#include "Slime.h"

#include "../data/MonsterTable.h"

namespace
{
MonsterInfo MakeSlimeInfo(int level)
{
    MonsterInfo info = GetMonsterTable().at(MonsterType::Slime);

    if (level <= 0)
    {
        // info.Stats.Name += " 0";
        return info;
    }

    info.Stats.HP += level;
    info.Stats.ATK += level;
    info.Stats.DEF += level;

    return info;
}
}

Slime::Slime(int level)
    : Monster(MakeSlimeInfo(level))
{
}
