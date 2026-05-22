#pragma once
#include "IItem.h"

#include <vector>
//#include <memory> // 스마트 포인터 사용시

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
    void AddItem(IItem* item);
    void UseItem(int index, Character* character);
    
    vector<IItem*> GetItems() { return items; }

};