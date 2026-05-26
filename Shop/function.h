
#pragma once

#include "struct.h"

class Character;

bool BuyItem(ShopItems item, Character* player);
bool SellItem(int index, Character* player);