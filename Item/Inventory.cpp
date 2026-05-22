#include "Inventory.h"
#include  "IItem.h"


using namespace std;


Inventory::Inventory()
{
}

/*
// uniq 라 뺌
// 순회 하면서 지우기
Inventory::~Inventory()
{
    for (IItem* item : items)
    {
        delete item;
    }
    items.clear();
}


void Inventory::AddItem(IItem* item)
{
    items.push_back(item);
}
*/


void Inventory::AddItem(unique_ptr<IItem> item)
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

vector<IItem*> Inventory::GetItems() const
{
    vector<IItem*> Items;
    Items.reserve(items.size());

    for (const auto& item : items)
    {
        Items.push_back(item.get()); 
    }

    return Items;
}

