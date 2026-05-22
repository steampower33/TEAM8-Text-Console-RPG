#include "AttackBoost.h"
#include "../Character.h" // 위치 중요

string AttackBoost::GetName() const
{
    return name;
}

// 공격력 + 10: 해당 전투에만 효과 발휘
void AttackBoost::Use(Character* character)
{
    character->SetAttackPower(character->GetAttackPower() + attackTempIncrease);
    
    // 확인용
    cout << GetName() << " is used.\n";
}

