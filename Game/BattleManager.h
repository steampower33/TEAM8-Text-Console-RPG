#pragma once

#include "GameManager.h"
#include "../Character/Character.h"
#include "../Monster/Monster.h"







class UIManager;
class Character;
class BattleManager
{
public:
    Phase BeforeBattle(Character& player);
    BattleResult Battle(Character& player);
    BattleResult BattleLoop(Character& player, Monster& monster);
    void PlayerWin(Character& player, Monster& monster);
    void GameVictory(Monster& monster);
    void MonsterWin(Character& player);
    void UseRandomItem(Character& player);
    void WaitForEnter();
    void Line();
    
protected:
    //Monster monster;
    
};
