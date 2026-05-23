# Console RPG UI Manager

윈도우 콘솔 기반 텍스트 RPG를 위한 UI 렌더링 및 레이아웃 관리 클래스입니다. 콘솔 창의 깜빡임 현상을 방지하기 위해 전체 화면 갱신(`system("cls")`)을 최소화하고, `Gotoxy`를 활용해 필요한 좌표의 텍스트만 부분 덮어쓰는 최적화된 렌더링 방식을 사용합니다.

## ✨ 주요 특징 (Features)

* **해상도 및 인코딩 강제 고정**
* `150 x 40` 해상도로 콘솔 창 크기를 강제 고정하고 입력 커서를 숨깁니다.
* `CP_UTF8`을 적용하여 한글 및 특수기호(아스키 아트, 테두리 선) 깨짐 현상을 방지합니다.


* **5분할 레이아웃 시스템**
* 하드코딩된 좌표가 아닌, 화면 비율에 맞춘 `Scene`, `Stat`, `Inventory`, `Log`, `Choose` 5개 패널의 구역을 자동으로 분할하고 테두리를 렌더링합니다.


* **동적 키보드 메뉴 선택기 (`ShowMenuAt`)**
* 상/하 방향키로 조작 가능한 범용 메뉴 UI를 제공합니다.
* 원하는 좌표(`Vec2`), 메뉴 리스트, 간격, 가로/세로 모드를 인자로 넘기면 사용자의 선택 결과값(Index)을 정수로 반환합니다.


* **실시간 스크롤링 로그 (`PrintLog`)**
* 전투 및 이벤트 상황을 출력하는 로그 패널입니다.
* 지정된 높이를 초과하여 로그가 쌓이면, 가장 오래된 메시지부터 위로 밀어내며 삭제하는 큐(Queue) 방식의 스크롤링을 지원합니다.


* **인벤토리 및 아이템 상호작용**
* 인벤토리 목록을 렌더링하고, 아이템 선택 시 '사용/취소'를 묻는 서브 메뉴 팝업 로직이 내장되어 있습니다.



---

## 🚀 시작하기 (Quick Start)

가장 최근 버전의 UI Manager가 적용된 게임 메인 루프 예제입니다.

```cpp
#include "UI/UIManager.h"
#include "Character/Character.h"
#include "Item/Inventory.h"
#include "Item/AttackBoost.h"
#include "Item/HealthPotion.h"

#include <windows.h>

int main() {
    UIManager ui;
    ui.Initialize(); // 필수: 콘솔 창 설정 및 UTF-8 세팅

    // 타이틀 화면 렌더링 (0: 시작, 1: 종료)
    int titleResult = ui.GetTitleResult();

    Character* character = new Character("SIUUUUUUUU");
    Inventory* inventory = new Inventory();
    
    inventory->AddItem(std::make_unique<HealthPotion>());
    inventory->AddItem(std::make_unique<AttackBoost>());
    
    if (titleResult == 0) {
        int loop = 1;
        while (loop)
        {
            // 루프 시작 시 화면 초기화 및 전체 프레임 렌더링
            system("cls");
            ui.ShowMainFrame();
            ui.UpdateStat(character);
            ui.PrintLog();
            ui.UpdateInventory(inventory);
        
            // 플레이어 행동 선택
            int choose = ui.GetMainResult();
            switch (choose)
            {
                case -1: // ESC 입력
                    loop = 0; break;
                case 0:  // 전투 선택
                    break;
                case 1:  // 아이템 사용 선택
                    ui.ChooseItem(inventory, character);
                    break;
            }
        }
    }
    
    return 0;
}

```

---

## 🛠️ 주요 API 가이드 (팀원 참고용)

팀원들이 게임 컨텐츠(몬스터, 아이템, 전투 로직)를 개발할 때 직접 호출하게 될 핵심 함수들입니다.

### 1. 화면 제어 및 출력

* **`void Initialize()`**
* 프로그램 시작 시 `main` 함수 최상단에서 무조건 1회 호출해야 합니다.


* **`void PrintLog(const std::string& message)`**
* 시스템 메시지, 획득 알림, 전투 결과 등을 좌측 하단 Log 패널에 출력합니다.
* 빈 문자열 `""`을 전달하면, 기존에 저장된 로그 배열을 화면에 다시 새로고침하여 그려줍니다.



### 2. 메뉴 시스템

* **`int ShowMenuAt(Vec2 at, const std::vector<std::string>& menuList, int step = 2, bool isVertical = true)`**
* **UIManager의 핵심 기능입니다.** * 화면 내 특정 좌표(`at`)에 원하는 메뉴 목록(`menuList`)을 띄우고 방향키 입력을 받습니다.
* Enter 키를 누르면 선택한 메뉴의 인덱스(0, 1, 2...)를 반환하며, ESC 키를 누르면 `-1`을 반환합니다.
* 가로 배치(`isVertical = false`)와 세로 줄바꿈 간격(`step`) 조절이 가능합니다.



### 3. 데이터 연동

* **`void UpdateStat(Character* character)`**
* 캐릭터의 객체를 넘겨받아 체력, 공격력, 레벨 등의 스탯을 Stat 패널에 최신화합니다.


* **`void UpdateInventory(Inventory* inven)`**
* 인벤토리 객체를 넘겨받아 보유 중인 아이템 목록을 Inventory 패널에 출력합니다.


* **`void ChooseItem(Inventory* inven, Character* character)`**
* 인벤토리 패널 내에서 아이템을 선택하고, 실제로 사용(UseItem) 처리까지 수행하는 래퍼(Wrapper) 함수입니다.



---

## 🚨 주의사항 (Troubleshooting)

**루프 중간에 `system("cls")` 사용을 엄격히 금지합니다.**

* UI Manager는 테두리(Box)와 고정 패널들을 최초에 한 번(`ShowMainFrame()`) 그리고, 이후에는 `Gotoxy`를 이용해 내용물(숫자, 글자)만 덮어씌우도록 설계되었습니다.
* 만약 플레이어의 턴이나 전투 중간에 `system("cls")`를 호출하면 애써 그린 5분할 테두리가 모두 날아갑니다. 화면을 지워야 한다면 메인 루프의 최상단(프레임 시작점)에서만 수행해 주세요.

---