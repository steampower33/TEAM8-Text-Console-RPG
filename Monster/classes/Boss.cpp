#include "Boss.h"

#include "../data/MonsterTable.h"
#include "../utils/common.h"

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

    info.Stats.HP = (GenRand(30, 45) * level);
    info.Stats.ATK = (GenRand(7, 15) * level);
    info.Stats.DEF += level;

    return info;
}
}

Boss::Boss(int level)
    : Monster(MakeBossInfo(level))
{
}
