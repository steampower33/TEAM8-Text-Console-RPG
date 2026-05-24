#pragma once
#include "../../Item/IItem.h"
#include "../data/MonsterStructs.h"

class MonsterItem : public IItem
{
public:
    MonsterItem(DevItem& item)
    {
        name = item.name;
        gold = item.gold;
    }

    std::string GetName() const override
    {
        return name;
    }

    void Use(Character* character) override
    {
    }
};
