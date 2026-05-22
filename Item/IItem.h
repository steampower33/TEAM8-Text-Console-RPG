#pragma once

#include <string>
#include <iostream>


/*
 // 이게 깔끔 하겠지만 번거로우니 우선 스킵
namespace ItemNames 
{
    const string HEALTH_POTION = "HealthPotion";
    const string ATTACK_BOOST = "AttackBoost";
}*/

class Character;

// namespace 나 GlobalVariable 로 설정하는게 깔끔하겠지만
// 우선 그냥 이렇게 전역으로 설정
const std::string HEALTH_POTION = "HealthPotion";
const std::string ATTACK_BOOST = "AttackBoost";

class IItem
{
protected:
    std::string name;  
    int gold; // 추후 상점 시스템에 필요
    
public:

    virtual ~IItem() {}
    
    virtual std::string GetName() const = 0; 
    virtual void Use(Character* character) = 0;
    
    virtual int GetGold() const { return gold; } // 추후
};
