#include "../Character/Character.h"
#include "../Item/Inventory.h"
#include "../Item/IItem.h"
#include "../Shop/function.h"
#include "../Monster/data/MonsterTable.h"
#include "../Sound/SoundManager.h"
#include "UIManager.h"

UIManager ui;

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
    
    auto monsterList = GetMonsterTable();
    
    for (auto& m : monsterList)
    {
        if (m.second.Stats.Name == "Boss")
            continue;
        killList[m.second.Stats.Name] = 0;
    }
    
    // SoundManager::GetInstance()->PlayBGM("Assets/Sound/music.wav");
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

    SoundManager::GetInstance()->PlaySFX("Assets/Sound/keyInput.wav", "keyInput");
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
    // ShowEndingCredit();
    // int key = _getch();
    
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
            SoundManager::GetInstance()->PlaySFX("Assets/Sound/keyInput.wav", "keyInput");
            break;
        case KEY_DOWN:
        case KEY_RIGHT:
            selectedIndex++;
            if (selectedIndex >= maxMenu)
                selectedIndex = 0;
            SoundManager::GetInstance()->PlaySFX("Assets/Sound/keyInput.wav", "keyInput");
            break;
        }
        return 0; // 아직 엔터 안 침
    }
    else if (key == KEY_ENTER)
    {
        SoundManager::GetInstance()->PlaySFX("Assets/Sound/keyInput.wav", "keyInput");
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
        {"경험치 ", std::to_string(character->Experience) + " / 100"},
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

void UIManager::ClearScenePanel()
{
    int innerWidth = EndSceneX - StartSceneX - 1;
    std::string blankLine(innerWidth, ' '); // 너비만큼의 공백
    
    for (int i = StartSceneY + 1; i < EndSceneY; i++)
    {
        PrintTextAt(StartSceneX + 1, i, blankLine);
    }
}

void UIManager::UpdateScene(bool isCombat, std::string monsterName, int playerOffset, int monsterOffset, bool isMonsterDead, bool isPlayerAttacked, bool isMonsterAttacked)
{
    int playerBaseX = StartSceneX + 5; 
    
    if (!isCombat)
    {
        for (int i = 0; i < warriorIdle.size(); i++)
        {
            PrintTextAlign(playerBaseX + playerOffset, EndSceneX - 1, StartSceneY + 1 + i, warriorIdle[i], TextAlign::Left);
        }
    }
    else
    {
        for (int i = 0; i < warriorBattle.size(); i++)
        {
            std::string text = warriorBattle[i];
            if (isPlayerAttacked)
                text = Color::ORANGE + text + Color::RESET;
            PrintTextAlign(playerBaseX + playerOffset, EndSceneX - 1, StartSceneY + 1 + i, text, TextAlign::Left);
        }

        int monsterX = StartSceneX + 55 + monsterOffset; 
        int startY = StartSceneY + 1;
        int SceneHeight = EndSceneY - StartSceneY - 1;

        // [핵심] 그릴 몬스터 배열의 '주소'만 저장할 포인터 변수 준비
        const std::vector<std::string>* targetMonsterArt = nullptr;

        // 이름에 따라 어떤 배열을 가리킬지 주소만 매핑
        if (monsterName.find("Goblin") != std::string::npos) targetMonsterArt = &goblin;
        else if (monsterName.find("Orc") != std::string::npos) targetMonsterArt = &orc;
        else if (monsterName.find("Troll") != std::string::npos) targetMonsterArt = &troll;
        else if (monsterName.find("Slime") != std::string::npos) targetMonsterArt = &slime;
        else if (monsterName.find("Boss") != std::string::npos) targetMonsterArt = &boss;

        // 매핑된 배열이 있다면, 렌더링 로직은 여기서 딱 한 번만 실행!
        if (targetMonsterArt != nullptr)
        {
            for (int i = 0; i < std::min(int(targetMonsterArt->size()), SceneHeight); i++)
            {
                // 포인터 역참조(*)로 배열 데이터 가져오기
                std::string text = (*targetMonsterArt)[i];
                
                // 몬스터가 죽었다면 시뻘겋게 칠하기
                if (isMonsterDead)
                    text = Color::RED + text + Color::RESET;
                if (isMonsterAttacked)
                    text = Color::ORANGE + text + Color::RESET;
                
                PrintTextAt(monsterX, startY + i, text);
            }
        }
    }
}

void UIManager::AnimateStrike(bool isPlayerAttacking, std::string monsterName, bool isMonsterDead, bool isPlayerAttacked, bool isMonsterAttacked)
{
    // 진동 패턴: 우로 3칸 -> 좌로 3칸 -> 우로 2칸 -> 좌로 2칸 -> 원위치
    // 콘솔 특성상 2~3칸만 움직여도 굉장히 역동적으로 보입니다.
    int shakePattern[] = { 3, -3, 2, -2, 0 }; 

    // 타격 효과음 재생 연동 (있다면)
    // SoundManager::GetInstance()->PlaySFX("Assets/Sound/hit.wav", "hit");

    for (int offset : shakePattern)
    {
        // 이전 프레임의 잔상 완벽 제거
        ClearScenePanel(); 
        
        // 오프셋을 적용해 새로운 프레임 렌더링
        if (isPlayerAttacking)
        {
            // 플레이어가 때림 -> 몬스터가 피격당해 흔들림
            UpdateScene(true, monsterName, 0, offset, isMonsterDead, isPlayerAttacked, isMonsterAttacked);
        }
        else
        {
            // 몬스터가 때림 -> 플레이어가 피격당해 흔들림
            UpdateScene(true, monsterName, offset, 0, isMonsterDead, isPlayerAttacked, isMonsterAttacked);
        }
        
        // 인간의 눈이 진동을 인식할 수 있도록 프레임 딜레이 부여 (히트스톱 효과 겸용)
        Sleep(40); 
    }
}
void UIManager::UpdateKillList(std::string monsterName)
{
    if (monsterName.size() != 0)
    {
        killList[monsterName]++;
    }
    int h = 0;
    for (auto& m : killList)
    {
        std::string name = m.first;
        int nameWidth = GetDisplayWidth(name);
    
        int paddingSize = 10 - nameWidth;
    
        if (paddingSize < 0) paddingSize = 0; 

        std::string padding(paddingSize, ' ');

        std::string text = name + padding + " x " + std::to_string(m.second);
        PrintTextAt(StartKillX + 4, StartKillY + 2 +  h, text);
        h++;
    }
}

// std::string sysLog = "\033[33m[📢 시스템] 플레이어의 턴입니다.\033[0m";
// std::string attackLog = "\033[31m[⚔️ 전투] 고블린에게 15의 치명타 피해!\t\033[0m";
// std::string itemLog = "\033[32m[🎁 획득] 낡은 롱소드를 얻었습니다.\t\033[0m";
// PrintLog(sysLog);
// PrintLog(attackLog);
// PrintLog(itemLog);
void UIManager::PrintLog(const std::string& message, int delay)
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
    Sleep(delay);
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
    if (items.empty()) return;
    for (int i = 0; i < items.size(); i++)
    {
        IItem* item = items[i];
        std::string name = item->GetName();

        int nameWidth = GetDisplayWidth(name);
    
        int paddingSize = 15 - nameWidth;
    
        if (paddingSize < 0) paddingSize = 0; 

        std::string padding(paddingSize, ' ');

        std::string text = name + padding + " x " + std::to_string(item->count);
    
        PrintTextAt(StartInventoryX + 4, StartInventoryY + 2 + i, text);
    }
}

void UIManager::ShowShop(Character* character)
{
    UpdateInventory(&character->CharacterInventory);
    UpdateStat(character);
    
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
            std::vector<ShopItems> keyList;
            for (auto i = ShopItemTable.begin(); i != ShopItemTable.end(); i++)
            {
                menuList.push_back(i->second.name + "(" + std::to_string(i->second.gold) + ")");
                keyList.push_back(i->first);
            }
            menuList.push_back("뒤로가기");
            int choose = ShowMenuAlign(startX + 2 + actMenuWidth + 2, endX - 1, startY + 2, menuList, 2, TextAlign::Center);
            
            if (choose == -1)
            {
                break;
            }
            else if (choose != menuList.size() - 1)
            {
                int buyOrNot = ShowMenuAt({startX + 2 + actMenuWidth + 5, endY - 2}, {"구매", "취소"}, 2, false);
                if (buyOrNot == 0)
                {
                    ShopItems selectedKey = keyList[choose];
                    std::string itemName = ShopItemTable.at(selectedKey).name;
                    
                    bool buy = BuyItem(selectedKey, character);
                    if (buy)
                    {
                        SoundManager::GetInstance()->PlaySFX("Assets/Sound/keys_jingling.wav", "ItemBuy");
                        PrintLog("\033[32m[아이템]\033[0m " + itemName + "을 구매했습니다.", 0);
                    }
                    else
                    {
                        PrintLog("\033[32m[아이템]\033[0m 골드가 부족해서 " + itemName + "을 구매하지못했습니다.");
                    }
                    UpdateInventory(&inventory);
                    UpdateStat(character);
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
                std::string itemName = items[i]->GetName();
                int itemGold = items[i]->GetGold();
                if (itemName == HEALTH_POTION || itemName == ATTACK_BOOST)
                    itemGold *= 0.6;
                menuList.push_back(items[i]->GetName() + "(" + std::to_string(itemGold) + ")");
            }
            menuList.push_back("뒤로가기");
            int choose = ShowMenuAlign(startX + 2 + actMenuWidth + 2, endX - 1, startY + 2, menuList, 2, TextAlign::Center);
            
            if (choose == -1)
            {
                break;
            }
            else if (choose != menuList.size() - 1)
            {
                int sellOrNot = ShowMenuAt({startX + 2 + actMenuWidth + 5, endY - 2}, {"판매", "취소"}, 2, false);
                if (sellOrNot == 0)
                {
                    std::string itemName = items[choose]->GetName();
                    bool sell = SellItem(choose, character);
                    if (sell)
                    {
                        SoundManager::GetInstance()->PlaySFX("Assets/Sound/coins_gather_quick.wav", "ItemSell");
                        PrintLog("\033[32m[아이템]\033[0m " + itemName + " 을 판매했습니다.", 0);
                    }
                    UpdateInventory(&inventory);
                    UpdateStat(character);
                }
            }
            break;
        }
        case -1:
        case 2:
        {
            loop = false;
            break;
        }
        }
    }
}

void UIManager::ShowPlayerDead()
{
    ClearScenePanel();
    
    std::vector<std::string> art = {
        R"EOF( ╱$$     ╱$$ ╱$$$$$$  ╱$$   ╱$$       ╱$$$$$$$  ╱$$$$$$ ╱$$$$$$$$ ╱$$$$$$$ )EOF",
        R"EOF(│  $$   ╱$$╱╱$$__  $$│ $$  │ $$      │ $$__  $$│_  $$_╱│ $$_____╱│ $$__  $$)EOF",
        R"EOF( ╲  $$ ╱$$╱│ $$  ╲ $$│ $$  │ $$      │ $$  ╲ $$  │ $$  │ $$      │ $$  ╲ $$)EOF",
        R"EOF(  ╲  $$$$╱ │ $$  │ $$│ $$  │ $$      │ $$  │ $$  │ $$  │ $$$$$   │ $$  │ $$)EOF",
        R"EOF(   ╲  $$╱  │ $$  │ $$│ $$  │ $$      │ $$  │ $$  │ $$  │ $$__╱   │ $$  │ $$)EOF",
        R"EOF(    │ $$   │ $$  │ $$│ $$  │ $$      │ $$  │ $$  │ $$  │ $$      │ $$  │ $$)EOF",
        R"EOF(    │ $$   │  $$$$$$╱│  $$$$$$╱      │ $$$$$$$╱ ╱$$$$$$│ $$$$$$$$│ $$$$$$$╱)EOF",
        R"EOF(    │__╱    ╲______╱  ╲______╱       │_______╱ │______╱│________╱│_______╱ )EOF",
    };

    int boxWidth = EndSceneX - StartSceneX - 1;
    int boxHeight = EndSceneY - StartSceneY - 1;
    int startX = (ConsoleWidth - boxWidth) / 2;
    int startY = (ConsoleHeight - boxHeight) / 2;
    for (int i = 0; i < art.size(); i++)
    {
        std::string text = "\033[31m" + art[i] + "\033[0m";
        PrintTextAt(startX, startY + i, text);
    }
}

void UIManager::ShowEndingCredit()
{
    system("cls");
    
    int boxWidth = 60;
    int boxHeight = ConsoleHeight - 1;
    int startX = (ConsoleWidth - boxWidth) / 2;
    int startY = (ConsoleHeight - boxHeight) / 2;
    int endX = startX + boxWidth;
    int endY = startY + boxHeight;
    DrawBox(startX, startY, endX, endY);
    std::vector<std::string> endLog = {
        "=== Development Team ===", // 혹은 === Programmers ===
        "",
        "GameLoop       : 조민웅",
        "Item/Inventory : 이인구",
        "Monster/Shop   : 김선우",
        "Character      : 김진우",
        "UI/Sound       : 이승민",
        "", 
        "",
        "=== Developer Comments ===",
        "",
        "[조민웅]",
        "  재미있는 시간이었습니당 클로드야 고마워!",
        "",
        "[이인구]",
        "  고생 많으셨습니다, 고수님들 덕분에 꿀 빨다 갑니다.",
        "  감사합니다.",
        "",
        "[김선우]",
        "  웅ㅇ애 CPP 어려웡 ",
        "",
        "[김진우]",
        "  재밌있는 시간이었습니다. 덕분에 많이 배워 갑니다.",
        "",
        "[이승민]",
        "  재밌었슴다! 다들 고생많으셨습니다!",
        "",
        "",
        "=== Special Thanks to ===",
        "",
        "  [ Development Tools ]",
        "    - Rider (Main Editor)",
        "    - GitHub (Version Control)",
        "",
        "  [ AI Assistants ]",
        "    - Gemini",
        "    - Claude",
        "    - ChatGPT",
        "",
        "  [ Collaboration & Docs ]",
        "    - Notion (Documentation)",
        "    - Gamma (Presentation)",
        "",
        "",
        "  [ Audio Assets ]",
            "    - Minifantasy Dungeon by Leohpaz",
            "    - 400 Sounds Pack by Chequered Ink",
            "", 
            "", 
            "", 
            "", 
            "", 
        "플레이해주셔서 감사합니다.....!!"
    };
    
    int idx = 1; // 1부터 시작해야 첫 줄이 바로 계산됨
    while (idx <= endLog.size())
    {
        // 박스 내부 여백 깔끔하게 지우기
        int innerWidth = endX - startX - 1;
        std::string blankLine(innerWidth, ' ');
        for (int i = startY + 1; i < endY; i++)
        {
            PrintTextAt(startX + 1, i, blankLine);
        }

        // 크레딧 한 줄씩 출력 (스크롤 로직)
        for (int i = 0; i < idx; i++)
        {
            std::string line = std::string("  ") + endLog[i];
            
            int currentY = endY - 1 - (idx - i);
            
            if (currentY > startY) 
            {
                PrintTextAt(startX + 2, currentY, line);
            }
        }
        
        // 스크롤 속도 제어 (1000 = 1초)
        Sleep(500); // 0.8초마다 한 줄씩 올라감 (원하는 감성에 맞춰 조절하세요)
        
        idx++;
    }
    
    Sleep(2000); 
    
    // 아무 키나 누르면 타이틀로 돌아가는 등의 후속 로직을 여기에 작성하면 됩니다.
}

// 이제 성공 여부(bool)가 아니라, 데미지 배율(float)을 반환합니다.
float UIManager::ShowTimingGauge()
{
    int x = StartSceneX + 15; 
    int y = StartSceneY + 20;
    
    int barLength = 30;
    
    std::random_device rd;
    std::mt19937 gen(rd());

    // 퍼펙트 존(2.0배)의 위치를 랜덤으로 설정합니다. 
    std::uniform_int_distribution<int> centerDist(4, barLength - 5);
    int centerIndex = centerDist(gen);

    // 커서 이동 속도 랜덤 설정
    std::uniform_int_distribution<int> speedDist(5, 35);
    int currentSpeed = speedDist(gen);
    
    int cursor = 0;           
    int direction = 1;        
    float multiplier = 0.0f; // 최종 데미지 배율

    while (_kbhit()) _getch();

    while (true)
    {
        // ==========================================
        // 렌더링: 현재 상태를 화면에 먼저 확실하게 그립니다.
        // ==========================================
        std::string bar = "[";
        for (int i = 0; i < barLength; i++)
        {
            int d = std::abs(i - centerIndex);
            std::string cellText = (i == cursor) ? "╋" : " ";

            if (d == 0) 
                bar += "\033[1;93;41m" + cellText + "\033[0m"; // 2.0배
            else if (d == 1) 
                bar += "\033[1;97;43m" + cellText + "\033[0m"; // 1.5배
            else if (d == 2) 
                bar += "\033[1;97;42m" + cellText + "\033[0m"; // 1.0배
            else if (d == 3) 
                bar += "\033[1;97;44m" + cellText + "\033[0m"; // 0.5배
            else 
                bar += (i == cursor) ? "\033[1;93m╋\033[0m" : "-"; // 0.0배
        }
        bar += "]";
        PrintTextAt(x, y, bar);

        // ==========================================
        // 마이크로 폴링(Polling): 렌더링된 화면 그대로 입력을 대기합니다.
        // ==========================================
        bool isPressed = false;
        
        // currentSpeed를 통째로 Sleep하지 않고, 5ms 단위로 쪼개서 키보드를 감시합니다.
        for (int wait = 0; wait < currentSpeed; wait += 5)
        {
            if (_kbhit())
            {
                int key = _getch();
                if (key == KEY_ENTER || key == 32)
                {
                    isPressed = true;
                    break; // 감지 즉시 for문 탈출
                }
            }
            Sleep(5); // 5ms 대기
        }

        // ==========================================
        // 판정 및 상태 업데이트
        // ==========================================
        if (isPressed)
        {
            // 방금 화면에 띄워둔 cursor 값을 기준으로 정확하게 판정!
            int distance = std::abs(cursor - centerIndex);
            multiplier = 2.0f - (distance * 0.5f);
            
            if (multiplier <= 0.0f) 
                multiplier = 0.0f;
            
            break; // 미니게임 루프(while) 탈출
        }

        // 아무도 키를 누르지 않았다면 비로소 커서를 다음 칸으로 이동시킵니다.
        cursor += direction;
        if (cursor <= 0 || cursor >= barLength - 1)
        {
            direction *= -1; 
        }
    }

    // 결과 처리 및 로그 출력
    // float는 정확한 == 2.0f 비교보다 >= 2.0f 이 안전합니다.
    if (multiplier >= 2.0f) {
        PrintLog("\033[1;31m[PERFECT] 완벽한 일격! (데미지 200%)\033[0m");
    } else if (multiplier > 0.0f) {
        std::string multStr = std::to_string(multiplier).substr(0, 3); 
        PrintLog("\033[32m[HIT] 공격 적중! (데미지 " + multStr + "배)\033[0m");
    } else {
        SoundManager::GetInstance()->PlaySFX("Assets/Sound/8_bit_negative_quick.wav", "PlayerAttackMiss");
        PrintLog("\033[90m[MISS] 헛스윙을 했습니다...\033[0m");
    }

    Sleep(500); 
    
    std::string blankBar(barLength + 2, ' ');
    PrintTextAt(x, y, blankBar);

    return multiplier;
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
    DrawKillListPanel();
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
    PrintTextAt(StartSceneX + 5, StartLogY, line);
}

void UIManager::DrawKillListPanel()
{
    DrawBox(StartKillX, StartKillY, EndKillX, EndKillY);

    std::string line = "Kill List";
    PrintTextAt(StartKillX + 5, StartKillY, line);
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