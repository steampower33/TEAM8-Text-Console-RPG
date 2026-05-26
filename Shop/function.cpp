
#include "function.h"

#include "../Character/Character.h"
#include "../Item/HealthPotion.h"
#include "../Item/AttackBoost.h"

namespace
{
bool BuyHealthPotion(Character* player)
{
    if (player->Gold < ShopItemTable.at(ShopItems::HEALTH_POTION).gold)
        return false;

    player->Gold -= ShopItemTable.at(ShopItems::HEALTH_POTION).gold;
    player->CharacterInventory.AddItem(std::make_unique<HealthPotion>());
    return true;
}

bool BuyAttackBoost(Character* player)
{
    if (player->Gold < ShopItemTable.at(ShopItems::ATTACK_BOOST).gold)
        return false;

    player->Gold -= ShopItemTable.at(ShopItems::ATTACK_BOOST).gold;
    player->CharacterInventory.AddItem(std::make_unique<AttackBoost>());
    return true;
}
}

bool SellItem(int index, Character* player) 
{
    IItem* ItemName = player->CharacterInventory.GetItems()[index];
    if (ItemName->GetName() == HEALTH_POTION)
    {
    player->Gold += ShopItemTable.at(ShopItems::HEALTH_POTION).gold;
        player->CharacterInventory.(index);
        return true; 
    }
    if (ItemName->GetName() == HEALTH_POTION)
    {player->Gold += ShopItemTable.at(ShopItems::HEALTH_POTION).gold;
        return true;
    }
    

}


bool BuyItem(ShopItems item, Character* player)
{
    switch (item)
    {
    case ShopItems::HEALTH_POTION:
        return BuyHealthPotion(player);
    case ShopItems::ATTACK_BOOST:
        return BuyAttackBoost(player);
    }

    return false;
}
