#include "../Character/Character.h"
#include "../Item/Inventory.h"
#include "../Item/IItem.h"
#include "UIManager.h"
UIManager ui;

// 상점 들어갈지 말지 선택지
// 상점 UI 만들기
// 메타몽 그리기
// 오디오 추가

void UIManager::Initialize()
{
    std::string systemLine("");
    systemLine.append("mode con cols=");
    systemLine.append(std::to_string(ConsoleWidth));
    systemLine.append(" lines=");
    systemLine.append(std::to_string(ConsoleHeight));

    // system("mode con cols=150 lines=40");
    system(systemLine.c_str());

    // 깜빡이는 커서 숨기기
    CONSOLE_CURSOR_INFO cursorInfo = {0,};
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    SetConsoleOutputCP(CP_UTF8); // 콘솔을 UTF-8 모드로 강제 전환
}

std::string UIManager::ShowCharacterGeneration()
{
    // 기존 화면을 깔끔하게 지웁니다. 
    // (캐릭터 생성은 게임 루프 시작 전의 독립된 '씬'이므로 cls를 써도 무방합니다.)
    system("cls");

    // 화면 정중앙에 그릴 박스의 크기와 좌표를 계산합니다.
    int boxWidth = 50;
    int boxHeight = 6;
    int startX = (ConsoleWidth - boxWidth) / 2;
    int startY = (ConsoleHeight - boxHeight) / 2;
    int endX = startX + boxWidth;
    int endY = startY + boxHeight;

    // 중앙 박스 및 안내 문구 렌더링
    DrawBox(startX, startY, endX, endY);

    PrintTextAlign(startX,
                   endX,
                   startY + 2,
                   "새로운 모험가의 이름을 입력하세요",
                   TextAlign::Center);

    // 입력 칸 프롬프트 렌더링
    std::string prompt = "이름 : ";
    PrintTextAt(startX + 4, startY + 4, prompt);

    // 숨겨놨던 커서를 유저가 볼 수 있도록 임시로 켭니다.
    CONSOLE_CURSOR_INFO cursorInfo = {0,};
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = TRUE; // 커서 On!
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    // 이름 입력 받기
    std::string name;

    // std::cin >> name; 대신 getline을 쓰는 이유:
    // cin은 띄어쓰기를 입력하면 그 뒷부분을 잘라버립니다. ("빛의 기사" -> "빛의"만 저장됨)
    // getline은 엔터를 칠 때까지의 모든 띄어쓰기를 포함해 온전히 가져옵니다.
    std::getline(std::cin, name);

    // 입력이 끝났으니 커서를 다시 숨겨서 게임 UI 모드로 복구합니다.
    cursorInfo.bVisible = FALSE; // 커서 Off!
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    // 유저가 아무것도 안 치고 엔터만 쳤을 경우의 예외 처리 (방어 코드)
    if (name.empty())
    {
        name = "무명"; // 디폴트 네임
    }

    return name;
}

void UIManager::Gotoxy(int x, int y)
{
    COORD pos = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

std::string UIManager::RepeatString(const std::string& str, int count)
{
    std::string result;
    // 최적화: 메모리가 중간에 부족해서 재할당되는 걸 막기 위해,
    // 필요한 만큼의 공간을 RAM에 미리 뚫어놓습니다.
    result.reserve(str.size() * count);

    for (int i = 0; i < count; i++)
    {
        result += str;
    }
    return result;
}

void UIManager::PrintTextAt(int x, int y, const std::string& text)
{
    Gotoxy(x, y);
    std::cout << text;
}

int UIManager::GetDisplayWidth(const std::string& text)
{
    int displayWidth = 0;
    for (size_t i = 0; i < text.length();)
    {
        // UTF-8의 첫 바이트를 확인하여 글자 바이트 수를 판별
        unsigned char c = text[i];
        if ((c & 0x80) == 0)
        {
            // 1바이트 문자 (영어, 숫자, 기호): 화면에서 1칸 차지
            displayWidth += 1;
            i += 1;
        }
        else
        {
            // 다중 바이트 문자 (한글 등): 화면에서 2칸 차지한다고 가정 (UTF-8 한글은 보통 3바이트)
            displayWidth += 2;
            i += 3; // 3바이트를 건너뜀
        }
    }
    return displayWidth;
}

void UIManager::PrintTextAlign(int startX,
                               int endX,
                               int y,
                               const std::string& text,
                               TextAlign align)
{
    int displayWidth = GetDisplayWidth(text);
    int totalWidth = endX - startX;
    int targetX = startX;

    switch (align)
    {
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

void UIManager::DrawBox(int startX, int startY, int endX, int endY)
{
    int width = endX - (startX + 1);

    // 윗변 그리기
    std::string topBorder = "╔" + RepeatString("═", width) + "╗";
    PrintTextAt(startX, startY, topBorder);

    // 양옆 벽 그리기
    for (int i = startY + 1; i <= endY - 1; i++)
    {
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
    DrawBox(0, 0, ConsoleWidth - 1, ConsoleHeight - 1);
    DrawTitleMenu();

    return ShowMenuAlign(1,
                         ConsoleWidth - 1,
                         25,
                         {"게임 시작", "게임 종료"},
                         2,
                         TextAlign::Center);
}

int UIManager::HandleMenuInput(int& selectedIndex, int maxMenu)
{
    int key = _getch();
    if (key == KEY_ESC)
        return -1;
    if (key == 224)
    {
        key = _getch();
        switch (key)
        {
        case KEY_UP:
        case KEY_LEFT:
            selectedIndex--;
            if (selectedIndex < 0)
                selectedIndex = maxMenu - 1;
            break;
        case KEY_DOWN:
        case KEY_RIGHT:
            selectedIndex++;
            if (selectedIndex >= maxMenu)
                selectedIndex = 0;
            break;
        }
        return 0; // 아직 엔터 안 침
    }
    else if (key == KEY_ENTER)
    {
        return 1; // 엔터 침! (선택 완료)
    }
    return 0;
}

int UIManager::ShowMenuAt(Vec2 at,
                          const std::vector<std::string>& menuList,
                          int step,
                          bool isVertical)
{
    int maxMenu = menuList.size();
    int selectedIndex = 0;

    while (true)
    {
        for (int i = 0; i < maxMenu; i++)
        {
            std::string text;
            if (i == selectedIndex)
            {
                text += std::string("[  ") + menuList[i] + std::string("  ]");
            }
            else
            {
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
        if (inputResult == -1)
            return -1; // ESC
        if (inputResult == 1)
            return selectedIndex; // ENTER
    }
}

int UIManager::ShowMenuAlign(int startX,
                             int endX,
                             int y,
                             const std::vector<std::string>& menuList,
                             int step,
                             TextAlign textAlign)
{
    int maxMenu = menuList.size();
    int selectedIndex = 0;

    while (true)
    {
        for (int i = 0; i < maxMenu; i++)
        {
            std::string text;
            if (i == selectedIndex)
            {
                text += std::string("[  ") + menuList[i] + std::string("  ]");
            }
            else
            {
                text += std::string("   ") + menuList[i] + std::string("   ");
            }

            PrintTextAlign(startX, endX, y + (i * step), text, textAlign);
        }

        int inputResult = HandleMenuInput(selectedIndex, maxMenu);
        if (inputResult == -1)
            return -1; // ESC
        if (inputResult == 1)
            return selectedIndex; // ENTER
    }
}

void UIManager::UpdateStat(Character* character)
{
    // 출력할 [라벨, 값] 쌍을 순서대로 백터에 밀어 넣습니다. (데이터 구성)
    std::vector<std::pair<std::string, std::string>> statLines = {
        {"이름   ", character->Name},
        {"레벨   ", std::to_string(character->Level)},
        {"체력   ",
         std::to_string(character->Health) + " / " + std::to_string(
             character->MaxHealth)},
        {"공격력 ", std::to_string(character->Attack)},
        {"경험치 ", std::to_string(character->Experience)},
        {"골드   ", std::to_string(character->Gold)}
        // 나중에 "방어력"이 생기면 그냥 여기에 한 줄만 추가하면 끝납니다!
    };

    for (int i = 0; i < statLines.size(); i++)
    {
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

void UIManager::UpdateScene(bool isCombat, std::string monsterName)
{
    if (!isCombat)
    {
        // 1. 비전투(Idle) 상태: 대검 출력
        for (int i = 0; i < warrior.size(); i++)
        {
            PrintTextAlign(StartSceneX + 1,
                           EndSceneX - 1,
                           StartSceneY + 1 + i,
                           warrior[i],
                           TextAlign::Left);
        }
    }
    else
    {
        for (int i = 0; i < warrior.size(); i++)
        {
            PrintTextAlign(StartSceneX + 1,
                           EndSceneX - 1,
                           StartSceneY + 1 + i,
                           warrior[i],
                           TextAlign::Left);
        }

        // 2. 전투(Combat) 상태: 몬스터 이름에 따라 아트 스왑
        int monsterX = StartSceneX + 55; // 우측 배치 기준 X좌표
        int startY = StartSceneY + 1;

        // C++ string의 find를 쓰는 이유: 팀원분 코드를 보면 레벨이 0일 때 "Slime 0" 처럼 
        // 뒤에 숫자가 붙는 예외 처리가 있어서, 이름 포함 여부로 검사하는 것이 안전합니다.

        int SceneHeight = EndSceneY - StartSceneY - 1;

        if (monsterName.find("Goblin") != std::string::npos)
        {
            // 뾰족한 고블린 배열 출력
            for (int i = 0; i < std::min(int(goblin.size()), SceneHeight); i++)
            {
                PrintTextAt(monsterX, startY + i, goblin[i]);
            }
        }
        else if (monsterName.find("Orc") != std::string::npos)
        {
            for (int i = 0; i < std::min(int(orc.size()), SceneHeight); i++)
            {
                PrintTextAt(monsterX, startY + i, orc[i]);
            }
        }
        else if (monsterName.find("Troll") != std::string::npos)
        {
            for (int i = 0; i < std::min(int(troll.size()), SceneHeight); i++)
            {
                PrintTextAt(monsterX, startY + i, troll[i]);
            }
        }
        else if (monsterName.find("Slime") != std::string::npos)
        {
            for (int i = 0; i < std::min(int(slime.size()), SceneHeight); i++)
            {
                PrintTextAt(monsterX, startY + i, slime[i]);
            }
        }
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
    int innerWidth = EndInventoryX - StartInventoryX - 1;
    std::string blankLine(innerWidth, ' '); // 너비만큼의 공백 문자열 생성
    
    for (int i = StartInventoryY + 1; i < EndInventoryY; i++)
    {
        PrintTextAt(StartInventoryX + 1, i, blankLine);
    }
    
    std::vector<IItem*> items = inven->GetItems();
    for (int i = 0; i < inven->GetItems().size(); i++)
    {
        std::string name = items[i]->GetName();
        PrintTextAt(StartInventoryX + 5, StartInventoryY + 2 + (i * 1), name);
    }
}

void UIManager::ShowShop(Character* character)
{
    int boxWidth = 50;
    int boxHeight = 20;
    int startX = (ConsoleWidth - boxWidth) / 2;
    int startY = (ConsoleHeight - boxHeight) / 2;
    int endX = startX + boxWidth;
    int endY = startY + boxHeight;
    DrawBox(startX, startY, endX, endY);

    int actMenuWidth = 16;
    
    std::string blankLine = RepeatString(" ", endX - startX - 1);
    std::string verticalAxis = "║";
    
    Inventory& inventory = character->CharacterInventory;
    bool loop = true;
    while (loop)
    {
        DrawBox(startX, startY, endX, endY);
        for (int i = 0; i < boxHeight - 1; i++)
        {
            PrintTextAt(startX + 1, startY + 1 + i, blankLine);
            PrintTextAt(startX + 1 + actMenuWidth, startY + 1 + i, verticalAxis);
        }
        
        int actChoose = ShowMenuAt({startX + 2, startY + 2}, {"구매하기", "판매하기", "나가기"});
        switch (actChoose)
        {
        case 0:
        {
            std::vector<std::string> menuList;
            for (int i = 0; i < 4; i++)
                menuList.push_back(std::string("구매") + std::to_string(i));
            menuList.push_back("뒤로가기");
            int choose = ShowMenuAlign(startX + 2 + actMenuWidth + 2, endX - 1, startY + 2, menuList, 2, TextAlign::Center);
            
            if (choose != menuList.size() - 1)
            {
                int buyOrNot = ShowMenuAt({startX + 2 + actMenuWidth + 5, endY - 2}, {"구매", "취소"}, 2, false);
                if (buyOrNot)
                {
                    
                }
            }
            break;
        }
        case 1:
        {
            std::vector<IItem*> items = inventory.GetItems();
            
            std::vector<std::string> menuList;
            for (int i = 0; i < items.size(); i++)
            {
                menuList.push_back(items[i]->GetName() + "(" + std::to_string(items[i]->GetGold()) + ")");
            }
            menuList.push_back("뒤로가기");
            int choose = ShowMenuAlign(startX + 2 + actMenuWidth + 2, endX - 1, startY + 2, menuList, 2, TextAlign::Center);
            
            if (choose != menuList.size() - 1)
            {
                int purchaseOrNot = ShowMenuAt({startX + 2 + actMenuWidth + 5, endY - 2}, {"판매", "취소"}, 2, false);
                if (purchaseOrNot == 0)
                {
                    character->Gold += std::max(0, items[choose]->GetGold());
                    // inventory.RemoveItem(choose); // inventory 헤더에 함수 추가해야함
                    UpdateInventory(&inventory);
                    UpdateStat(character);
                    PrintLog("\033[32m[아이템]\033[0m 아이템을 판매했습니다.");
                }
            }
            break;
        }
        case 2:
        {
            loop = false;
            break;
        }
        }
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
}

void UIManager::DrawScenePanel()
{
    DrawBox(StartSceneX, StartSceneY, EndSceneX, EndSceneY);

    std::string line = "Scene";
    PrintTextAt(StartSceneX + 5, StartSceneY, line);
}

void UIManager::DrawStatPanel()
{
    DrawBox(StartStatX, StartStatY, EndStatX, EndStatY);

    std::string line = "Stat";
    PrintTextAt(EndSceneX + 1 + 5, StartSceneY, line);
}

void UIManager::DrawInventoryPanel()
{
    DrawBox(StartInventoryX, StartInventoryY, EndInventoryX, EndInventoryY);

    std::string line = "Inventory";
    PrintTextAt(StartInventoryX + 1 + 5, StartInventoryY, line);
}

void UIManager::DrawLogPanel()
{
    DrawBox(StartLogX, StartLogY, EndLogX, EndLogY);

    std::string line = "Log";
    PrintTextAt(StartSceneX + 5, EndSceneY + 1, line);
}

void UIManager::DrawTitleMenu()
{
    // 11줄짜리 아스키 아트를 배열에 저장
    // R"EOF( )EOF" 를 사용하여 내부의 모든 특수기호를 안전하게 보호함
    std::string art[11] = {
        R"EOF(  ╱$$$$$$   ╱$$$$$$  ╱$$   ╱$$  ╱$$$$$$   ╱$$$$$$  ╱$$       ╱$$$$$$$$       ╱$$$$$$$  ╱$$$$$$$   ╱$$$$$$ )EOF",
        R"EOF( ╱$$__  $$ ╱$$__  $$│ $$$ │ $$ ╱$$__  $$ ╱$$__  $$│ $$      │ $$_____╱      │ $$__  $$│ $$__  $$ ╱$$__  $$)EOF",
        R"EOF(│ $$  ╲__╱│ $$  ╲ $$│ $$$$│ $$│ $$  ╲__╱│ $$  ╲ $$│ $$      │ $$            │ $$  ╲ $$│ $$  ╲ $$│ $$  ╲__╱)EOF",
        R"EOF(│ $$      │ $$  │ $$│ $$ $$ $$│  $$$$$$ │ $$  │ $$│ $$      │ $$$$$         │ $$$$$$$╱│ $$$$$$$╱│ $$ ╱$$$$)EOF",
        R"EOF(│ $$      │ $$  │ $$│ $$  $$$$ ╲____  $$│ $$  │ $$│ $$      │ $$__╱         │ $$__  $$│ $$____╱ │ $$│_  $$)EOF",
        R"EOF(│ $$    $$│ $$  │ $$│ $$╲  $$$ ╱$$  ╲ $$│ $$  │ $$│ $$      │ $$            │ $$  ╲ $$│ $$      │ $$  ╲ $$)EOF",
        R"EOF(│  $$$$$$╱│  $$$$$$╱│ $$ ╲  $$│  $$$$$$╱│  $$$$$$╱│ $$$$$$$$│ $$$$$$$$      │ $$  │ $$│ $$      │  $$$$$$╱)EOF",
        R"EOF( ╲______╱  ╲______╱ │__╱  ╲__╱ ╲______╱  ╲______╱ │________╱│________╱      │__╱  │__╱│__╱       ╲______╱ )EOF",
        R"EOF(                                                                                                          )EOF",
        R"EOF(                                                                                                          )EOF",
        R"EOF(                                                                                                          )EOF"
    };

    // 가로 폭 110칸을 화면 중앙(150칸)에 맞추기 위해 x좌표 조정
    int startX = 20;
    // 로고가 화면 약간 위쪽에 위치하도록 y좌표 조정 (메뉴와 안 겹치게)
    int startY = 8;

    // 11줄을 한 줄씩 좌표를 밑으로 내리며(startY + i) 렌더링
    for (int i = 0; i < 11; i++)
    {
        PrintTextAt(startX, startY + i, art[i]);
    }
}