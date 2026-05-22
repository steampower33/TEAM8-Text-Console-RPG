#include <iostream>

#include "BattleManager.h"
#include "../Monster/utils/Create.h"
#include "../Monster/data/MonsterStructs.h"

using namespace std;

// Monster,Character 클래스에 hp <= 0 일경우를 리턴하는 bool함수 IsDead() 추가 요망
// Character클래스에 몬스터클래스 참조해서 TakeDamage()함수 필요
//Monster,Character 클래스에 공격력 Getter 필요

//각 턴의 전투로직
void BattleManager::Battle(Character& player)
{
    
    unique_ptr<Monster> monster = CreateRandomMonster(player.Level);

    // while (!IsBattleEnd(player, monster))
    // {
    //     BattleLoof(player, monster);
    // }
    bool BattleResult = false;
    
    while (BattleResult == false)
    {
        BattleResult = BattleLoop(player, monster);
    }
    
}

bool BattleManager::BattleLoop(Character& player, unique_ptr<Monster>& monster)
{
    bool IsMonsterDead = monster->TakeDamageWithIsDead(player.Attack); //플레이어 공격
    player.TakeDamage(monster->GetStatus().ATK); //몬스터 공격
    
    if (player.IsDead == true || IsMonsterDead == true) 
        return true;
    
    return false;
        
}


// bool BattleManager::IsBattleEnd(Character& player, Monster& monster)
// {
// }