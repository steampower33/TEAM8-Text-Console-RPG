#pragma once

#include <string>
#include <vector>
#include "../Item/Inventory.h"

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
    Inventory Inventory;
    
    Character(std::string name);
    
public:
    ~Character();
    
    static Character* GetInstance(std::string name = "");
    void DisplayStatus();
    void LevelUp();
    void UseItem(int index);
    
    // Getter
    int GetHealth() const { return Health; }
    int GetMaxHealth() const { return MaxHealth; }
    int GetAttack() const { return Attack; }

    // Setter
    void SetHealth(int health) { Health = health; }
    void SetAttack(int attack) { Attack = attack; }
};