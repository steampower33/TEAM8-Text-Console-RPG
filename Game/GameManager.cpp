#include "GameManager.h"

#include "BattleManager.h"
#include "../Character/Character.h"


GameManager* GameManager::instance = nullptr;

GameManager& GameManager::GetInstance(Character* player)
{
    if (instance == nullptr)
    {
        instance = new GameManager(player);
    }
    return *instance;
}

GameManager::GameManager(Character* player)
{
    this->player = player;
}



void GameManager::StartGame()
{
    while (true)
    {
        UpdateGameState();
    }
}

void GameManager::UpdateGameState()
{
    switch (curentGameState)
    {
    case GameStateMachine::Menu:
        // 시작 메뉴 처리
        break;

    case GameStateMachine::Exploring:
        // 이동 / 이벤트
        break;

    case GameStateMachine::Battle:
        battleManager->BattleLoop(player);
        currentState = GameStateMachine::Exploring;
        break;

    case GameStateMachine::Shop:
        shopManager.OpenShop(player);
        currentState = GameState::Exploring;
        break;

    case GameState::GameOver:
        // 종료 처리
        break;
    }
}


// void GameManager::StartBattleLoop()
// {
//     
// }