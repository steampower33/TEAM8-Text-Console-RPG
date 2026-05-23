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

void UIManager::PrintLineAt(int x, int y, std::string& line)
{
    Gotoxy(x, y);
    std::cout << line;
}

void UIManager::DrawBox(int startX, int startY, int endX, int endY) {
    int width = endX - (startX + 1);

    // 윗변 그리기
    std::string topBorder = "╔" + RepeatString("═", width) + "╗";
    PrintLineAt(startX, startY, topBorder);

    // 양옆 벽 그리기
    for (int i = startY + 1; i <= endY - 1; i++) {
        std::string line = "║";
        PrintLineAt(startX, i, line);
        PrintLineAt(endX, i, line);
    }

    // 아랫변 그리기
    std::string bottomBorder = "╚" + RepeatString("═", width) + "╝";
    PrintLineAt(startX, endY, bottomBorder);
}

int UIManager::GetTitleResult() {

    DrawBox(0,0, ConsoleWidth - 1, ConsoleHeight - 1);
    DrawTitleMenu();

    return ShowMenuAt(Vec2{60, 30}, { "게임 시작", "게임 종료" });
}

int UIManager::GetMainResult()
{
    return ShowMenuAt(Vec2{StartChooseX + 2, StartChooseY + 2},{ "전투", "아이템 사용" });
}

int UIManager::ShowMenuAt(Vec2 at, const std::vector<std::string>& menuList, int step, bool isVertical) {
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
            std::string line;
            if (i == selectedIndex) {
                line += std::string("[  ") + menuList[i] + std::string("  ]");
            } else {
                line += std::string("   ") + menuList[i] + std::string("   ");
            }
            
            if (isVertical)
                PrintLineAt(at.x, at.y + (i * step), line);
            else
            {
                if (i != 0)
                {
                    PrintLineAt(at.x + menuList[i - 1].length() + 6, at.y, line);
                }
                else
                {
                    PrintLineAt(at.x, at.y, line);
                }
            }
        }

        int key = _getch();
        if (key == KEY_ESC) { // ESC 키를 눌렀을 때
            return -1;
        }
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
            case KEY_LEFT:
                selectedIndex--;
                if (selectedIndex < 0) selectedIndex = maxMenu - 1;
                break;
            case KEY_RIGHT:
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
        PrintLineAt(StartStatX + 4, StartStatY + 2 + i, line);
    }
}

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
        PrintLineAt(StartLogX + 1, i, blankLine);
    }
    
    size_t logSize = LogMessages.size();
    for (int i = 0; i < logSize; i++)
    {
        std::string line = std::string("> ") + LogMessages[i];
        PrintLineAt(StartLogX + 2, EndLogY - 1 - (logSize - i), line);
    }
}

void UIManager::UpdateInventory(Inventory* inven)
{
    std::vector<IItem*> items = inven->GetItems();
    for (int i = 0; i < inven->GetItems().size(); i++)
    {
        std::string name = items[i]->GetName();
        PrintLineAt(StartInventoryX + 5, StartInventoryY + 2 + (i * 1), name);
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
    PrintLineAt(StartSceneX + 5, StartSceneY, line);
}

void UIManager::DrawStatPanel()
{
    DrawBox(StartStatX,StartStatY, EndStatX, EndStatY);
    
    std::string line = "Stat";
    PrintLineAt(EndSceneX + 1 + 5, StartSceneY, line);
}

void UIManager::DrawInventoryPanel()
{
    DrawBox(StartInventoryX,StartInventoryY, EndInventoryX, EndInventoryY);
    
    std::string line = "Inventory";
    PrintLineAt(StartInventoryX + 1 + 5, StartInventoryY, line);
}

void UIManager::DrawLogPanel()
{
    DrawBox(StartLogX, StartLogY, EndLogX, EndLogY);
    
    std::string line = "Log";
    PrintLineAt(StartSceneX + 5, EndSceneY + 1, line);
}

void UIManager::DrawMenuPanel()
{
    DrawBox(StartChooseX, StartChooseY, EndChooseX, EndChooseY);
    
    std::string line = "Choose";
    PrintLineAt(EndSceneX + 1 + 5, EndSceneY + 1, line);
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
        PrintLineAt(startX, startY + i, art[i]);
    }
}
