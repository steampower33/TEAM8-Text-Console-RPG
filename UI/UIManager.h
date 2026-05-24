#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <iomanip>

class Character;
class Inventory;
class IItem;

enum class TextAlign {
    Left,
    Center,
    Right
};

struct Vec2 {
    int x;
    int y;
};

struct Box
{
    int startX;
    int startY;
    int endX;
    int endY;
};

class UIManager {
public:
    void Initialize(); // 시스템 초기화 (창 크기 고정, 커서 숨기기)
    std::string ShowCharacterGeneration();
    int GetTitleResult(); // 타이틀 화면 렌더링 및 메뉴 선택 (반환값: 0=게임시작, 1=종료)
    int GetMainResult();
    void ShowMainFrame(); // 메인 게임 플레이 프레임 그리기
    int ShowMenuAt(Vec2 at, const std::vector<std::string>& menuList, int step = 2, bool isVertical = true);
    int ShowMenuAlign(int startX, int endX, int y, const std::vector<std::string>& menuList, int step = 2, TextAlign textAlign = TextAlign::Left);
    void UpdateStat(Character* character); // 캐릭터 포인터를 받아서 스탯 패널 텍스트 갱신
    
    // 로그 추가 및 화면 갱신
    std::vector<std::string> LogMessages;
    void PrintLog(const std::string& message = "");
    
    // 인벤토리 목록 갱신
    void UpdateInventory(Inventory* inven);
    
    void ChooseItem(Inventory* inven, Character* character);
    
private:
    int HandleMenuInput(int& selectedIndex, int maxMenu);
    void PrintTextAt(int x, int y, const std::string& text);
    int GetDisplayWidth(const std::string& text);
    void PrintTextAlign(int startX, int endX, int y, const std::string& text, TextAlign align);
    std::string RepeatString(const std::string& str, int count);
    void DrawBox(int startX, int startY, int endX, int endY);
    void Gotoxy(int x, int y);
    void DrawTitleMenu();
    
    void DrawScenePanel();
    void DrawStatPanel();
    void DrawInventoryPanel();
    void DrawLogPanel();
    void DrawMenuPanel();

private:
    int ConsoleWidth = 150;
    int ConsoleHeight = 40;
    
    int StartSceneX = 0;
    int StartSceneY = 0;
    int EndSceneX = 110;
    int EndSceneY = 25;
    
    int StartStatX = EndSceneX + 1;
    int StartStatY = StartSceneY;
    int EndStatX = ConsoleWidth - 1;
    int EndStatY = 9;
    
    int StartInventoryX = StartStatX;
    int StartInventoryY = EndStatY + 1;
    int EndInventoryX = ConsoleWidth - 1;
    int EndInventoryY = EndSceneY;
    
    int StartLogX = StartSceneX;
    int StartLogY = EndSceneY + 1;
    int EndLogX = EndSceneX;
    int EndLogY = ConsoleHeight - 1;
    
    int StartChooseX = EndSceneX + 1;
    int StartChooseY = EndSceneY + 1;
    int EndChooseX = ConsoleWidth - 1;
    int EndChooseY = ConsoleHeight - 1;
    
};