
#pragma once

#include "struct.h"

class Character;

bool BuyItem(ShopItems item, Character* player);
bool SellItem(ShopItems item, Character* player);