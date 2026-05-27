#pragma once

#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <filesystem>
#include <unordered_map>

#pragma comment(lib, "winmm.lib")

class SoundManager {
private:
    // 열려있는 효과음 별명(alias)들을 기록하는 캐시 메모리
    std::unordered_map<std::string, bool> openedAliases; 

public:
    static SoundManager* GetInstance() {
        static SoundManager instance;
        return &instance;
    }

    bool PlayBGM(std::string path, bool repeat = true) {
        std::error_code ec;
        std::filesystem::path absPath = std::filesystem::absolute(path, ec);
        if (ec) return false;

        std::string finalPath = absPath.string();
        for (char& c : finalPath) {
            if (c == '/') c = '\\';
        }

        mciSendStringA("stop bgm", NULL, 0, NULL);
        mciSendStringA("close bgm", NULL, 0, NULL);

        std::string openCmd = "open \"" + finalPath + "\" type mpegvideo alias bgm";
        MCIERROR error = mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
        if (error != 0) return false;
        
        std::string command = "play bgm from 0";
        if (repeat) command += " repeat";
        MCIERROR playError = mciSendStringA(command.c_str(), NULL, 0, NULL);
        if (playError != 0) return false;

        return true;
    }

    // 효과음 재생 (캐싱 적용 최적화)
    void PlaySFX(std::string path, std::string alias) {
        // 해당 별명(alias)이 처음 불렸을 때만 파일을 Open
        if (openedAliases.find(alias) == openedAliases.end()) {
            std::error_code ec;
            std::filesystem::path absPath = std::filesystem::absolute(path, ec);
            if (ec) return;

            std::string finalPath = absPath.string();
            for (char& c : finalPath) {
                if (c == '/') c = '\\';
            }

            std::string openCmd = "open \"" + finalPath + "\" alias " + alias;
            mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
            
            // 파일이 성공적으로 열렸다고 맵에 기록
            openedAliases[alias] = true; 
        }

        // 매번 힘들게 닫고 열지 않고, 이미 끼워진 테이프를 되감아서 바로 재생
        std::string playCmd = "play " + alias + " from 0";
        mciSendStringA(playCmd.c_str(), NULL, 0, NULL);
    }

    // BGM 정지
    void StopBGM() {
        mciSendStringA("stop bgm", NULL, 0, NULL);
        mciSendStringA("close bgm", NULL, 0, NULL);
    }
};