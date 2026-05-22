#include <iostream>

#include "BattleManager.h"
#include "MonsterFactory.h"

using namespace std;

// Monster,Character 클래스에 hp <= 0 일경우를 리턴하는 bool함수 IsDead() 추가 요망
// Character클래스에 몬스터클래스 참조해서 TakeDamage()함수 필요
//Monster,Character 클래스에 공격력 Getter 필요

//각 턴의 전투로직
void BattleManager::Battle(Character& player)
{
    
    monster = MonsterFactory::CreateRandomMonster();

    while (!IsBattleEnd(player, monster))
    {
        BattleLoof(player, monster);
    }
}

void BattleManager::BattleLoof(Character& player, Monster& monster)
{
    int playerDamage = player.GetAttack();
    monster.TakeDamage(playerDamage);

    if (monster.IsDead()) return;

    // Monster 공격
    int monsterDamage = monster;
    player.TakeDamage(monsterDamage);
}


bool BattleManager::IsBattleEnd(Character& player, Monster& monster)
{

    if (player.IsDead() || monster.IsDead())
    {
        return true;
    }
    
    return false;
}