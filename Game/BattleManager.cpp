#include <iostream>
#include <random>
#include <conio.h>

#include "BattleManager.h"
#include "../Monster/utils/Create.h"
#include "../Monster/data/MonsterStructs.h"
#include "../UI/UIManager.h"

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

// UIManager ui;
void BattleManager::WaitForEnter()
{
    int key;
    do {
        key = _getch();
    } while (key != 13);

    while (_kbhit()) _getch();
}

Phase BattleManager::BeforeBattle(Character& player)
{
    system("cls");
    ui.ShowMainFrame();
    ui.UpdateStat(&player);
    ui.UpdateInventory(&player.CharacterInventory);

    ui.UpdateScene();

    ui.PrintLog("\033[36m[탐험]\033[0m 미궁을 걷고 있습니다... [Enter] 전진 (ESC 종료)");

    int key = _getch();
    if (key == KEY_ESC)
    {
        return Phase::Escape;
    }
    if (key == KEY_ENTER)
        return Phase::Enter;

    // //ESC, Enter 외 다른 키입력에 대한 방어코드 필요함
    // return Phase::Continue; //등
}

//각 턴의 전투로직
BattleResult BattleManager::Battle(Character& player)
{
    unique_ptr<Monster> monster = nullptr;

    if (player.Level == 10)
        monster = CreateBoss();

    else if (player.Level < 10)
        monster = CreateRandomMonster(player.Level);

    MonsterStats MonsterStats = monster->GetStatus();

    system("cls");
    ui.ShowMainFrame();
    ui.UpdateStat(&player);
    ui.UpdateInventory(&player.CharacterInventory);
    
    ui.PrintLog("\033[31m[새로운 전투!]\033[0m");
    if (MonsterStats.Name == "웅애 CPP 어려웡")
    {
        ui.PrintLog("\033[31m[전투 발생!]\033[0m 조져따! 보스 몬스터 " + MonsterStats.Name + "이(가) 나타났다!");
    }
    else
    {
        ui.PrintLog("\033[31m[전투 발생!]\033[0m 야생의 " + MonsterStats.Name + "이(가) 나타났다!");
    }
    
    // isCombat = true 전달
    ui.UpdateScene(true, MonsterStats.Name);
    
    BattleResult BattleResult = BattleLoop(player, *monster); //승리시true 패배시false
    
    return BattleResult;
}

BattleResult BattleManager::BattleLoop(Character& player, Monster& monster)
{
    random_device rd;
    mt19937 RandomEngine(rd()); //선우님 좋은 거 알아오셨네
    uniform_int_distribution<int> FightOrUseItem(1, 100); //1~100 사이 정수 값

    string MonsterName = monster.GetStatus().Name;
    
    while (true)
    {
        int Result = FightOrUseItem(RandomEngine);
        
        WaitForEnter();
        ui.PrintLog("\033[33m[전투 대기]\033[0m [Enter]를 누르면 턴이 진행됩니다.");
        static int debugCount = 0;
        ui.PrintLog("[전투 대기] 호출 횟수: " + std::to_string(++debugCount));
                
        if (Result <= 50)
        {
            //플레이어 공격
            bool IsMonsterDead = monster.TakeDamageWithIsDead(player.Attack);
            ui.PrintLog(player.Name + "의 공격! " + MonsterName + "에게 피해를 입혔습니다. " + MonsterName + "의 잔여 HP = "+ std::to_string(monster.GetStatus().HP));
                    
            if (IsMonsterDead == true)
            {
                PlayerWin(player, monster);
            
                player.LevelUp();
                if (MonsterName == "웅애 CPP 어려웡")
                    return BattleResult::BossClear;
                
                return BattleResult::Win;
                
            }

            player.TakeDamage(monster.GetStatus().ATK); //몬스터 공격
            ui.PrintLog("\033[31m[반격]\033[0m " + MonsterName + "의 맹공! " + std::to_string(monster.GetStatus().ATK) + "의 피해!");
            ui.UpdateStat(&player);
            
            if (player.IsDead == true)
            {
                MonsterWin(player);
                return BattleResult::Fail;
            }
        
        }
        else
        { // 아이템이 없을 때 여기 들어오면 안댐

            UseRandomItem(player);
        }
        
    }
    //return fightResult;
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
    
    if (monster.GetStatus().Name == "웅애 CPP 어려웡")
    {
        // 골드 : 10~20 범위에서 랜덤 획득
        player.Gold += 1000000;
        player.CharacterInventory.AddItem(monster.MonsterDropItems());
        ui.PrintLog("\033[36m[아이템 획득]\033[0m ++" + monster.MonsterDropItems()->GetName() + "++ 을 획득했습니다.");
        // 아이템 : 30% 확률로 획득
        ui.PrintLog("\033[36m[전투 승리]\033[0m " + monster.GetStatus().Name + " 처치! (EXP +" + std::to_string(monster.GetReward().Exp) + " / Gold +" + std::to_string(GoldAmount) + ")");
        
        GameVictory(monster);
    }
    else
    {
        // 골드 : 10~20 범위에서 랜덤 획득
        player.Gold += GoldAmount;

        // 아이템 : 30% 확률로 획득
        if (ResultOfItemPersent <= 30)
        {
            player.CharacterInventory.AddItem(monster.MonsterDropItems());
            ui.PrintLog("\033[36m[아이템 획득]\033[0m ++" + monster.MonsterDropItems()->GetName() + "++ 을 획득했습니다.");
        }else
        {
            ui.PrintLog("\033[36m[아이템 획득 실패]\033[0m 몬스터의 사체를 뒤져보았지만 아이템을 발견하지 못했습니다.");
        }
    
        ui.PrintLog("\033[36m[전투 승리]\033[0m " + monster.GetStatus().Name + " 처치! (EXP +" + std::to_string(monster.GetReward().Exp) + " / Gold +" + std::to_string(GoldAmount) + ")");
    }
    
}

void BattleManager::MonsterWin(Character& player)
{
    ui.PrintLog("\033[31m[사망]\033[0m " + player.Name + "이(가) 쓰러졌습니다... 게임 오버!");
    ui.PrintLog("\033[31m[게임 종료]\033[0m [Enter]를 누르면 게임이 종료됩니다.");
}

void BattleManager::GameVictory(Monster& monster)
{
    ui.PrintLog("\033[36m[게임클리어]\033[0m 보스 몬스터 "+ monster.GetStatus().Name +"을 물리치고 던전을 탈출합니다. 게임 클리어!");
    ui.PrintLog("\033[36m[게임 종료]\033[0m [Enter]를 누르면 게임이 종료됩니다.");
}

void BattleManager::UseRandomItem(Character& player)
{
    random_device rd;
    mt19937 RandomEngine(rd()); //선우님 좋은 거 알아오셨네
    uniform_int_distribution<int> UseItemPersent(1, 100); //1~100 사이 정수 값

    int ResultOfUseItemPersent = UseItemPersent(RandomEngine);
    int itemCount = player.CharacterInventory.GetItems().size();

    if (ResultOfUseItemPersent <= 30 )
    {

        // 가지고 있는 아이템 중 랜덤으로 하나 선택
        std::uniform_int_distribution<int> itemDist(0, itemCount - 1);
        int randomIndex = itemDist(RandomEngine);

        vector<IItem*> items = player.CharacterInventory.GetItems(); //&쓰면 좋은데 vector<IItem*>& 타입으로 받아올수가 없네요
        string itemName = items[randomIndex]->GetName();
        if (itemName == "HealthPotion" || itemName == "AttackBoost")
        {
            
            player.UseItem(randomIndex);
            ui.UpdateStat(&player);
            ui.UpdateInventory(&player.CharacterInventory);
            ui.PrintLog("\033[32m[아이템]\033[0m " + player.Name + "이(가) 가방에서 " + itemName + "아이템을 사용했습니다!");
        }
        else
        {
            ui.PrintLog("\033[32m[아이템]\033[0m " + itemName + "아이템은 전투 중 사용할 수 없는 아이템입니다.");
        }
    }
    else if (ResultOfUseItemPersent > 50 && itemCount > 0)
    {
        ui.PrintLog("\033[32m[아이템]\033[0m 아이템을 사용하려 했으나 몬스터의 방해로 인해 사용하지 못하였습니다. ㅋㅋ");
    }
    else if (itemCount <= 0)
        ui.PrintLog("\033[32m[아이템]\033[0m 사용할 아이템이 없답니다? ㅋㅋ");
}