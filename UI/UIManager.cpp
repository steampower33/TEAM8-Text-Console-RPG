#include "UIManager.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

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
    
    const int MAX_MENU = 2;
    std::string menu[MAX_MENU] = { "전투", "아이템 사용" };
    int selectedIndex = 0;

    while (true) {
        // 메뉴 렌더링
        for (int i = 0; i < MAX_MENU; i++) {
            Gotoxy(StartChooseX + 2, StartChooseY + 2 + (i * 2)); // Y좌표를 2칸씩 띄워서 여유롭게 배치
            
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
            // return selectedIndex;
        }
    }
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