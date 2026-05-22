#pragma once
#include "IItem.h"

#include <vector>
#include <memory> 

using namespace std;

class IItem;


class Inventory
{
private:
    vector<unique_ptr<IItem>> items;       

public:
    Inventory();
    ~Inventory() = default; // unique_ptr 이니깐 따로 메모리 해제 안함.
    
    //void AddItem(IItem* item);
    
    void AddItem(unique_ptr<IItem> item);
    void UseItem(int index, Character* character);
    
    //vector<IItem*> GetItems() { return items; }
    
    vector<IItem*> GetItems() const;

};