#pragma once


enum class GameStateMachine
{
    Start,
    Battle,
    Shop,
    GameEnd,
    Explore
};

class BattleManager;
class Character;
class StartEndManager;

class GameManager
{
    
private:
    static GameManager* instance;
    
    GameManager();
    // GameManager(const GameManager&) = delete;  // 복사 생성자 삭제
    // GameManager& operator = (const GameManager&) = delete; // 대입 생성자 방지용
    
    //핸들러 함수
    void StarterHandler(); //초기 캐릭터 생성
    void ExploreHandler(); //몬스터 조우 전 (탐험중)
    void BattleHandler(); // 몬스터 조우 및 전투 승리 시 다시 탐험중
    void GameEndHandler(); // 패배 시 게임종료
    //void ShopHandler(); shopManager 완성 시 사용
    
    GameStateMachine curentGameState; //현재 상태 저장용
    BattleManager* battleManager;
    Character* player;
    
public:
    static GameManager& GetInstance();
    
    void Run();

    
    
};

