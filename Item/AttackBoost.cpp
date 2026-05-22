#include "AttackBoost.h"
#include "../Character/Character.h"

string AttackBoost::GetName() const
{
    return name;
}

// 공격력 + 10: 해당 전투에만 효과 발휘
void AttackBoost::Use(Character* character)
{
    //character->SetAttackPower(character->GetAttackPower() + attackTempIncrease);

}