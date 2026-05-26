// #include <iostream>
// #include <windows.h>

#include "Game/GameManager.h"
#include "UI/UIManager.h"

int main() {
    ui.Initialize();

    GameManager::GetInstance().Run();
    return 0;
}

//현재 아이템사용 로직 구조상 잡템이 늘어날수록 포션 사용하기 어려워지는 구조임...

//아이템 50퍼센트 확률로 아이템 사용시도,
//소지중인 모든 아이템 중 균일한 확률로 한가지 선택
//사용 시도 시 30퍼센트 확률로 사용 성공

//**소지중인 아이템 중 사용이 가능한 아이템에 대해** 조건을 맨 앞에 추가하면 좋을 듯 
