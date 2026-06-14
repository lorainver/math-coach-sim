#include "animation.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

namespace anim {

    static void sleepMs(unsigned int ms) {
        this_thread::sleep_for(chrono::milliseconds(ms));
    }

    static void clearScreen() {
#ifdef _WIN32
        system("CLS");
#else
        system("clear");
#endif
    }

    static void typewriterPrint(const wstring &text, int delayMs = 30) {
        for (size_t i = 0; i < text.size(); i++) {
            wprintf(L"%lc", text[i]);
            fflush(stdout);
            sleepMs(delayMs);
        }
    }

    static void drawProgressBar(int current, int total, int width = 30) {
        wprintf(L"\r  [");
        int filled = (current * width) / total;
        for (int i = 0; i < width; i++) {
            if (i < filled)
                wprintf(L"█");
            else
                wprintf(L"░");
        }
        wprintf(L"] %3d%%", (current * 100) / total);
        fflush(stdout);
    }

    static void sparkleEffect() {
        const wchar_t *sparkles[] = {L"✨", L"⭐", L"💫", L"🌟", L"✦"};
        int numSparkles = 5;
        for (int i = 0; i < 8; i++) {
            int x = rand() % 50 + 5;
            int y = rand() % 3 + 1;
            wprintf(L"\033[%d;%dH%ls", y, x, sparkles[i % numSparkles]);
            fflush(stdout);
            sleepMs(80);
        }
    }

    void successAnimation(int reward) {
        clearScreen();
        wprintf(L"\n");

        wprintf(L"\033[92m");
        wprintf(L"╔══════════════════════════════════════════════════════════════╗\n");
        wprintf(L"║                                                              ║\n");
        wprintf(L"╚══════════════════════════════════════════════════════════════╝\n");
        wprintf(L"\033[0m");

        wprintf(L"\033[92m");
        wprintf(L"\033[2A");
        wprintf(L"\033[14C");
        wstring mainText = L"🎉 恭喜你，回答正确！";
        typewriterPrint(mainText, 50);
        wprintf(L"\n");
        wprintf(L"\033[0m");

        sleepMs(200);

        if (reward > 0) {
            wprintf(L"\033[93m");
            wprintf(L"\033[14C");
            wstring rewardText = L"💡 达成卓越解答！能力提升了 " + to_wstring(reward) + L" 级！";
            typewriterPrint(rewardText, 40);
            wprintf(L"\n");
            wprintf(L"\033[0m");
        }

        sleepMs(150);

        wprintf(L"\n");
        wprintf(L"\033[92m");
        wprintf(L"\033[14C");
        wprintf(L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        wprintf(L"\033[0m");

        sleepMs(100);

        int progressWidth = 25;
        for (int i = 0; i <= progressWidth; i++) {
            drawProgressBar(i, progressWidth, progressWidth);
            sleepMs(30);
        }
        wprintf(L"\n");

        sleepMs(200);

        wprintf(L"\033[92m");
        wprintf(L"\033[14C");
        wstring congratsText = L"继续加油，离AKIMO更近一步！";
        typewriterPrint(congratsText, 45);
        wprintf(L"\n");
        wprintf(L"\033[0m");

        sleepMs(300);

        wprintf(L"\033[92m");
        wprintf(L"\n");
        wprintf(L"╔══════════════════════════════════════════════════════════════╗\n");
        wprintf(L"║                                                              ║\n");
        wprintf(L"╚══════════════════════════════════════════════════════════════╝\n");
        wprintf(L"\033[0m");

        sleepMs(300);
    }

    void failureAnimation() {
        clearScreen();
        wprintf(L"\n");

        wprintf(L"\033[91m");
        wprintf(L"╔══════════════════════════════════════════════════════════════╗\n");
        wprintf(L"║                                                              ║\n");
        wprintf(L"╚══════════════════════════════════════════════════════════════╝\n");
        wprintf(L"\033[0m");

        wprintf(L"\033[91m");
        wprintf(L"\033[2A");
        wprintf(L"\033[14C");
        wstring failText = L"❌ 答案错误！！！";
        typewriterPrint(failText, 60);
        wprintf(L"\n");
        wprintf(L"\033[0m");

        sleepMs(300);

        wprintf(L"\033[91m");
        wprintf(L"\033[14C");
        wprintf(L"▓▒░ 冲 击 ░▒▓");
        fflush(stdout);
        sleepMs(100);
        wprintf(L"\r\033[14C         \r\033[14C");
        fflush(stdout);
        sleepMs(100);
        wprintf(L"\033[14C▓▒░ 冲 击 ░▒▓");
        fflush(stdout);
        sleepMs(200);
        wprintf(L"\n");
        wprintf(L"\033[0m");

        for (int i = 0; i < 3; i++) {
            wprintf(L"\033[91m\033[14C闪 烁\033[0m");
            fflush(stdout);
            sleepMs(100);
            wprintf(L"\r\033[14C      \r\033[14C");
            fflush(stdout);
            sleepMs(100);
        }
        wprintf(L"\033[91m\033[14C闪 烁\033[0m");
        fflush(stdout);
        sleepMs(200);
        wprintf(L"\n");

        sleepMs(200);

        wprintf(L"\033[93m");
        wprintf(L"\033[14C");
        wstring hintText = L"💡 看来还需要继续钻研，加油！";
        typewriterPrint(hintText, 40);
        wprintf(L"\n");
        wprintf(L"\033[0m");

        sleepMs(150);

        wprintf(L"\033[91m");
        wprintf(L"\033[14C");
        wprintf(L"本次能力值未发生改变。\n");
        wprintf(L"\033[0m");

        sleepMs(150);

        wprintf(L"\n");
        wprintf(L"\033[91m");
        wprintf(L"\033[14C");
        wprintf(L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        wprintf(L"\033[0m");

        sleepMs(200);

        wprintf(L"\033[91m");
        wprintf(L"\033[14C");
        wprintf(L"再试一次吧！你一定可以的！💪\n");
        wprintf(L"\033[0m");

        sleepMs(300);

        wprintf(L"\033[91m");
        wprintf(L"\n");
        wprintf(L"╔══════════════════════════════════════════════════════════════╗\n");
        wprintf(L"║                                                              ║\n");
        wprintf(L"╚══════════════════════════════════════════════════════════════╝\n");
        wprintf(L"\033[0m");

        sleepMs(300);
    }

}
