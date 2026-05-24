#include "Boss.h"

#include "../data/MonsterTable.h"

namespace
{
MonsterInfo MakeBossInfo(int level)
{
    MonsterInfo info = GetMonsterTable().at(MonsterType::Boss);

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

Boss::Boss(int level)
    : Monster(MakeBossInfo(level))
{
}
