#include "Inventory.h"
#include  "IItem.h"


using namespace std;


Inventory::Inventory()
{
}

// 순회 하면서 지우기
Inventory::~Inventory()
{
    for (IItem* item : items)
    {
        delete item;
    }
    items.clear();
}

//void Inventory::AddItem(unique_ptr<IItem> item)
void Inventory::AddItem(IItem* item)
{
    items.push_back(item);
}


void Inventory::UseItem(int index, Character* character)
{
    if (index >= 0 && index < items.size())
    {
        items[index]->Use(character);

        //
        items.erase(items.begin() + index); // 사용후 제거 (메모리 해제는 시스템 종료시)
    }
}

