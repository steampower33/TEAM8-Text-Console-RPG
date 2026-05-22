# Item README

## By In Goo Lee

## Description

* 우선 UML 기준으로해서 필수기능 완료

## TO DO
### 같이 이야기 해서 해야 하는 부분
- [ ] creating Character class
  - [ ] 만들고 위치 확인 후 item 헤더 수정
- [ ] 캐릭터 클래스랑 연결
  - [ ]  HealthPotion 의 Use 에서 SetHealthPoint 바로 하기 때문에, 
  Character 클래스에서 Max Health 안넘게 막아줘야함
  - [ ] Character Class 에서 아이템 사용시 랜덤 하게 사용



---
## Things to Create (BY UML & Requirements)

| Following UML (these will be edited)
<br>
| Interface == Item | Class == HealthPotion & AttackBoost
### Item
* IItem (interface)
  * getName(): string
  * use(character: Charter*): void

### HealthPotion
<br>
* HealthPotion (class) : IItem

(mem)
* string name;
* int healthRestore;

<br>
(Function)

* HealthPotion();
* getName(): string:
* use(character: Charter*) void:


### AttackBoost
<br>
* AttackBoost (class) : IItem

(mem)
* string name;
* int healthRestore;

<br>
(Function)

* AttackBoost();
* getName(): string:
* use(character: Charter*) void:

### Maybe Later
(mem)
* int price (== gold)
* Testing

### ✅ 체크리스트

- [x]  아이템 사용 조건 : 자율
- [x]  아이템은 사용 순간 사라지고, 효과는 바로 캐릭터에게 반영
- [x]  아이템 : **체력 +50(포션) 또는 공격력 +10 증가**
- [x]  체력 +50(포션) : 현재 체력을 50 회복
- [x]  공격력 + 10: 해당 전투에만 효과 발휘
- [x]  플레이어 캐릭터 : 전투 중 아이템 랜덤 사용 가능