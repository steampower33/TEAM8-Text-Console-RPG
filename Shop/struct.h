#pragma once
#include <string>
#include <unordered_map>
#include "../Item/IItem.h"

enum class ShopItems
{
    HEALTH_POTION,
    ATTACK_BOOST,
};


struct ShopItemInfo
{
    std::string name;
    int gold;
};

  static const std::unordered_map<ShopItems, ShopItemInfo> ShopItemTable =
  {
      {ShopItems::HEALTH_POTION, {HEALTH_POTION, 200}},
      {ShopItems::ATTACK_BOOST, {HEALTH_POTION, 100}},
  };

