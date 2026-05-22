#pragma once
#include <string>

class Character;

class UIManager {
public:
    // 시스템 초기화 (창 크기 고정, 커서 숨기기)
    void Initialize();

    // 타이틀 화면 렌더링 및 메뉴 선택 (반환값: 0=게임시작, 1=종료)
    int ShowTitleMenu();
    
    // 게임 플레이 화면
    void ShowMainFrame();
    
    // 로그 추가 및 화면 갱신
    void PrintLog(const std::string& message);

    // 캐릭터 포인터를 받아서 스탯 패널 텍스트 갱신
    void UpdateStat(Character* character);
    
    // 인벤토리 목록 갱신
    void UpdateInventory();
    
private:
    void DrawBox(int startX, int startY, int endX, int endY);
    void Gotoxy(int x, int y);
    void DrawTitleMenu();
    
    void DrawScenePanel();
    void DrawStatInventoryPanel();
    void DrawLogPanel();
    void DrawChoosePanel();

private:
    int ConsoleWidth = 150;
    int ConsoleHeight = 40;
    
    int StartSceneX = 0;
    int StartSceneY = 0;
    int EndSceneX = 110;
    int EndSceneY = 25;
    
    int StartStatInventoryX = EndSceneX + 1;
    int StartStatInventoryY = StartSceneY;
    int EndStatInventoryX = ConsoleWidth - 1;
    int EndStatInventoryY = EndSceneY;
    
    int StartLogX = StartSceneX;
    int StartLogY = EndSceneY + 1;
    int EndLogX = EndSceneX;
    int EndLogY = ConsoleHeight - 1;
    
    int StartChooseX = EndSceneX + 1;
    int StartChooseY = EndSceneY + 1;
    int EndChooseX = ConsoleWidth - 1;
    int EndChooseY = ConsoleHeight - 1;
    
};