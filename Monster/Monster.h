#pragma once

#include <algorithm>
#include <string>

#include "data/MonsterStructs.h"

class Monster
{
protected:
    MonsterInfo Info;

public:
    explicit Monster(const MonsterInfo& info)
        : Info(info)
    {
    }

    virtual ~Monster() = default;

    // std::string GetName() const
    // {
    //     return Info.Stats.Name;
    // } Get Status 으로 통합.

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
