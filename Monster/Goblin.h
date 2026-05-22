#pragma once
#include "Monster.h"


class Goblin : public Monster
{
    std::string name;
    int HP;
    int Atk;
    int Def;
    int Avd;

public:
    Goblin();

    std::string GetName() const override;
    int GetStatus() const override;
    void TakeDamage(int damage) override;
    MonsterReward GetReward() override;
};