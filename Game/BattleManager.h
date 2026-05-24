#pragma once
#include "GameManager.h"
#include "../Character/Character.h"
#include "../Monster/Monster.h"

class BattleManager
{
public:
    void Battle(Character& player);
    bool BattleLoop(Character& player, std::unique_ptr<Monster>& monster);
    //bool IsBattleEnd(Character& player, Monster& monster);
protected:
    //Monster monster;
    
};
