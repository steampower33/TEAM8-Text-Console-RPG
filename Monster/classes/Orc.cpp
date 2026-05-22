#include "Orc.h"

#include "../data/MonsterTable.h"

namespace
{
MonsterInfo MakeOrcInfo(int level)
{
    MonsterInfo info = GetMonsterTable().at(MonsterType::Orc);

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

Orc::Orc(int level)
    : Monster(MakeOrcInfo(level))
{
}
