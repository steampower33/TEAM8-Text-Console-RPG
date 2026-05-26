#include <algorithm>
#include <iostream>
#include <windows.h>

#include "Character/Character.h"
#include "Game/GameManager.h"
#include "Item/HealthPotion.h"
#include "Item/IItem.h"
#include "Monster/Monster.h"
#include "Monster/utils/Create.h"
#include "Shop/function.h"



int main() {
    Character* character = Character::GetInstance("test");

    character->CharacterInventory.AddItem(std::make_unique<HealthPotion>());
    character->CharacterInventory.AddItem(std::make_unique<HealthPotion>());
    character->CharacterInventory.AddItem(std::make_unique<HealthPotion>());
    character->CharacterInventory.AddItem(std::make_unique<HealthPotion>());

    bool result = SellItem(0, character);

    auto items = character->CharacterInventory.GetItems();

    for (IItem* item : items)
    {
        if (item != nullptr)
        {
            std::cout << item->GetName() << ":" << item->count << std::endl;
            std::cout << item->count<< std::endl; 
        }
    }

    // std::cout << "판매 결과: " << result << std::endl;
    std::cout << "끝" << std::endl;

    system("pause");
    return 0;
}
