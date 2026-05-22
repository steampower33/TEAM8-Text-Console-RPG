#pragma once
#include "IItem.h"

#include <vector>
#include <memory>

using namespace std;

class IItem;


class Inventory
{
private:
    //vector<unique_ptr<IItem>> items;    
    vector<IItem*> items;   
public:
    Inventory();
    ~Inventory();
    //void AddItem(unique_ptr<IItem> item);
    void AddItem(IItem *item);
    void UseItem(int index, Character* character);
    void DisplayInventory();
};
