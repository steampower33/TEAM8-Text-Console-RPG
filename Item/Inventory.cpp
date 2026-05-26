#include "Inventory.h"
#include  "IItem.h"


Inventory::Inventory()
{
}


void Inventory::AddItem(std::unique_ptr<IItem> item)
{
    items.push_back(move(item));
}


void Inventory::UseItem(int index, Character* character)
{
    if (index >= 0 && index < items.size())
    {
        items[index]->Use(character);
        items.erase(items.begin() + index); 
    }
}

void Inventory::RemoveItem(int index)
{
    if (index >= 0 && index < items.size())
    {
        items.erase(items.begin() + index);
    }
}

std::vector<IItem*> Inventory::GetItems() const
{
    std::vector<IItem*> Items;
    Items.reserve(items.size());

    for (const auto& item : items)
    {
        Items.push_back(item.get()); 
    }

    return Items;
}

