#pragma once

#include <algorithm>
#include <random>
#include <string>
#include "data/MonsterStructs.h"




inline int Rand(int min, int max) {
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> dist(min, max);
return dist(gen);
}


class Monster
{
protected:
    MonsterInfo Info;
    
    /**TODO 
    * 레벨 스케일링 대응.
    * - [ ]  몬스터도 이름, 체력, 공격력을 가집니다.
    * - [ ]  몬스터의 스탯은 캐릭터 레벨에 비례해서 랜덤하게 설정합니다.
    * - [ ]  체력: (레벨 × 20) ~ (레벨 × 30) 
    * - [ ]  공격력: (레벨 × 5) ~ (레벨 × 10)
    * ***/
    void LevelScaling(int Level)
    {
        
        Info.Stats.HP = Rand(20,30);
        Info.Stats.ATK = Rand(5,10);
        Info.Stats.DEF = Rand(2,5);
        //AVD 쓸꺼면 따로 추가 
    }
    void LevelScalingWithBase(int Level )
    {
        Info.Stats.HP  += Rand(20,30);
        Info.Stats.ATK += Rand(5,10);
        Info.Stats.DEF += Rand(2,5);
    }  // 구조체 그대로  
    
public:
    explicit Monster(const MonsterInfo& info)
        : Info(info)
    {
    }

    virtual ~Monster() = default;

    MonsterStats GetStatus() const
    {
        return Info.Stats;
    }

    void TakeDamage(int damage)
    {
        int finalDamage = std::max(1, damage - Info.Stats.DEF);
        Info.Stats.HP = std::max(0, Info.Stats.HP - finalDamage);
    }
    bool TakeDamageWithIsDead(int damage)
    {
        TakeDamage(damage);
        if ( Info.Stats.HP <= 0)
        {
            return true;
        }      
        return false;
    }
    MonsterReward GetReward() const
    {
        return Info.Reward;
    } 
    
    
};
