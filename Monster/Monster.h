#pragma once
#include <string>
#include <unordered_map>
#include <array>
struct MonsterStats {
    std::string name;
    int baseHealth;
    int healthPerLevel;
    int baseAttack;
    int attackPerLevel;
};

enum class MonsterType {
    Goblin,
    Orc,
    Troll,
    Slime, 
    Length,
    
};
const std::array<MonsterStats, static_cast<size_t>(MonsterType::Length)> MonsterTable = {{
    { "Goblin", 50, 10, 5, 2 },
    { "Orc",    80, 15, 8, 3 },
    { "Troll", 120, 20, 12, 4 },
    { "Slime",  30,  5, 3, 1 }
}};
//
// const std::unordered_map<MonsterType, MonsterStats> MonsterTable = {
//     {
//         MonsterType::Goblin,
//         { "Goblin", 50, 10, 5, 2 }
//     },
//     {
//         MonsterType::Orc,
//         { "Orc", 80, 15, 8, 3 }
//     },
//     {
//         MonsterType::Troll,
//         { "Troll", 120, 20, 12, 4 }
//     },
//     {
//         MonsterType::Slime,
//         { "Slime", 30, 5, 3, 1 }
//     }
// };



class Monster {
public:
    virtual ~Monster() = default;
    virtual std::string getName() const = 0;
    virtual int getHealth() const = 0;
    virtual int getAttack() const = 0;
    virtual void takeDamage(int damage) = 0;
    void take_damage();
    void Get_damage();
    
};

