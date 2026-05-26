#include "Inventory.h"
#include  "IItem.h"


Inventory::Inventory() = default;


void Inventory::AddItem(std::unique_ptr<IItem> item)
{
    
    for (const auto& existing : items)
    {
        if (existing->GetName() == item->GetName()) 
        {
            existing->count++;   
            return;              
        }
    }
    
    
    items.push_back(std::move(item));
    
}


void Inventory::UseItem(int index, Character* character)
{
    
    if (index >= 0 && index < items.size())
    {
        items[index]->Use(character);
        
        items[index]->count--;          

        if (items[index]->count <= 0)   
        {
            items.erase(items.begin() + index);
        }
        
    }   

}

void Inventory::RemoveItem(int index)
{
    if (index >= 0 && index < items.size())
    {
        items[index]->count--;          

        if (items[index]->count <= 0)   
        {
            items.erase(items.begin() + index);
        }
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

