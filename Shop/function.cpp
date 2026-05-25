
#include "function.h"

#include "../Character/Character.h"
#include "../Item/HealthPotion.h"
#include "../Item/AttackBoost.h"

namespace
{
bool BuyHealthPotion(Character* player)
{
    if (player->Gold < ShopItemTable.at(ShopItems::HealthPotion).gold)
        return false;

    player->Gold -= ShopItemTable.at(ShopItems::HealthPotion).gold;
    player->CharacterInventory.AddItem(std::make_unique<HealthPotion>());
    return true;
}

bool BuyAttackBoost(Character* player)
{
    if (player->Gold < ShopItemTable.at(ShopItems::AttackBoost).gold)
        return false;

    player->Gold -= ShopItemTable.at(ShopItems::AttackBoost).gold;
    player->CharacterInventory.AddItem(std::make_unique<AttackBoost>());
    return true;
}
}

bool BuyItem(ShopItems item, Character* player)
{
    switch (item)
    {
    case ShopItems::HealthPotion:
        return BuyHealthPotion(player);
    case ShopItems::AttackBoost:
        return BuyAttackBoost(player);
    }

    return false;
}

bool SellItem(ShopItems item, Character* player){ return false;}