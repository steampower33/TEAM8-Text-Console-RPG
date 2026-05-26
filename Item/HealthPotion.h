#pragma once
#include "IItem.h"


class HealthPotion : public IItem
{
private:
    int healthRestore; 
    
    
public:
    HealthPotion()
    {
        name = HEALTH_POTION;
        healthRestore = 50;
        gold = 200;
    }
    
    std::string GetName() const override;
    void Use(Character* character) override;
    
};
