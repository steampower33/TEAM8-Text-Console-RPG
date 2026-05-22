#pragma once

#include <string>
#include "../Item/Inventory.h"

class Character
{
public:
    ~Character();
    
    static Character* Instance;
    
    Character(std::string name);
    
    static Character* GetInstance(std::string name = "");
    void DisplayStatus();
    void LevelUp();
    void UseItem(int index);
    
    // 스탯 관련 변수들 public으로 옮김
    std::string Name;
    int Level;
    int Health;             // Health는 SetHealth로만 건드리기
    int MaxHealth;
    int Attack;
    int Experience;
    int Gold;
    Inventory CharacterInventory;           // 안 헷갈리게 타입명이랑 다르게 설정 바꿈

    // Setter
    void SetHealth(int health)          // MaxHealth보다 Health가 커지지 않게 설정, 0보다 아래로 내려가지 않게 설정 -> 포션이랑 죽는거에도 적용 가능하겠지..
	{
        if (health > MaxHealth) Health = MaxHealth;
        else if (health < 0) Health = 0;
        else Health = health;
	}

};