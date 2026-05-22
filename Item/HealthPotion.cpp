#include "HealthPotion.h"
#include "../Character/Character.h"

string HealthPotion::GetName() const
{
    return name;
}

void HealthPotion::Use(Character* character)
{
    character->SetHealth(character->Health + healthRestore);

}
