#pragma once


enum class GameStateMachine
{
    Menu,
    Battle,
    Shop,
    GameEnd
};

class BattleManager;
class Character;


class GameManager
{
    
private:
    static GameManager* instance;
    GameManager(Character* player);

    GameStateMachine curentGameState;
    BattleManager* battleManager;
    Character* player;
    
public:
    static GameManager& GetInstance(Character* player);
    
    void StartGame();
    void UpdateGameState();

    
    
};

