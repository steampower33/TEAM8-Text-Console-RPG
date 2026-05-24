#pragma once
#include "GameManager.h"
#include "../Character/Character.h"
#include "../Monster/Monster.h"

class UIManager;

class BattleManager
{
public:
    void BeforeBattle(Character& player);
    void Battle(Character& player);
    bool BattleLoop(Character& player, Monster& monster);
    void PlayerWin(Character& player, Monster& monster);
    void MonsterWin(Character& player);
    void UseRandomItem(Character& player);
protected:
    //Monster monster;
    
};
