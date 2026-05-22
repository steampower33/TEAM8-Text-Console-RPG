#include "Goblin.h"

#include "../data/MonsterTable.h"

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

    info.Stats.HP += level;
    info.Stats.ATK += level;
    info.Stats.DEF += level;

    return info;
}
}

Goblin::Goblin(int level)
    : Monster(MakeGoblinInfo(level))
{
}
