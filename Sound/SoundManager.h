#pragma once

#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <filesystem>

#pragma comment(lib, "winmm.lib")

class SoundManager {
public:
    static SoundManager* GetInstance() {
        static SoundManager instance;
        return &instance;
    }

    bool PlayBGM(std::string path) {
        std::error_code ec;
        std::filesystem::path absPath = std::filesystem::absolute(path, ec);
        if (ec) return false;

        std::string finalPath = absPath.string();
        for (char& c : finalPath) {
            if (c == '/') c = '\\';
        }

        mciSendStringA("close bgm", NULL, 0, NULL);

        // 이제 순정 wav 파일이라 에러가 나지 않으며, repeat 명령어를 정상적으로 수행합니다.
        std::string openCmd = "open \"" + finalPath + "\" type mpegvideo alias bgm";
        MCIERROR error = mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
        if (error != 0) return false;

        // 이제 repeat가 완벽하게 먹힙니다.
        MCIERROR playError = mciSendStringA("play bgm from 0 repeat", NULL, 0, NULL);
        if (playError != 0) return false;

        return true;
    }

    // 효과음 재생 (비동기, 연타 가능)
    void PlaySFX(std::string path, std::string alias) {
        std::error_code ec;
        std::filesystem::path absPath = std::filesystem::absolute(path, ec);
        if (ec) return;

        std::string finalPath = absPath.string();
        for (char& c : finalPath) {
            if (c == '/') c = '\\';
        }

        // 재생할 때마다 기존 채널을 닫고 새로 열어서 처음부터 재생(연타 최적화)
        std::string closeCmd = "close " + alias;
        mciSendStringA(closeCmd.c_str(), NULL, 0, NULL);

        std::string openCmd = "open \"" + finalPath + "\" alias " + alias;
        mciSendStringA(openCmd.c_str(), NULL, 0, NULL);

        std::string playCmd = "play " + alias + " from 0";
        mciSendStringA(playCmd.c_str(), NULL, 0, NULL);
    }

    // BGM 정지
    void StopBGM() {
        mciSendStringA("stop bgm", NULL, 0, NULL);
        mciSendStringA("close bgm", NULL, 0, NULL);
    }
};