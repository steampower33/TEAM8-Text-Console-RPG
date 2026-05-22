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
    ~Inventory() = default; // unique_ptr 이니깐 따로 메모리 해제 안함.
    
    //void AddItem(IItem* item);
    
    void AddItem(std::unique_ptr<IItem> item);
    void UseItem(int index, Character* character);
    
    //vector<IItem*> GetItems() { return items; }
    
    std::vector<IItem*> GetItems() const;

};