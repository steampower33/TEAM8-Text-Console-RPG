#pragma once
#include "../Character/Character.h"
#include "../Monster/Monster.h"

class BattleManager
{
public:
    void Battle(Character& player);
    bool BattleLoop(Character& player, unique_ptr<Monster>& monster);
    //bool IsBattleEnd(Character& player, Monster& monster);
protected:
    //Monster monster;
    
};
