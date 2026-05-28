# 🎮 Console RPG UI Manager

윈도우 콘솔 기반 텍스트 RPG를 위한 UI 렌더링 및 레이아웃 관리 프레임워크입니다. 콘솔 창의 깜빡임 현상을 방지하기 위해 전체 화면 갱신(`system("cls")`)을 최소화하고, 메모리 상에서 문자열을 조립한 뒤 `Gotoxy`를 활용해 필요한 좌표에만 덮어쓰는 최적화된 부분 렌더링 아키텍처를 사용합니다.

## ✨ 주요 특징 (Features)

* **해상도 및 인코딩 강제 고정**
* `150 x 40` 해상도로 콘솔 창 크기를 강제 고정하고 입력 커서를 숨깁니다.
* `CP_UTF8`을 적용하여 아스키 아트 및 테두리 선 깨짐을 방지하며, 내부적으로 한글(다중 바이트)과 영어(단일 바이트)의 **시각적 넓이(Display Width)를 정밀하게 계산**하여 화면 틀어짐을 원천 차단합니다.


* **5분할 레이아웃 시스템**
* 하드코딩된 좌표가 아닌, 화면 비율에 맞춘 `Scene`, `Stat`, `Inventory`, `Log`, `Kill List` 5개 패널 구역을 자동으로 분할하고 관리합니다.


* **동적 연출 및 미니게임 지원**
* 타격 시 화면 흔들림(Screen Shake)과 역경직(Hit-stop) 효과를 모방한 `AnimateStrike` 시스템을 지원합니다.
* 리듬 게임 방식의 QTE(Quick Time Event) 데미지 배율 시스템(`ShowTimingGauge`)이 내장되어 있습니다.


* **ANSI 색상 제어**
* 하드코딩된 색상 코드 대신 `Color::RED`, `Color::GREEN` 등 직관적인 네임스페이스를 통해 텍스트에 다채로운 속성과 시각적 피드백을 제공합니다.

---

## 📚 API 문서 (API Documentation)

### 🖥️ 1. 패널 갱신 (Panel Updates)

화면 각 구역의 데이터를 갱신합니다. 전체 화면을 지우지 않고 해당 구역의 내부 공백만 지운 뒤 덮어씌웁니다.

* **`void UpdateScene(bool isCombat, std::string monsterName, ...)`**
* 메인 씬(좌측 상단)을 갱신합니다.
* 비전투 시: `ui.UpdateScene(false, "");` (기본 플레이어 아트 출력)
* 전투 시: `ui.UpdateScene(true, "Goblin");` (해당 몬스터의 아트 출력)


* **`void UpdateStat(Character* character)`**
* 캐릭터의 현재 스탯(체력, 골드, 공격력 등)을 Stat 패널에 즉시 갱신합니다.


* **`void UpdateInventory(Inventory* inven)`**
* 인벤토리 패널을 갱신합니다.


* **`void UpdateKillList(std::string monsterName)`**
* 처치한 몬스터의 수를 카운트하고 우측 하단 패널에 렌더링합니다.



### 📜 2. 로그 시스템 (Log System)

* **`void PrintLog(const std::string& message, int delay = 300)`**
* Log 패널(좌측 하단)에 메시지를 추가합니다.
* 내부적으로 줄 수를 계산하여 오래된 로그는 자동으로 위로 밀려나며 삭제됩니다.
* `delay` 매개변수로 메시지 출력 후 대기 시간을 밀리초(ms) 단위로 조절할 수 있습니다.
* *Tip:* `Color::` 네임스페이스를 조합하여 사용하세요. (예: `ui.PrintLog(Color::RED + "[경고] 피격당했습니다!" + Color::RESET);`)



### 🎬 3. 전투 연출 및 미니게임 (Combat & Effects)

* **`void AnimateStrike(bool isPlayerAttacking, std::string monsterName, ...)`**
* 텍스트 아트를 좌우로 진동시켜 역동적인 타격감을 연출합니다.
* 인자에 따라 플레이어가 피격당해 흔들릴지, 몬스터가 피격당해 흔들릴지 결정합니다.


* **`float ShowTimingGauge()`**
* 전투 시 데미지 배율을 결정하는 QTE 미니게임을 화면에 띄웁니다.
* 스페이스바나 엔터로 타이밍을 맞추면 위치에 따라 `0.0f` ~ `1.2f` 사이의 데미지 배율을 반환합니다.



### 🕹️ 4. 메뉴 시스템 (Menu System)

사용자의 키보드 입력(방향키, 엔터)을 처리하여 직관적인 메뉴를 구성합니다.

* **`int ShowMenuAlign(int startX, int endX, int y, const std::vector<std::string>& menuList, int step, TextAlign textAlign)`**
* 화면의 특정 가로 범위 내에서 정렬(좌, 우, 중앙)된 메뉴를 출력하고 선택된 인덱스를 반환합니다.


* **`int ShowMenuAt(Vec2 at, const std::vector<std::string>& menuList, int step, bool isVertical)`**
* 지정된 특정 좌표(`at.x`, `at.y`)에서 메뉴를 출력합니다. 가로/세로 배치를 지원합니다.