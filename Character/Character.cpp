#include <iostream>
#include "Character.h"


Character* Character::Instance = nullptr;

Character* Character::GetInstance(std::string name) // 이름 받기
{
    if (Instance == nullptr)
    {
        Instance = new Character(name);
    }
    return Instance;
}

Character::Character(std::string name) // 기본 정보
{
    Name = name;
    Level = 1;
    Health = 200;
    MaxHealth = 200;
    Attack = 30;
    Experience = 0;
    Gold = 0;
}

Character::~Character() // 소멸자
{
    for (Item* item : Inventory)
    {
        delete item;
    }
    Inventory.clear();
}

void Character::DisplayStatus() // 스탯 보기
{
    
}

void Character::LevelUp() // 레벨 업
{
    
}

void Character::UseItem(int index) // 아이템 사용
{
    
}