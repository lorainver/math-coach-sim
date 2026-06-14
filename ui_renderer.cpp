#include "ui_renderer.h"
#include "animation.h"
#include <iostream>
#include <cwchar>
#include <clocale>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

using namespace std;

// ANSI 颜色转义字符定义
#define COLOR_RESET   L"\033[0m"
#define COLOR_BOLD    L"\033[1m"
#define COLOR_GREY    L"\033[90m"

// 前景色
#define COLOR_RED     L"\033[91m"
#define COLOR_GREEN   L"\033[92m"
#define COLOR_YELLOW  L"\033[93m"
#define COLOR_BLUE    L"\033[94m"
#define COLOR_MAGENTA L"\033[95m"
#define COLOR_CYAN    L"\033[96m"
#define COLOR_WHITE   L"\033[97m"

// 背景色
#define BG_BLUE       L"\033[44m"
#define BG_GREEN      L"\033[42m"
#define BG_RED        L"\033[41m"

namespace ui {

    void initConsole() {
        setlocale(LC_ALL, "");
#ifdef _WIN32
        // 设置 Windows 控制台为 UTF-8 编码页，保证 Unicode 字符不乱码
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);

        // 启用虚拟终端，开启 ANSI 颜色支持
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }

    void drawBanner() {
        wprintf(L"%ls", COLOR_CYAN);
        wprintf(L"=====================================================================\n");
        wprintf(L"     __  __  ____     _____                  _                     \n");
        wprintf(L"    |  \\/  |/ __ \\   / ____|                | |                    \n");
        wprintf(L"    | \\  / | |  | | | |     ___   __ _  ___| |__                  \n");
        wprintf(L"    | |\\/| | |  | | | |    / _ \\ / _` |/ __| '_ \\                 \n");
        wprintf(L"    | |  | | |__| | | |___| (_) | (_| | (__| | | |                 \n");
        wprintf(L"    |_|  |_|\\____/   \\_____\\___/ \\__,_|\\___|_| |_|                 \n");
        wprintf(L"                                                                   \n");
        wprintf(L"               🧮 MO 数学竞赛主教练模拟器 (v1.1)                    \n");
        wprintf(L"=====================================================================\n");
        wprintf(L"%ls", COLOR_RESET);
    }

    void clearScreen() {
#ifdef _WIN32
        system("CLS");
#else
        system("clear");
#endif
    }

    // ================================================================
    // drawTextBox — 绘制带边框的文本对话框
    //   title : 对话框标题（如 "系统消息"）
    //   text  : 正文内容
    //   type  : 类型 ("info" / "warn" / "success" / "error")，控制图标与颜色
    //
    // 注意：emoji 图标（✅❌📢 等）需要终端字体支持（推荐 Windows Terminal
    // + Cascadia Code）。已注释掉图标行以兼容更多终端环境；如需恢复，
    // 取消 %ls icon 那一行即可。
    // ================================================================
    void drawTextBox(const wstring &title, const wstring &text, const string &type) {
        wstring themeColor = COLOR_WHITE;
        wstring icon = L"📢";
        
        if (type == "info") {
            themeColor = COLOR_CYAN;
            icon = L"ℹ️ ";
        } else if (type == "warn") {
            themeColor = COLOR_YELLOW;
            icon = L"⚠️ ";
        } else if (type == "success") {
            themeColor = COLOR_GREEN;
            icon = L"✅";
        } else if (type == "error") {
            themeColor = COLOR_RED;
            icon = L"❌";
        }

        // [临时] 关闭主题色前缀，避免 ANSI 转义码在某些终端下干扰框线渲染
        // wprintf(L"%ls", themeColor);
        //
        // [临时] 去掉 emoji 图标，因非 WT 终端（cmd / 旧 PowerShell）的默认
        // 字体缺少 emoji 字形，显示为 ? 或方块，破坏框线对齐
        wprintf(L"┌───  %ls  ────────────────────────────────────────────────\n",  title.c_str());
        // 原始带图标版本（UTF-8 + WT 终端可用）：
        // wprintf(L"┌─── %ls %ls ────────────────────────────────────────────────\n", icon.c_str(), title.c_str());
        wprintf(L"│ %ls\n", text.c_str());
        wprintf(L"└─────────────────────────────────────────────────────────────\n");
        wprintf(L"%ls", COLOR_RESET);
    }

    wstring getColoredRatingText(int rating) {
        switch (rating) {
            case 1: return wstring(COLOR_RED) + L"差" + COLOR_RESET;
            case 2: return wstring(COLOR_YELLOW) + L"中" + COLOR_RESET;
            case 3: return wstring(COLOR_CYAN) + L"良" + COLOR_RESET;
            case 4: return wstring(COLOR_GREEN) + COLOR_BOLD + L"优" + COLOR_RESET;
            default: return wstring(COLOR_GREY) + L"无" + COLOR_RESET;
        }
    }

    wstring getProgressBar(int rating) {
        switch (rating) {
            case 1: return wstring(L"[") + COLOR_RED + L"■" + COLOR_GREY + L"□□□" + COLOR_RESET + L"]";
            case 2: return wstring(L"[") + COLOR_YELLOW + L"■■" + COLOR_GREY + L"□□" + COLOR_RESET + L"]";
            case 3: return wstring(L"[") + COLOR_CYAN + L"■■■" + COLOR_GREY + L"□" + COLOR_RESET + L"]";
            case 4: return wstring(L"[") + COLOR_GREEN + COLOR_BOLD + L"■■■■" + COLOR_RESET + L"]";
            default: return L"[□□□□]";
        }
    }

    void drawStudentList() {
        wprintf(L"  🧑‍🎓 可选学生名册：\n");
        wprintf(L"  -------------------------------------------------------------\n");
        wprintf(L"  %ls[a] 甲子%ls | 代数: %ls %ls | 几何: %ls %ls | 数论: %ls %ls | 组合: %ls %ls\n", 
                COLOR_BOLD, COLOR_RESET, getProgressBar(1).c_str(), getColoredRatingText(1).c_str(),
                getProgressBar(2).c_str(), getColoredRatingText(2).c_str(),
                getProgressBar(2).c_str(), getColoredRatingText(2).c_str(),
                getProgressBar(3).c_str(), getColoredRatingText(3).c_str());
                
        wprintf(L"  %ls[b] 乙丑%ls | 代数: %ls %ls | 几何: %ls %ls | 数论: %ls %ls | 组合: %ls %ls\n", 
                COLOR_BOLD, COLOR_RESET, getProgressBar(2).c_str(), getColoredRatingText(2).c_str(),
                getProgressBar(1).c_str(), getColoredRatingText(1).c_str(),
                getProgressBar(2).c_str(), getColoredRatingText(2).c_str(),
                getProgressBar(1).c_str(), getColoredRatingText(1).c_str());

        wprintf(L"  %ls[c] 丙寅%ls | 代数: %ls %ls | 几何: %ls %ls | 数论: %ls %ls | 组合: %ls %ls\n", 
                COLOR_BOLD, COLOR_RESET, getProgressBar(1).c_str(), getColoredRatingText(1).c_str(),
                getProgressBar(1).c_str(), getColoredRatingText(1).c_str(),
                getProgressBar(2).c_str(), getColoredRatingText(2).c_str(),
                getProgressBar(1).c_str(), getColoredRatingText(1).c_str());

        wprintf(L"  %ls[d] 丁卯%ls | 代数: %ls %ls | 几何: %ls %ls | 数论: %ls %ls | 组合: %ls %ls\n", 
                COLOR_BOLD, COLOR_RESET, getProgressBar(1).c_str(), getColoredRatingText(1).c_str(),
                getProgressBar(3).c_str(), getColoredRatingText(3).c_str(),
                getProgressBar(1).c_str(), getColoredRatingText(1).c_str(),
                getProgressBar(1).c_str(), getColoredRatingText(1).c_str());
        wprintf(L"  -------------------------------------------------------------\n");
    }

    void drawStudentCard(int studentId, const wstring &name, int dai, int ji, int shu, int zu) {
        wstring idStr = L"[" + to_wstring(studentId) + L"]";
        wprintf(L"%ls", COLOR_CYAN);
        wprintf(L"┌─── 📊 学生状态面板: %ls %ls ────────────────────────────────────\n", idStr.c_str(), name.c_str());
        wprintf(L"│  📐 %ls代数%ls: %ls %-6ls   📐 %ls几何%ls: %ls %-6ls\n", 
                COLOR_BOLD, COLOR_RESET, getProgressBar(dai).c_str(), getColoredRatingText(dai).c_str(),
                COLOR_BOLD, COLOR_RESET, getProgressBar(ji).c_str(), getColoredRatingText(ji).c_str());
        wprintf(L"│  📐 %ls数论%ls: %ls %-6ls   📐 %ls组合%ls: %ls %-6ls\n", 
                COLOR_BOLD, COLOR_RESET, getProgressBar(shu).c_str(), getColoredRatingText(shu).c_str(),
                COLOR_BOLD, COLOR_RESET, getProgressBar(zu).c_str(), getColoredRatingText(zu).c_str());
        wprintf(L"└─────────────────────────────────────────────────────────────\n");
        wprintf(L"%ls", COLOR_RESET);
    }

    void drawQuestionBox(const wstring &intro, const wstring &question, int difficulty) {
        wstring stars = L"";
        wstring diffText = L"";
        wstring diffColor = COLOR_WHITE;

        if (difficulty == 1) {
            stars = L"★☆☆";
            diffText = L"简单";
            diffColor = COLOR_GREEN;
        } else if (difficulty == 2) {
            stars = L"★★☆";
            diffText = L"中等";
            diffColor = COLOR_YELLOW;
        } else if (difficulty == 3) {
            stars = L"★★★";
            diffText = L"困难 (高联/CMO真题)";
            diffColor = COLOR_RED;
        }

        wprintf(L"%ls", COLOR_BLUE);
        wprintf(L"┌─── 📝 题目分配中... ─────────────────────────────────────────\n");
        wprintf(L"│  ⭐ 难度：%ls%ls (%ls)%ls\n", diffColor.c_str(), stars.c_str(), diffText.c_str(), COLOR_BLUE);
        wprintf(L"│  📌 提示：%ls%ls%ls\n", COLOR_CYAN, intro.c_str(), COLOR_BLUE);
        wprintf(L"├─────────────────────────────────────────────────────────────\n");
        wprintf(L"│  %ls%ls%ls\n", COLOR_WHITE, question.c_str(), COLOR_BLUE);
        wprintf(L"└─────────────────────────────────────────────────────────────\n");
        wprintf(L"%ls", COLOR_RESET);
    }

    void showAnswerFeedback(bool isCorrect, int reward) {
        if (isCorrect) {
            anim::successAnimation(reward);
        } else {
            anim::failureAnimation();
        }
    }

    void showVictoryScreen() {
        wprintf(L"%ls", COLOR_GREEN);
        wprintf(L"=====================================================================\n");
        wprintf(L"  🏆  🎓  🎯   恭 喜 你 达 成 终 极 目 标 ： A K I M O ! ! !   🎯  🎓  🏆\n");
        wprintf(L"=====================================================================\n");
        wprintf(L"  你培养的学生成功攻克了代数、几何、数论、组合全部四个方向的奥数难题。\n");
        wprintf(L"  作为主教练，你带领学校登上了数学竞赛的荣誉巅峰！\n");
        wprintf(L"  游戏结束，感谢游玩！\n");
        wprintf(L"=====================================================================\n");
        wprintf(L"%ls", COLOR_RESET);
    }

    void sleepMs(unsigned int ms) {
        this_thread::sleep_for(chrono::milliseconds(ms));
    }
}
