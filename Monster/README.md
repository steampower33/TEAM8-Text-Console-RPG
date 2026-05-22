# TEAM8-Text-Console-RPG

## Monster 사용 방법

몬스터는 `CreateMonster()` 팩토리 함수로 생성합니다.

```cpp
#include <iostream>

#include "Monster/utils/Create.h"

int main()
{
    std::unique_ptr<Monster> monster = CreateMonster(MonsterType::Goblin, 3);

    std::cout << monster->GetName() << '\n';

    MonsterStats stats = monster->GetStatus();
    std::cout << "HP: " << stats.HP << '\n';
    std::cout << "ATK: " << stats.ATK << '\n';
    std::cout << "DEF: " << stats.DEF << '\n';
    std::cout << "AVD: " << stats.AVD << '\n';

    monster->TakeDamage(20);
    std::cout << "After Damage HP: " << monster->GetStatus().HP << '\n';

    MonsterReward reward = monster->GetReward();
    std::cout << "EXP: " << reward.Exp << '\n';
    std::cout << "Item: " << reward.Item.name << '\n';
    std::cout << "Gold: " << reward.Item.gold << '\n';
}
```

생성 가능한 타입은 아래와 같습니다.

```cpp
auto goblin = CreateMonster(MonsterType::Goblin, 3);
auto orc = CreateMonster(MonsterType::Orc, 3);
auto troll = CreateMonster(MonsterType::Troll, 3);
auto slime = CreateMonster(MonsterType::Slime, 3);


    auto goblin = CreateMonster(MonsterType::Goblin, 10);
    std::cout << goblin->GetStatus().HP << std::endl;
    goblin->TakeDamage(100);
    std::cout <<  goblin->GetStatus().HP<< std::endl;

    auto goblin2 = CreateMonster(MonsterType::Goblin, 100);
    std::cout << goblin2->GetStatus().HP << std::endl;
    goblin2->TakeDamage(100);
    std::cout <<  goblin2->GetStatus().HP<< std::endl;

    /**********랜덤 ******** */
    auto randomMonster = CreateRandomMonster(10); // 랜덤
    std::cout << randomMonster->GetStatus().Name << std::endl;

    auto randomMonster2 = CreateRandomMonster(10);
    std::cout << randomMonster2->GetStatus().Name << std::endl;

    auto randomMonster3 = CreateRandomMonster(10);
    std::cout << randomMonster3->GetStatus().Name << std::endl;


```

`GetStatus()`는 현재 상태의 복사본을 반환합니다. 데미지 적용 후 체력을 확인하려면 `TakeDamage()` 호출 뒤 다시 `GetStatus()`를 호출해야 합니다.

## 코덱스 코드리뷰

### 현재 구조 장점

- `Monster/data/MonsterStructs.h`, `Monster/data/MonsterTable.h`, `Monster.h`가 분리되어 구조체, 기본 데이터, 공통 동작의 책임이 이전보다 명확합니다.
- `Monster/classes`의 `Goblin`, `Orc`, `Troll`, `Slime`은 생성자에서 각 타입의 데이터만 넘기고, `GetStatus()`, `TakeDamage()`, `GetReward()`는 `Monster`에서 공통 처리합니다.
- `CreateMonster()`를 통해 생성 방식을 통일해서 전투 코드가 특정 몬스터 클래스에 직접 의존하지 않아도 됩니다.

### 개선 필요 사항

- `Monster/classes` 안의 레벨 보정 코드가 거의 같습니다. 몬스터별 성장식이 같다면 공통 함수로 분리하는 것이 좋습니다.
- `MonsterType::Count`는 실제 몬스터 타입이 아니므로 실수로 생성 요청에 들어갈 수 있습니다. 이름을 `Invalid`나 `Max`처럼 더 명확하게 하거나 제거를 검토할 수 있습니다.
- `CreateMonster()`는 미지원 타입에서 예외를 던집니다. 호출하는 쪽에서 `try-catch`를 할지, `nullptr`를 반환할지 팀 규칙을 정해야 합니다.
- `MonsterStats`의 `HP`는 현재 체력으로 사용됩니다. 나중에 최대 체력이 필요하면 `MaxHP`, `CurrentHP`처럼 분리하는 것이 좋습니다.
- `DevItem`은 임시 아이템 구조체입니다. 아이템 시스템이 확정되면 실제 `Item` 타입과 연결해야 합니다.
