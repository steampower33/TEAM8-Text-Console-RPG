#include "StartEndManager.h"
#include "../Item/AttackBoost.h"
#include "../Item/HealthPotion.h"
#include "../UI/UIManager.h"
#include "../Character/Character.h"

//UIManager ui;

void StartEndManager::Start()
{
    int titleResult = ui.GetTitleResult();
    
    if (titleResult == 0)
    {
        std::string name = ui.ShowCharacterGeneration();
        
        // 싱글톤으로 캐릭터 인스턴스 획득
        Character* player = Character::GetInstance(name);
    
        // 시작 아이템을 캐릭터 내부 인벤토리에 지급
        player->CharacterInventory.AddItem(std::make_unique<HealthPotion>());
        player->CharacterInventory.AddItem(std::make_unique<AttackBoost>());
    }
}

void StartEndManager::End(Character& player)
{
    //어떤식으로 들어갈지 몰라서 일단 클리어/엔드로 구분해놓았슴다
    
    if (player.Health > 0)
        ui.PrintLog("게임 클리어!");  // 승리 엔딩
    else
        ui.PrintLog("게임 오버 아ㅋㅋ");

    ui.PrintLog("\033[31m[게임 종료]\033[0m 최종 레벨: " + player.Level);
    
   
}