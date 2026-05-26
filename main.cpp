#include <iostream>
#include <windows.h>

#include "Game/GameManager.h"

void InitConsoleWindow() {
    // 창 크기와 버퍼를 DOS 명령어로 강제 고정 (가로 120, 세로 30)
    system("mode con cols=150 lines=50");

    // 창 크기 조절 비활성화 (이 코드는 유지)
    HWND hConsole = GetConsoleWindow();
    if (hConsole != NULL) {
        LONG style = GetWindowLong(hConsole, GWL_STYLE);
        style = style & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
        SetWindowLong(hConsole, GWL_STYLE, style);
    }
}

// int main() {

    // GameManager::GetInstance().StartGame();
    
    // return 0;
// }

