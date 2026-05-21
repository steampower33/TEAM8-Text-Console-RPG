#pragma once

#include <string>
#include <vector>
#include "Item.h"

class Character
{
private:
    static Character* Instance;
    
    std::string Name;
    int Level;
    int Health;
    int MaxHealth;
    int Attack;
    int Experience;
    int Gold;
    std::vector<Item*> Inventory;
    
    Character(std::sring name);
    
public:
    ~Character();
    
    static Character* GetInstance(std::string name = "");
    void DisplayStatus();
    void LevelUp
    void UseItem(int index)
};