#pragma once
#include "GameManager.h"
#include "../Character/Character.h"
#include "../Monster/Monster.h"

class UIManager;

class BattleManager
{
public:
    void BeforeBattle(Character& player, UIManager& ui);
    void Battle(Character& player, UIManager& ui);
    bool BattleLoop(Character& player, Monster& monster);
    void PlayerWin(Character& player, Monster& monster);
    void MonsterWin(Character& player);
protected:
    //Monster monster;
    
};
