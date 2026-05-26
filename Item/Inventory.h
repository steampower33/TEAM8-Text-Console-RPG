#pragma once
#include "IItem.h"

#include <vector>
#include <memory> 


class IItem;


class Inventory
{
private:
    std::vector<std::unique_ptr<IItem>> items;       

public:
    Inventory();
    ~Inventory() = default; 
    
    
    void AddItem(std::unique_ptr<IItem> item);
    void UseItem(int index, Character* character);
    
    void RemoveItem(int index);
    
    std::vector<IItem*> GetItems() const;

};