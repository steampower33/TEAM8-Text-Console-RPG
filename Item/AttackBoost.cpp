#include "AttackBoost.h"
#include "../Character/Character.h"

std::string AttackBoost::GetName() const
{
    return name;
}

// 공격력 + 10: 해당 전투에만 효과 발휘
void AttackBoost::Use(Character* character)
{
    
    //character->SetAttackPower(character->GetAttackPower() + attackTempIncrease);
    
    // 캐릭터 클래스에 set 아니고 그냥 public 이여서 이렇게 했습니다.
    character->Attack += attackTempIncrease;
}