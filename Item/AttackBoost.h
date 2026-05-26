#pragma once
#include "IItem.h"

class AttackBoost : public IItem
{
private:
    int attackTempIncrease; // 해당 전투에만 효과 발휘

public:
    AttackBoost()
    {
        name = ATTACK_BOOST;
        attackTempIncrease = 10;
        gold = 100;
    }


    std::string GetName() const override;
    void Use(Character* character) override;
};