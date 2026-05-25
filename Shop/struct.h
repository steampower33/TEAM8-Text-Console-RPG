#pragma once
#include <string>
#include <unordered_map>

enum class ShopItems
{
    HealthPotion,
    AttackBoost,
};


struct ShopItemInfo
{
    std::string name;
    int gold;
};

static const std::unordered_map<ShopItems, ShopItemInfo> ShopItemTable =
{
    {ShopItems::HealthPotion, {"HealthPotion", 200}},
    {ShopItems::AttackBoost, {"AttackBoost", 100}},
};

