# Console RPG UI Manager

윈도우 콘솔 기반 텍스트 RPG를 위한 UI 렌더링 및 레이아웃 관리 프레임워크입니다. 콘솔 창의 깜빡임 현상을 방지하기 위해 전체 화면 갱신(`system("cls")`)을 최소화하고, 메모리 상에서 문자열을 조립한 뒤 `Gotoxy`를 활용해 필요한 좌표에만 덮어쓰는 최적화된 렌더링 아키텍처를 사용합니다.

## ✨ 주요 특징 (Features)

* **해상도 및 인코딩 강제 고정**
  * `150 x 40` 해상도로 콘솔 창 크기를 강제 고정하고 입력 커서를 숨깁니다.
  * `CP_UTF8`을 적용하여 아스키 아트 및 테두리 선 깨짐을 방지하며, 내부적으로 한글(3바이트)과 영어(1바이트)의 **시각적 넓이(Display Width)를 정밀하게 계산**하여 화면 틀어짐을 원천 차단합니다.

* **4분할 레이아웃 시스템**
  * 하드코딩된 좌표가 아닌, 화면 비율에 맞춘 `Scene`, `Stat`, `Inventory`, `Log` 4개 패널의 구역을 자동으로 분할하고 테두리를 렌더링합니다.

---

## 🚀 시작하기 (Quick Start)

가장 최근 버전의 UI Manager가 적용된 게임 메인 루프 예제입니다.

```cpp
#include <iostream>
#include <conio.h>
#include <random>

#include "Character/Character.h"
#include "Item/Inventory.h"
#include "Item/AttackBoost.h"
#include "Item/HealthPotion.h"
#include "UI/UIManager.h"

#include "Monster/utils/Create.h" 
#include "Monster/Monster.h"

int main() {
    UIManager ui;
    ui.Initialize();

    // 난수 생성기 세팅 (아이템 자동 사용 30% 등)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> chance30(1, 100);

    int titleResult = ui.GetTitleResult();
    
    if (titleResult == 0) {
        std::string name = ui.ShowCharacterGeneration();
        
        // 싱글톤으로 캐릭터 인스턴스 획득
        Character* character = Character::GetInstance(name);
    
        // 시작 아이템을 캐릭터 내부 인벤토리에 지급
        character->CharacterInventory.AddItem(std::make_unique<HealthPotion>());
        character->CharacterInventory.AddItem(std::make_unique<AttackBoost>());
        
        bool isPlaying = true;

        while (isPlaying)
        {
            // --- [1. 탐색 모드 (Idle)] ---
            system("cls");
            ui.ShowMainFrame();
            ui.UpdateStat(character);
            ui.UpdateInventory(&character->CharacterInventory);
            
            ui.UpdateScene(); 
            
            ui.PrintLog("\033[36m[탐험]\033[0m 미궁을 걷고 있습니다... [Enter] 전진 (ESC 종료)");
            
            int key = _getch();
            if (key == KEY_ESC) {
                isPlaying = false;
                break;
            }
            if (key != KEY_ENTER) continue; 

            // --- [2. 전투 모드 진입] ---
            std::unique_ptr<Monster> monster = CreateRandomMonster(character->Level);
            MonsterStats mStats = monster->GetStatus();

            ui.PrintLog("\033[31m[새로운 전투!]\033[0m");
            ui.PrintLog("\033[31m[전투 발생!]\033[0m 야생의 " + mStats.Name + "이(가) 나타났다!");
            
            // 이번 전투에서만 쓰일 임시 공격력 버프
            int tempAttackBuff = 0; 
            bool inCombat = true;
            while (inCombat)
            {
                system("cls");
                ui.ShowMainFrame();
                ui.UpdateStat(character);
                ui.UpdateInventory(&character->CharacterInventory);
                
                // isCombat = true 전달
                ui.UpdateScene(true, mStats.Name);

                ui.PrintLog("\033[33m[전투 대기]\033[0m [Enter]를 누르면 턴이 진행됩니다.");
                
                int combatKey = _getch();
                if (combatKey != KEY_ENTER) continue;

                // --- [턴 연산 시작] ---
                
                // 1) 플레이어의 아이템 자동 사용 (30% 확률)
                int itemCount = character->CharacterInventory.GetItems().size();
                if (chance30(gen) <= 30 && itemCount > 0) {
                    
                    // 가지고 있는 아이템 중 랜덤으로 하나 선택
                    std::uniform_int_distribution<int> itemDist(0, itemCount - 1);
                    int randomIndex = itemDist(gen);
                    
                    // 주의: 아이템이 '해당 전투만 공격력 +10'이라면, 
                    // AttackBoost 아이템 내부에서 character->Attack을 직접 올리지 마시고,
                    // tempAttackBuff += 10; 으로 처리되도록 구조를 맞춰야 영구 스탯 뻥튀기 버그가 안 납니다.
                    character->UseItem(randomIndex); 
                    
                    ui.PrintLog("\033[32m[아이템]\033[0m " + character->Name + "이(가) 가방에서 아이템을 사용했습니다!");
                }

                // 2) 플레이어 -> 몬스터 공격
                int pAtk = character->Attack + tempAttackBuff; 
                bool isDead = monster->TakeDamageWithIsDead(pAtk);
                
                ui.PrintLog(character->Name + "의 공격! " + mStats.Name + "에게 피해를 입혔습니다.");

                // 몬스터 사망 체크 및 보상
                if (isDead) {
                    MonsterReward reward = monster->GetReward();
                    
                    // Character.h에 멤버 변수가 public이므로 직접 연산
                    character->Experience += reward.Exp; 
                    character->Gold += reward.Item.gold;
                    
                    ui.PrintLog("\033[36m[전투 승리]\033[0m " + mStats.Name + " 처치! (EXP +" + std::to_string(reward.Exp) + " / Gold +" + std::to_string(reward.Item.gold) + ")");
                    
                    // 경험치 100 달성시 내부에서 레벨업 처리됨
                    character->LevelUp(); 
                    
                    inCombat = false;
                    continue; 
                }

                // 3) 몬스터 -> 플레이어 반격
                mStats = monster->GetStatus(); 
                character->TakeDamage(mStats.ATK); 
                
                ui.PrintLog("\033[31m[반격]\033[0m " + mStats.Name + "의 맹공! " + std::to_string(mStats.ATK) + "의 피해!");

                // 4) 플레이어 사망 체크
                if (character->IsDead) {
                    ui.PrintLog("\033[31m[사망]\033[0m " + character->Name + "이(가) 쓰러졌습니다... 게임 오버!");
                    
                    ui.PrintLog("\033[31m[게임 종료]\033[0m [Enter]를 누르면 게임이 종료됩니다.");
                
                    int exitKey = _getch();
                    if (exitKey != KEY_ENTER) continue;
                    inCombat = false;
                    isPlaying = false; 
                }
            } // -- 전투 루프(While inCombat) 종료 --
            
        } // -- 메인 루프(While isPlaying) 종료 --
        
        // 싱글톤 메모리 해제는 프로그램 종료 전 한 번만 처리
        // (구현 방식에 따라 delete Character::Instance; 등 필요)
    }

    return 0;
}

```
---

팀원들이 이 UI 시스템을 원활하게 사용할 수 있도록, [Console UI Management System 문서]를 작성해 드립니다. 이 내용을 README.md나 팀 위키(Notion 등)에 붙여넣으면 팀원들이 훨씬 편하게 개발할 수 있을 겁니다.

---

# 🎮 Console UI Management System 문서

이 시스템은 텍스트 RPG를 위한 효율적인 UI 렌더링 프레임워크입니다. `system("cls")`를 남발할 때 발생하는 화면 깜빡임(Flickering) 현상을 방지하기 위해 **좌표 기반 덮어쓰기 렌더링 방식**을 사용합니다.

## 🚀 1. 시스템 초기화 및 시작

게임의 `main` 함수 가장 앞단에서 반드시 호출해야 합니다. 콘솔 창 크기(150x40), 커서 숨기기, UTF-8 인코딩 설정을 담당합니다.

```cpp
UIManager ui;
ui.Initialize();

```

## 🖥️ 2. 주요 렌더링 API

### 🎨 `void UpdateScene(bool isCombat, std::string monsterName)`

화면 왼쪽의 메인 씬을 갱신합니다.

* **비전투 시:** `ui.UpdateScene(false);`를 호출하여 Warrior 를 출력합니다.
* **전투 시:** Warrior 를 출력 및 `ui.UpdateScene(true, "Goblin");`처럼 호출하여 몬스터 이름에 맞는 아트를 출력합니다. (내부적으로 아트가 매칭되어 있습니다.)

### 📊 `void UpdateStat(Character* character)`

캐릭터의 현재 정보를 Stat 패널에 즉시 갱신합니다. `Character` 객체의 포인터를 인자로 넘겨주세요.

### 📜 `void PrintLog(const std::string& message = "")`

Log 패널에 메시지를 추가합니다.

* 메시지를 생략하고 호출(`ui.PrintLog()`)하면, 기존 로그를 화면에 새로고침(Refresh)합니다.
* 문자열을 인자로 넘기면(예: `ui.PrintLog("공격 성공!");`) 로그가 추가되고 화면이 갱신됩니다.

### 🎒 `void UpdateInventory(Inventory* inven)`

인벤토리 패널을 갱신합니다. `Inventory` 객체의 포인터를 인자로 넘겨주세요.

---

## ⚠️ 필수 개발 가이드 (주의사항)

### 1. `system("cls")` 사용 금지

본 UI 프레임워크는 덮어쓰기 방식으로 동작하므로, **게임 루프 내부에서 `system("cls")`를 호출하면 UI 테두리와 고정 패널이 전부 날아갑니다.**

* **허용 구간:** 게임 시작 전, 캐릭터 생성 등 완전히 새로운 화면으로 전환될 때만 사용 가능.
* **금지 구간:** `while(loop)` 안에서 매 턴마다 호출하는 것은 절대 금지.

### 2. 전투 루프 로직 권장 구조

화면 깜빡임 없이 부드러운 UI 갱신을 위해 아래와 같은 순서를 권장합니다.

```cpp
// 1. 프레임 전체 틀 그리기 (박스 생성)
ui.ShowMainFrame();

// 2. 데이터 갱신
ui.UpdateStat(character);
ui.UpdateInventory(&character->CharacterInventory);

// 3. 상태에 따른 씬 갱신
ui.UpdateScene(true, "Goblin"); // 전투 중일 때

// 4. 로그 출력
ui.PrintLog("메시지 입력"); 

```

---
