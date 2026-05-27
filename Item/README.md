# Item README

## By In Goo Lee

## Description

Things included
* IItem.h (interface)
* Inventory (class)
* AttackBoost & HealthPotion (class)

### Inventory
check duplicate items with item index and item count

* AddItem: used unique_ptr & vector to manage items
* UseItem: check item validity and use it then remove it.
* RemoveItem: same logic as UseItem but only remove item.
* GetItem: return item bt vector<IItem*>

### AttackBoost & HealthPotion
(has name & gold)
* AttackBoost: Increase Player attack power (only in current battle)
* HealthPotion: Restore Player health point

<br>

## ALL Done [TO DO]

- [x] creating Character class
  - [x] 만들고 위치 확인 후 item 헤더 수정
- [x] 캐릭터 클래스랑 연결
  - [x]  HealthPotion 의 Use 에서 SetHealthPoint 바로 하기 때문에,
    Character 클래스에서 Max Health 안넘게 막아줘야함
  - [x] Character Class 에서 아이템 사용시 랜덤 하게 사용



---

## ✅ 체크리스트

- [x]  아이템 사용 조건 : 자율
- [x]  아이템은 사용 순간 사라지고, 효과는 바로 캐릭터에게 반영
- [x]  아이템 : **체력 +50(포션) 또는 공격력 +10 증가**
- [x]  체력 +50(포션) : 현재 체력을 50 회복
- [x]  공격력 + 10: 해당 전투에만 효과 발휘
- [x]  플레이어 캐릭터 : 전투 중 아이템 랜덤 사용 가능