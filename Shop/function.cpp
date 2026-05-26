#include "function.h"

#include "../Character/Character.h"
#include "../Item/HealthPotion.h"
#include "../Item/AttackBoost.h"
#include "../Monster/data/MonsterTable.h"

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


int GetSellByItemName(const std::string& itemName)
{
    if (itemName == HEALTH_POTION)
        return ShopItemTable.at(ShopItems::HEALTH_POTION).gold;

    if (itemName == ATTACK_BOOST)
        return ShopItemTable.at(ShopItems::ATTACK_BOOST).gold;

    const auto& monsterTable = GetMonsterTable();

    for (const auto& [type, info] : monsterTable)
    {
        return info.Reward.Item.gold;
    }
    return 0;
}


}

bool SellItem(int index, Character* player)
{
    auto items = player->CharacterInventory.GetItems();
    if (index < 0 || index >= items.size())
        return false;

    IItem* item = items[index];
    if (item == nullptr)
        return false;
    std::string ItemName = item->GetName();
    int sellGold = GetSellByItemName(ItemName);
    if (sellGold < 0)
        return false;

    if (ItemName == HEALTH_POTION || ItemName == ATTACK_BOOST)
    {
        player->Gold += sellGold * 60 / 100;
    }
    else
    {
        player->Gold += sellGold;
    }
    player->CharacterInventory.RemoveItem(index);
    return true;
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