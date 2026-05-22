#include "HealthPotion.h"
#include "../Character.h" // 위치 중요

string HealthPotion::GetName() const
{
    return name;
}

void HealthPotion::Use(Character* character)
{
    character->SetHealthPoint(character->GetHealthPoint() + healthRestore);
    
    // 확인용
    cout << GetName() << " is used.\n";
}
