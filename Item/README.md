# Item README

## By In Goo Lee

## Description

## Things to Create

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