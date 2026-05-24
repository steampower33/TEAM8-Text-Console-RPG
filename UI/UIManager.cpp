#include "../Character/Character.h"
#include "../Item/Inventory.h"
#include "../Item/IItem.h"

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <iomanip>

#include "UIManager.h"

#define KEY_UP    72
#define KEY_DOWN  80
#define KEY_LEFT  75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC   27

void UIManager::Initialize() {
    std::string systemLine("");
    systemLine.append("mode con cols=");
    systemLine.append(std::to_string(ConsoleWidth));
    systemLine.append(" lines=");
    systemLine.append(std::to_string(ConsoleHeight));
    
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

std::string UIManager::RepeatString(const std::string& str, int count) {
    std::string result;
    // 최적화: 메모리가 중간에 부족해서 재할당되는 걸 막기 위해,
    // 필요한 만큼의 공간을 RAM에 미리 뚫어놓습니다.
    result.reserve(str.size() * count); 
    
    for (int i = 0; i < count; i++) {
        result += str;
    }
    return result;
}

void UIManager::PrintTextAt(int x, int y, const std::string& text)
{
    Gotoxy(x, y);
    std::cout << text;
}

int UIManager::GetDisplayWidth(const std::string& text) {
    int displayWidth = 0;
    for (size_t i = 0; i < text.length(); ) {
        // UTF-8의 첫 바이트를 확인하여 글자 바이트 수를 판별
        unsigned char c = text[i];
        if ((c & 0x80) == 0) {
            // 1바이트 문자 (영어, 숫자, 기호): 화면에서 1칸 차지
            displayWidth += 1;
            i += 1;
        } else {
            // 다중 바이트 문자 (한글 등): 화면에서 2칸 차지한다고 가정 (UTF-8 한글은 보통 3바이트)
            displayWidth += 2;
            i += 3; // 3바이트를 건너뜀
        }
    }
    return displayWidth;
}

void UIManager::PrintTextAlign(int startX, int endX, int y, const std::string& text, TextAlign align) {
    int displayWidth = GetDisplayWidth(text);
    int totalWidth = endX - startX;
    int targetX = startX;

    switch (align) {
    case TextAlign::Left:
        targetX = startX;
        break;
    case TextAlign::Center:
        // 남은 여백의 절반만큼 시작 위치를 뒤로 밉니다.
        targetX = startX + (totalWidth - displayWidth) / 2;
        break;
    case TextAlign::Right:
        // 전체 넓이에서 글자 크기만큼 뺀 곳에서 시작합니다.
        targetX = endX - displayWidth;
        break;
    }

    Gotoxy(targetX, y);
    std::cout << text;
}

void UIManager::DrawBox(int startX, int startY, int endX, int endY) {
    int width = endX - (startX + 1);

    // 윗변 그리기
    std::string topBorder = "╔" + RepeatString("═", width) + "╗";
    PrintTextAt(startX, startY, topBorder);

    // 양옆 벽 그리기
    for (int i = startY + 1; i <= endY - 1; i++) {
        std::string line = "║";
        PrintTextAt(startX, i, line);
        PrintTextAt(endX, i, line);
    }

    // 아랫변 그리기
    std::string bottomBorder = "╚" + RepeatString("═", width) + "╝";
    PrintTextAt(startX, endY, bottomBorder);
}

int UIManager::GetTitleResult()
{
    DrawBox(0,0, ConsoleWidth - 1, ConsoleHeight - 1);
    DrawTitleMenu();

    return ShowMenuAlign(1, ConsoleWidth - 1, 31, { "게임 시작", "게임 종료" }, 2, TextAlign::Center);
}

int UIManager::GetMainResult()
{
    return ShowMenuAt(Vec2{StartChooseX + 2, StartChooseY + 2},{ "전투", "아이템 사용" });
}

int UIManager::HandleMenuInput(int& selectedIndex, int maxMenu) {
    int key = _getch();
    if (key == KEY_ESC) return -1;
    if (key == 224) {
        key = _getch();
        switch (key) {
        case KEY_UP: case KEY_LEFT:
            selectedIndex--;
            if (selectedIndex < 0) selectedIndex = maxMenu - 1;
            break;
        case KEY_DOWN: case KEY_RIGHT:
            selectedIndex++;
            if (selectedIndex >= maxMenu) selectedIndex = 0;
            break;
        }
        return 0; // 아직 엔터 안 침
    }
    else if (key == KEY_ENTER) {
        return 1; // 엔터 침! (선택 완료)
    }
    return 0;
}

int UIManager::ShowMenuAt(Vec2 at, const std::vector<std::string>& menuList, int step, bool isVertical)
{
    int maxMenu = menuList.size();
    int selectedIndex = 0;
    
    while (true) {
        for (int i = 0; i < maxMenu; i++) {
            std::string text;
            if (i == selectedIndex) {
                text += std::string("[  ") + menuList[i] + std::string("  ]");
            } else {
                text += std::string("   ") + menuList[i] + std::string("   ");
            }
            
            if (isVertical)
                PrintTextAt(at.x, at.y + (i * step), text);
            else
            {
                if (i != 0)
                {
                    PrintTextAt(at.x + menuList[i - 1].length() + 6, at.y, text);
                }
                else
                {
                    PrintTextAt(at.x, at.y, text);
                }
            }
        }

        int inputResult = HandleMenuInput(selectedIndex, maxMenu);
        if (inputResult == -1) return -1; // ESC
        if (inputResult == 1) return selectedIndex; // ENTER
    }
}

int UIManager::ShowMenuAlign(int startX, int endX, int y, const std::vector<std::string>& menuList, int step, TextAlign textAlign)
{
    int maxMenu = menuList.size();
    int selectedIndex = 0;
    
    while (true) {
        for (int i = 0; i < maxMenu; i++) {
            std::string text;
            if (i == selectedIndex) {
                text += std::string("[  ") + menuList[i] + std::string("  ]");
            } else {
                text += std::string("   ") + menuList[i] + std::string("   ");
            }
            
            PrintTextAlign(startX, endX, y + (i * step), text, textAlign);
        }

        int inputResult = HandleMenuInput(selectedIndex, maxMenu);
        if (inputResult == -1) return -1; // ESC
        if (inputResult == 1) return selectedIndex; // ENTER
    }
}

void UIManager::UpdateStat(Character* character)
{
    // 출력할 [라벨, 값] 쌍을 순서대로 백터에 밀어 넣습니다. (데이터 구성)
    std::vector<std::pair<std::string, std::string>> statLines = {
        {"이름   ", character->Name},
        {"레벨   ", std::to_string(character->Level)},
        {"체력   ", std::to_string(character->Health) + " / " + std::to_string(character->MaxHealth)},
        {"공격력 ", std::to_string(character->Attack)},
        {"경험치 ", std::to_string(character->Experience)},
        {"골드   ", std::to_string(character->Gold)}
        // 나중에 "방어력"이 생기면 그냥 여기에 한 줄만 추가하면 끝납니다!
    };
 
    for (int i = 0; i < statLines.size(); i++) {
        // 가상의 출력 스트림(oss)을 만듭니다.
        std::ostringstream oss;
    
        // cout에 출력하듯이 oss에 << 연산자로 데이터와 setw를 밀어 넣습니다.
        oss << statLines[i].first << "│ " 
            << std::left << std::setw(14) << statLines[i].second;
    
        // 완성된 결과물을 std::string으로 뽑아냅니다.
        std::string line = oss.str();
    
        // 이제 하나로 묶인 line을 원하는 위치에 출력합니다.
        PrintTextAt(StartStatX + 4, StartStatY + 2 + i, line);
    }
}

// std::string sysLog = "\033[33m[📢 시스템] 플레이어의 턴입니다.\033[0m";
// std::string attackLog = "\033[31m[⚔️ 전투] 고블린에게 15의 치명타 피해!\t\033[0m";
// std::string itemLog = "\033[32m[🎁 획득] 낡은 롱소드를 얻었습니다.\t\033[0m";
// PrintLog(sysLog);
// PrintLog(attackLog);
// PrintLog(itemLog);
void UIManager::PrintLog(const std::string& message)
{
    if (message.size() != 0)
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
        PrintTextAt(StartLogX + 1, i, blankLine);
    }
    
    size_t logSize = LogMessages.size();
    for (int i = 0; i < logSize; i++)
    {
        std::string line = std::string("> ") + LogMessages[i];
        PrintTextAt(StartLogX + 2, EndLogY - 1 - (logSize - i), line);
    }
}

void UIManager::UpdateInventory(Inventory* inven)
{
    std::vector<IItem*> items = inven->GetItems();
    for (int i = 0; i < inven->GetItems().size(); i++)
    {
        std::string name = items[i]->GetName();
        PrintTextAt(StartInventoryX + 5, StartInventoryY + 2 + (i * 1), name);
    }
}

void UIManager::ChooseItem(Inventory* inven, Character* character)
{
    std::vector<IItem*> items = inven->GetItems();
    
    if (items.empty()) {
        PrintLog("\033[33m[시스템] 인벤토리가 비어있습니다.\033[0m");
        return;
    }
    
    std::vector<std::string> menuList;
    
    for (size_t i = 0; i < items.size(); i++)
    {
        std::string line = items[i]->GetName();
        menuList.push_back(line);
    }
    
    int itemIndex = ShowMenuAt({StartInventoryX + 2, StartInventoryY + 2}, menuList, 1);
    
    // 0 = 사용, 1 = 취소
    int wannaUse = ShowMenuAt({StartInventoryX + 8, EndInventoryY - 3}, 
        {"사용", "취소"}, 2, false);
    
    if (wannaUse == 0)
    {
        std::string line = character->Name + "이(가) " + items[itemIndex]->GetName() + "을(를) 사용했습니다!";
        inven->UseItem(itemIndex, character);
        
        PrintLog(line);
    }
}

// ====================================================== 
// = 아래로는 Panel 그리기 위주 함수
// ======================================================
void UIManager::ShowMainFrame()
{
    DrawScenePanel();
    DrawStatPanel();
    DrawInventoryPanel();
    DrawLogPanel();
    DrawMenuPanel();
}

void UIManager::DrawScenePanel()
{
    DrawBox(StartSceneX,StartSceneY, EndSceneX, EndSceneY);
    
    std::string line = "Scene";
    PrintTextAt(StartSceneX + 5, StartSceneY, line);
}

void UIManager::DrawStatPanel()
{
    DrawBox(StartStatX,StartStatY, EndStatX, EndStatY);
    
    std::string line = "Stat";
    PrintTextAt(EndSceneX + 1 + 5, StartSceneY, line);
}

void UIManager::DrawInventoryPanel()
{
    DrawBox(StartInventoryX,StartInventoryY, EndInventoryX, EndInventoryY);
    
    std::string line = "Inventory";
    PrintTextAt(StartInventoryX + 1 + 5, StartInventoryY, line);
}

void UIManager::DrawLogPanel()
{
    DrawBox(StartLogX, StartLogY, EndLogX, EndLogY);
    
    std::string line = "Log";
    PrintTextAt(StartSceneX + 5, EndSceneY + 1, line);
}

void UIManager::DrawMenuPanel()
{
    DrawBox(StartChooseX, StartChooseY, EndChooseX, EndChooseY);
    
    std::string line = "Choose";
    PrintTextAt(EndSceneX + 1 + 5, EndSceneY + 1, line);
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
        PrintTextAt(startX, startY + i, art[i]);
    }
}
