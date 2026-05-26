#include "GameManager.h"

#include "BattleManager.h"
#include "../Character/Character.h"
#include "../Item/AttackBoost.h"
#include "../Item/HealthPotion.h"
#include "../UI/UIManager.h"

//GameManager 싱글톤 생성 시작
GameManager* GameManager::instance = nullptr;

GameManager& GameManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new GameManager();
    }
    return *instance;
}

GameManager::GameManager()
    : curentGameState(GameStateMachine::Start)
      , battleManager(nullptr)
      , player(nullptr)
{
}

//GameManager 싱글톤 생성 끝


// //핸들러 함수 시작
// void GameManager::StarterHandler() //초기 캐릭터 생성
// {
//     
//     StartEndManager starterManager;
//     starterManager.Start();
//     player = Character::GetInstance();
//     curentGameState = GameStateMachine::Explore;
// }
//
// void GameManager::ExploreHandler() //몬스터 조우 전 (탐험중)
// {
//     Phase phase = battleManager->BeforeBattle(*player);
//
//     if (phase == Phase::Enter)
//         curentGameState = GameStateMachine::Battle;
//
//     else if (phase == Phase::Escape)
//         curentGameState = GameStateMachine::GameEnd;
// }
//
// void GameManager::BattleHandler() // 몬스터 조우 및 전투 승리 시 다시 탐험중
// {
//     bool BattleResult = battleManager->Battle(*player);
//
//     if (BattleResult)
//         curentGameState = GameStateMachine::Explore;
//
//     else
//         curentGameState = GameStateMachine::GameEnd;
// }
//
// void GameManager::GameEndHandler() // 패배 시 게임종료
// {
//     StartEndManager starterManager;
//     starterManager.End(*player);
// }
//
// //void ShopHandler(); shopManager 완성 시 사용
// // //핸들러 함수 끝
// void GameManager::Run()
// {
//     while (curentGameState != GameStateMachine::GameEnd)
//     {
//         switch (curentGameState)
//         {
//         case GameStateMachine::Start:
//             StarterHandler();
//             break;
//         case GameStateMachine::Explore:
//             ExploreHandler();
//             break;
//         case GameStateMachine::Battle:
//             BattleHandler();
//             break;
//         }
//     }
//     
//     GameEndHandler();
// }


void GameManager::Run()
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
        curentGameState = GameStateMachine::Explore;
    }
    else
    {
        curentGameState = GameStateMachine::GameEnd;
    }

    player = Character::GetInstance();
    battleManager = new BattleManager();

    while (curentGameState != GameStateMachine::GameEnd)
    {
        switch (curentGameState)
        {
        case GameStateMachine::Explore:
        {
            Phase phase = battleManager->BeforeBattle(*player);
            if (phase == Phase::Enter)
                curentGameState = GameStateMachine::Battle;
            else if (phase == Phase::Escape)
                curentGameState = GameStateMachine::GameEnd;
            break;
        }
        case GameStateMachine::Battle:
        {
            BattleResult BattleResult = battleManager->Battle(*player);
            if (BattleResult == BattleResult::Win  )
                curentGameState = GameStateMachine::Explore;
            else
                curentGameState = GameStateMachine::GameEnd;
            break;
        }
        }
    }
}