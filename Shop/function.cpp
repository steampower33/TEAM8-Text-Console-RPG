
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
    auto items = player->CharacterInventory.GetItems();

    if (index < 0 || index >= items.size())
        return false;

    IItem* item = items[index];
    if (item->GetName() == HEALTH_POTION)
    {
        player->Gold += ShopItemTable.at(ShopItems::HEALTH_POTION).gold;
        item->count--;
        if (item->count <= 0)
        {
            player->CharacterInventory.RemoveItem(index);
        }
        return true; 
    }
    if (item->GetName() == ATTACK_BOOST)
    {
        player->Gold += ShopItemTable.at(ShopItems::ATTACK_BOOST).gold;
        item->count--;
        if (item->count <= 0)
        {
            player->CharacterInventory.RemoveItem(index);
        }
        return true;
    }
    return false;

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
