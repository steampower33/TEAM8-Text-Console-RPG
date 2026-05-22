#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() {}

GameManager& GameManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new GameManager();
    }
    return *instance;
}



void GameManager::StartGame()
{
    
}


void GameManager::StartBattleLoop()
{
    
}