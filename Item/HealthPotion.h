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
    }
    
    std::string GetName() const override;
    void Use(Character* character) override;
    
   

};
