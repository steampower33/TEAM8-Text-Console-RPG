# Console RPG UI Manager

윈도우 콘솔 기반 텍스트 RPG를 위한 UI 렌더링 및 레이아웃 관리 클래스입니다. 콘솔 창의 깜빡임 현상을 방지하기 위해 전체 화면 갱신 대신 `Gotoxy`를 활용한 부분 덮어쓰기 렌더링 방식을 사용합니다.

## 주요 기능
* **해상도 고정 시스템** 150x40 해상도 강제 고정 및 입력 커서 숨김 처리
* **4분할 레이아웃 제공** Scene, Stat/Inventory, Log, Choose 패널 자동 분할
* **동적 메뉴 선택기** 방향키로 조작 가능한 범용 메뉴 UI 지원
* **스크롤링 로그** 오래된 메시지는 위로 밀려나는 실시간 전투 로그 패널 구현
* **타이틀 렌더링** 거대 아스키 아트 로고 및 시작 메뉴 내장

## 사용 가이드

### 1. 시스템 초기화 및 타이틀 화면
프로젝트 진입점 메인 함수에서 UI 시스템을 초기화하고 타이틀 화면을 띄웁니다.

```cpp
#include "UI/UIManager.h"
#include <windows.h>
#include "Character/Character.h"

int main() {
    UIManager ui;
    ui.Initialize();

    int titleResult = ui.ShowTitleMenu();

    Character* character = new Character("SIUUUUUUUU");
    
    if (titleResult == 0) {
        system("cls");
        
        ui.ShowMainFrame();
        ui.UpdateStat(character);
        ui.ShowMenuBox({ "전투", "아이템 사용" });
    }
    else if (titleResult == 1) {
        // '게임 종료'를 선택함
        return 0; 
    }

    return 0;
}
```

### 2. 패널 렌더링 및 데이터 갱신
GameManager 등 외부 컨트롤러에서 UI 객체를 호출하여 화면 데이터를 갱신합니다.
```cpp
// 스탯 갱신
// Character 객체의 포인터를 전달하면 Stat 패널에 이름, 레벨, 체력 등이 즉시 갱신됩니다.
ui.UpdateStat(player);

// 시스템 및 전투 로그 출력
// Log 패널 하단에 텍스트가 추가되며 최대 줄 수를 초과하면 위로 스크롤링됩니다.
ui.PrintLog("야생의 고블린이 나타났다!");

// 3. 범용 메뉴 선택기
// string 벡터를 전달하면 하단 Choose 패널에 메뉴를 띄우고 유저가 선택한 인덱스를 반환합니다.
std::vector<std::string> battleMenu = { "공격", "방어", "도주" };
int choice = ui.ShowMenuBox(battleMenu);
```