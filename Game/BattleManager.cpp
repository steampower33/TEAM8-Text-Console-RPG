#include <iostream>
#include <random>
#include <conio.h>
#include "BattleManager.h"



#include "../Monster/utils/Create.h"
#include "../Monster/data/MonsterStructs.h"
#include "../UI/UIManager.h"
#include "../UI/UIManager.cpp"
// //이거 파일에 넣어야함??
// #define KEY_UP    72
// #define KEY_DOWN  80
// #define KEY_LEFT  75
// #define KEY_RIGHT 77
// #define KEY_ENTER 13
// #define KEY_ESC   27

using namespace std;

// Monster,Character 클래스에 hp <= 0 일경우를 리턴하는 bool함수 IsDead() 추가 요망
// Character클래스에 몬스터클래스 참조해서 TakeDamage()함수 필요
//Monster,Character 클래스에 공격력 Getter 필요


// Notion 지문이 아이템 자동 사용이 맞는지 확인 필요
// #include <random>

// random_device rd;
// mt19937 gen(rd());
// uniform_int_distribution<int> chance30(1, 100);
// void BattleManager::BeforeBattle(Character& player)
// {
//     // 1) 플레이어의 아이템 자동 사용 (30% 확률)
//     int itemCount = player.CharacterInventory.GetItems().size();
//     if (chance30(gen) <= 30 && itemCount > 0) {
//                     
//         // 가지고 있는 아이템 중 랜덤으로 하나 선택
//         uniform_int_distribution<int> itemDist(0, itemCount - 1);
//         int randomIndex = itemDist(gen);
//                     
//         // 주의: 아이템이 '해당 전투만 공격력 +10'이라면, 
//         // AttackBoost 아이템 내부에서 character->Attack을 직접 올리지 마시고,
//         // tempAttackBuff += 10; 으로 처리되도록 구조를 맞춰야 영구 스탯 뻥튀기 버그가 안 납니다.
//         player.UseItem(randomIndex); 
//                     
//         ui.PrintLog("\033[32m[아이템]\033[0m " + player.Name + "이(가) 가방에서 아이템을 사용했습니다!");
//     }
// }

UIManager ui;

void BattleManager::BeforeBattle(Character& player, UIManager& ui)
{
    system("cls");
    ui.ShowMainFrame();
    ui.UpdateStat(&player);
    ui.UpdateInventory(&player.CharacterInventory);
            
    ui.UpdateScene(); 
            
    ui.PrintLog("\033[36m[탐험]\033[0m 미궁을 걷고 있습니다... [Enter] 전진 (ESC 종료)");
            
    int key = _getch();
    if (key == KEY_ESC) {
        return;
    }
    if (key != KEY_ENTER) 
        Battle(player, ui); 
}

//각 턴의 전투로직
void BattleManager::Battle(Character& player, UIManager& ui)
{
    

    bool BattleResult = false;
    unique_ptr<Monster> monster = CreateRandomMonster(player.Level);
    MonsterStats MonsterStats = monster->GetStatus();
    
    
    ui.PrintLog("\033[31m[새로운 전투!]\033[0m");
    ui.PrintLog("\033[31m[전투 발생!]\033[0m 야생의 " + MonsterStats.Name + "이(가) 나타났다!");
    
    
    while (BattleResult == false)
    {
        system("cls");
        ui.ShowMainFrame();
        ui.UpdateStat(&player);
        ui.UpdateInventory(&player.CharacterInventory);
                
        // isCombat = true 전달
        ui.UpdateScene(true, MonsterStats.Name);

        
        ui.PrintLog("\033[33m[전투 대기]\033[0m [Enter]를 누르면 턴이 진행됩니다.");
                
        int combatKey = _getch();
        if (combatKey != KEY_ENTER) continue;
        BattleResult = BattleLoop(player, *monster);
    }

}

bool BattleManager::BattleLoop(Character& player, Monster& monster)
{
    
    bool IsMonsterDead = monster.TakeDamageWithIsDead(player.Attack); //플레이어 공격
    if (IsMonsterDead == true)
    {
        PlayerWin(player, monster);
        return true;
    }

    player.TakeDamage(monster.GetStatus().ATK); //몬스터 공격
    if (player.IsDead == true)
    {
        return false;
    }

}

void BattleManager::PlayerWin(Character& player, Monster& monster)
{
    random_device rd;
    mt19937 RandomEngine(rd()); //선우님 좋은 거 알아오셨네

    uniform_int_distribution<int> RandomGold(10, 20); //10~20 사이 정수 값 
    uniform_int_distribution<int> ItemPersent(1, 100); //1~100 사이 정수 값

    int GoldAmount = RandomGold(RandomEngine); // 난수 그대로 사용함 (gold 10~20 사이로 획득)
    int ResultOfItemPersent = ItemPersent(RandomEngine);

    // 경험치 : (Notion)50 획득, 100 이상 획득 시 레벨업 -> (실제구현) 생성된 몬스터 별 차등 경험치 획득, 100 이상 획득 시 레벨업
    player.Experience += monster.GetReward().Exp;

    // 골드 : 10~20 범위에서 랜덤 획득
    player.Gold += GoldAmount;

    // 아이템 : 30% 확률로 획득
    if (ResultOfItemPersent <= 30)
    {
        auto reward = monster.GetReward();
        player.CharacterInventory.AddItem(
            make_unique<ConcreteItem>(reward)
        );
        unique_ptr<IItem> RewardItem;
        RewardItem->GetName() = monster.GetReward().Item.name;
        RewardItem->GetGold() = monster.GetReward().Item.gold;

        player.CharacterInventory.AddItem(
            (unique_ptr<IItem>)monster.GetReward().Item);

    }

}

void BattleManager::MonsterWin(Character& player)
{
    
    ui.PrintLog("\033[31m[사망]\033[0m " + player.Name + "이(가) 쓰러졌습니다... 게임 오버!");
                    
    ui.PrintLog("\033[31m[게임 종료]\033[0m [Enter]를 누르면 게임이 종료됩니다.");
}

// bool BattleManager::IsBattleEnd(Character& player, Monster& monster)
// {
// }