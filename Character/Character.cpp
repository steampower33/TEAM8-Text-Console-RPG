#include <iostream>
#include <iomanip> // DisplayStatus 쪽에서 setw 안쓰면 지워도 됨
#include "Character.h"
#include "../UI/UIManager.h"
#include "../Sound/SoundManager.h"

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
    
}

void Character::DisplayStatus() // 스탯 보기
{
    std::cout << "╔═════════════════════════╗" << std::endl;
    std::cout << "║      [ 캐릭터 정보 ]      ║" << std::endl;
    std::cout << "╠═════════════════════════╣" << std::endl;
    std::cout << "║  이름   │ " << std::left << std::setw(14) << Name        << "║" << std::endl;
    std::cout << "║  레벨   │ " << std::left << std::setw(14) << Level       << "║" << std::endl;
    std::cout << "║  체력   │ " << std::left << std::setw(14) << std::to_string(Health) + " / " + std::to_string(MaxHealth) << "║" << std::endl;
    std::cout << "║  공격력 │ " << std::left << std::setw(14) << Attack      << "║" << std::endl;
    std::cout << "║  경험치 │ " << std::left << std::setw(14) << Experience  << "║" << std::endl;
    std::cout << "║  골드   │ " << std::left << std::setw(14) << Gold        << "║" << std::endl;
    std::cout << "╚═════════════════════════╝" << std::endl;
}

void Character::LevelUp() // 레벨 업
{
    if (Level >= 10)
    {
        ui.PrintLog(Color::RED + "[최대 레벨] 최대 레벨에 도달했습니다!" + Color::RESET);
        return;
    }

    if (Experience >= 100)
    {
        Experience -= 100;
        Level++;
        MaxHealth += Level * 20;
        Attack += Level * 5;
        Health = MaxHealth;

        ui.PrintLog(Color::YELLOW + "[레벨업] 현재 레벨: " + std::to_string(Level) + Color::RESET);
        ui.PrintLog(Color::YELLOW + "[레벨업] 능력치가 상승되었습니다!" + Color::RESET);
        SoundManager::GetInstance()->PlaySFX("Assets/Sound/8_bit_chime_positive.wav", "LevelUP");
    }
}

void Character::UseItem(int index) // 아이템 사용
{
    CharacterInventory.UseItem(index, this);    // Inventory -> CharacterInventory
}