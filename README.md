### README

- **소개**
    - Auto Battler Text RPG
    - 어떻게 하면 플레이어가 TEXT RPG 감성을 조금이라도 느낄수있을까 에 초점을 맞춤

- **주요 특징**
    - 한 화면에서 창을 분할해서, 다양한 정보를 한눈에 볼수있도록 설정
    - GameManager를 중심으로 각 시스템들(Item, Inventory, Character, Monster, UI, Shop, Sound)이 유기적으로 돌아갈수있도록 구현
- **주요 게임 사례 및 시연 영상**
    - 게임의 주요 장면이나 기능을 보여주는 시연 영상 (진행중)

      메인 게임 루프

      탐색 → 전투 → 승리시 상점

      플레이어 죽음

      후반부 보스전

      엔딩 - Thanks to ~~~

- **게임 개발 과정**
    - 노션의 다이어그램 예시대로 파트를 나눔
    - 그러다 GameLog 및 UI 필요하다 판단되어 UIManager를 설정
    - GameManager에서 전투를 담당하는 BattleManager를 따로 빼서 관리

- **트러블 슈팅**
    - 매번마다 파일 추가하면 .vcxproj 충돌 → CMakelists.txt 바꿈
    - Branch Rules
        - 한명이 독단적으로 pull request & merge로 날려버리는 위험성을 방지하기위해서 2명이상의 승인이 필요하도록 설정
    - git convention
        - branch 이름을 대소문자구분해서 이름을 짓다가 branch 이름 충돌? 이 발생함
        - 그래서 대소문자 구분을 하지않고 이름을 짓기로 결정