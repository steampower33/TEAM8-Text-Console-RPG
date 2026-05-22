#include "UIManager.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <iomanip>

#include "../Character/Character.h"

#define KEY_UP    72
#define KEY_DOWN  80
#define KEY_ENTER 13

using namespace std;

void UIManager::Initialize() {
    string systemLine("");
    systemLine.append("mode con cols=");
    systemLine.append(to_string(ConsoleWidth));
    systemLine.append(" lines=");
    systemLine.append(to_string(ConsoleHeight));
    
    // system("mode con cols=150 lines=40");
    system(systemLine.c_str());

    // 깜빡이는 커서 숨기기
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    SetConsoleOutputCP(CP_UTF8); // 콘솔을 UTF-8 모드로 강제 전환
}

void UIManager::Gotoxy(int x, int y) {
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void UIManager::DrawBox(int startX, int startY, int endX, int endY) {
    // 윗변 그리기
    Gotoxy(startX, startY);
    std::cout << "╔";
    for (int i = startX + 1; i < endX; i++) std::cout << "═";
    std::cout << "╗";

    // 양옆 벽 그리기
    for (int i = startY + 1; i <= endY - 1; i++) {
        Gotoxy(startX, i);   std::cout << "║";
        Gotoxy(endX, i); std::cout << "║";
    }

    // 아랫변 그리기
    Gotoxy(startX, endY);
    std::cout << "╚";
    for (int i = startX + 1; i < endX; i++) std::cout << "═";
    std::cout << "╝";
}

void UIManager::ShowMainFrame()
{
    DrawScenePanel();
    DrawStatInventoryPanel();
    DrawLogPanel();
    DrawChoosePanel();
}

int UIManager::ShowMenuBox(const std::vector<std::string>& menuList) {
    int maxMenu = menuList.size();
    int selectedIndex = 0;
    
    // std::string sysLog = "\033[33m[📢 시스템] 플레이어의 턴입니다.\033[0m";
    // std::string attackLog = "\033[31m[⚔️ 전투] 고블린에게 15의 치명타 피해!\t\033[0m";
    // std::string itemLog = "\033[32m[🎁 획득] 낡은 롱소드를 얻었습니다.\t\033[0m";
    
    while (true) {
        // PrintLog(sysLog);
        // PrintLog(attackLog);
        // PrintLog(itemLog);
        // 메뉴 렌더링 (그리는 역할만 수행)
        for (int i = 0; i < maxMenu; i++) {
            // Choose 패널 구역 안에 그리기
            Gotoxy(StartChooseX + 2, StartChooseY + 2 + (i * 2)); 
            
            if (i == selectedIndex) {
                std::cout << " ▶  " << menuList[i] << "  ◀ "; 
            } else {
                std::cout << "    " << menuList[i] << "    "; 
            }
        }

        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case KEY_UP:
                selectedIndex--;
                if (selectedIndex < 0) selectedIndex = maxMenu - 1;
                break;
            case KEY_DOWN:
                selectedIndex++;
                if (selectedIndex >= maxMenu) selectedIndex = 0;
                break;
            }
        }
        else if (key == KEY_ENTER) {
            // 🚨 주의: 여기서 system("cls")를 해버리면 애써 그린 3분할 테두리가 다 날아갑니다!
            // 화면을 지우지 않고 그냥 번호만 토스합니다.
            return selectedIndex; 
        }
    }
}

void UIManager::UpdateStat(Character* character)
{
    string Name = character->Name;
    int Level = character->Level;
    int Health = character->Health;
    int MaxHealth = character->MaxHealth;
    int Attack = character->Attack;
    int Experience = character->Experience;
    int Gold = character->Gold;
 
    int left = 3;
    
    Gotoxy(StartStatInventoryX + left, StartStatInventoryY + 2);   
    std::cout << "이름   │ " << std::left << std::setw(14) << Name;
    
    Gotoxy(StartStatInventoryX + left, StartStatInventoryY + 3);
    std::cout << "레벨   │ " << std::left << std::setw(14) << Level;
    
    Gotoxy(StartStatInventoryX + left, StartStatInventoryY + 4);
    std::cout << "체력   │ " << std::left << std::setw(14) << std::to_string(Health) + " / " + std::to_string(MaxHealth);
    
    Gotoxy(StartStatInventoryX + left, StartStatInventoryY + 5);
    std::cout << "공격력 │ " << std::left << std::setw(14) << Attack;
    
    Gotoxy(StartStatInventoryX + left, StartStatInventoryY + 6);
    std::cout << "경험치 │ " << std::left << std::setw(14) << Experience;
    
    Gotoxy(StartStatInventoryX + left, StartStatInventoryY + 7);
    std::cout << "골드   │ " << std::left << std::setw(14) << Gold;
}

void UIManager::PrintLog(const std::string& message)
{
    LogMessages.push_back(message);
    
    int innerHeight = EndLogY - StartLogY - 1;
    
    if (LogMessages.size() >= innerHeight)
    {
        LogMessages.erase(LogMessages.begin());
    }
    
    int innerWidth = EndLogX - StartLogX - 1;
    std::string blankLine(innerWidth, ' ');
    for (int i = StartLogY + 1; i < EndLogY; i++)
    {
        Gotoxy(StartLogX + 1, i);
        std::cout << blankLine;
    }
    
    for (int i = 0; i < LogMessages.size(); i++)
    {
        Gotoxy(StartLogX + 2, EndLogY - 1 - i);
        std::cout << "> " << LogMessages[i];
    }
}

void UIManager::DrawScenePanel()
{
    DrawBox(StartSceneX,StartSceneY, EndSceneX, EndSceneY);
    
    Gotoxy(StartSceneX + 5, StartSceneY);
    std::cout << "Scene";
}

void UIManager::DrawStatInventoryPanel()
{
    DrawBox(StartStatInventoryX,StartStatInventoryY, EndStatInventoryX, EndStatInventoryY);
    
    Gotoxy(EndSceneX + 1 + 5, StartSceneY);
    std::cout << "Stat";
}

void UIManager::DrawLogPanel()
{
    DrawBox(StartLogX, StartLogY, EndLogX, EndLogY);
    
    Gotoxy(StartSceneX + 5, EndSceneY + 1);
    std::cout << "Log";
}

void UIManager::DrawChoosePanel()
{
    DrawBox(StartChooseX, StartChooseY, EndChooseX, EndChooseY);
    
    Gotoxy(EndSceneX + 1 + 5, EndSceneY + 1);
    std::cout << "Choose";
}

void UIManager::DrawTitleMenu() {
    // 15줄짜리 거대 아스키 아트를 배열에 저장
    // R"EOF( )EOF" 를 사용하여 내부의 모든 특수기호를 안전하게 보호함
    std::string art[15] = {
        R"EOF(                                                                 ,──,)EOF",
        R"EOF(               ,────..             ,──.                ,────..   ,───.'│                               ,─.────.)EOF",
        R"EOF(  ,────..     ╱   ╱   ╲         ,──.'│  .──.──.      ╱   ╱   ╲  │   │ :        ,───,.        ,─.────.  ╲    ╱  ╲    ,────..)EOF",
        R"EOF( ╱   ╱   ╲   ╱   .     :    ,──,:  : │ ╱  ╱   '.    ╱   .     : :   : │      ,'  .' │        ╲    ╱  ╲  │   :    ╲  ╱   ╱   ╲)EOF",
        R"EOF(│   :     : .   ╱   ;.  ╲,`──.'`│  ' :│  :  ╱`. ╱  .   ╱   ;.  ╲│   ' :    ,───.'   │        ;   :    ╲ │   │  .╲ :│   :     :)EOF",
        R"EOF(.   │  ;. ╱.   ;   ╱  ` ;│   :  :  │ │;  │  │──`  .   ;   ╱  ` ;;   ; '    │   │   .'        │   │ .╲ : .   :  │: │.   │  ;. ╱)EOF",
        R"EOF(.   ; ╱──` ;   │  ; ╲ ; │:   │   ╲ │ :│  :  ;_    ;   │  ; ╲ ; │'   │ │__ :   :  │─,        .   : │: │ │   │   ╲ :.   ; ╱──`)EOF",
        R"EOF(;   │ ;    │   :  │ ; │ '│   : '  '; │ ╲  ╲    `. │   :  │ ; │ '│   │ :.'│:   │  ;╱│        │   │  ╲ : │   : .   ╱;   │ ;  __)EOF",
        R"EOF(│   : │    .   │  ' ' ' :'   ' ;.    ;  `────.   ╲.   │  ' ' ' :'   :    ;│   :   .'        │   : .  ╱ ;   │ │`─' │   : │.' .')EOF",
        R"EOF(.   │ '___ '   ;  ╲; ╱  ││   │ │ ╲   │  __ ╲  ╲  │'   ;  ╲; ╱  ││   │  .╱ │   │  │─,        ;   │ │  ╲ │   │ ;    .   │ '_.' :)EOF",
        R"EOF('   ; : .'│ ╲   ╲  ',  ╱ '   : │  ; .' ╱  ╱`──'  ╱ ╲   ╲  ',  ╱ ;   : ;   '   :  ;╱│        │   │ ;╲  ╲:   ' │    '   ; : ╲  │)EOF",
        R"EOF('   │ '╱  :  ;   :    ╱  │   │ '`──'  '──'.     ╱  ;   :    ╱  │   ,╱    │   │    ╲        :   ' │ ╲.':   : :    '   │ '╱  .')EOF",
        R"EOF(│   :    ╱    ╲   ╲ .'   '   : │        `──'───'     ╲   ╲ .'  '───'     │   :   .'        :   : :─'  │   │ :    │   :    ╱)EOF",
        R"EOF( ╲   ╲ .'      `───`     ;   │.'                     `───`               │   │ ,'          │   │.'    `───'.│     ╲   ╲ .')EOF",
        R"EOF(  `───`                  '───'                                           `────'            `───'        `───`      `───`)EOF"
    };
    
    int startX = 12; // 화면 오른쪽에서부터 띄우고 그리기 시작
    int startY = 3; // 화면 위에서부터 띄우고 그리기 시작

    // 15줄을 한 줄씩 좌표를 밑으로 내리며(startY + i) 렌더링
    for (int i = 0; i < 15; i++) {
        Gotoxy(startX, startY + i);
        std::cout << art[i];
    }
}

int UIManager::ShowTitleMenu() {

    DrawBox(0,0, 149, 39);
    DrawTitleMenu();

    const int MAX_MENU = 2;
    std::string menu[MAX_MENU] = { "게임 시작", "게임 종료" };
    int selectedIndex = 0;

    while (true) {
        // 메뉴 렌더링
        for (int i = 0; i < MAX_MENU; i++) {
            Gotoxy(60, 30 + (i * 2)); // Y좌표를 2칸씩 띄워서 여유롭게 배치
            
            if (i == selectedIndex) {
                std::cout << " ▶  " << menu[i] << "  ◀ "; 
            } else {
                std::cout << "    " << menu[i] << "    "; 
            }
        }

        // 키 입력 대기
        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case KEY_UP:
                selectedIndex--;
                if (selectedIndex < 0) selectedIndex = MAX_MENU - 1;
                break;
            case KEY_DOWN:
                selectedIndex++;
                if (selectedIndex >= MAX_MENU) selectedIndex = 0;
                break;
            }
        }
        else if (key == KEY_ENTER) {
            // 화면을 깨끗하게 지우고 선택한 값을 반환
            system("cls"); 
            return selectedIndex;
        }
    }
}