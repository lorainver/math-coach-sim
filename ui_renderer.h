#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <string>
#include <vector>

namespace ui {

    // 初始化控制台（主要是 Windows 下启用虚拟终端以支持 ANSI 颜色和 UTF-8 代码页）
    void initConsole();

    // 绘制炫酷的游戏艺术 Banner 首屏
    void drawBanner();

    // 清屏，并附带优雅的转场提示
    void clearScreen();

    // 绘制带边框的文本对话框，支持不同主题颜色
    // type: "info" (青), "warn" (黄), "success" (绿), "error" (红), "normal" (灰/白)
    void drawTextBox(const std::wstring &title, const std::wstring &text, const std::string &type = "normal");

    // 绘制选人界面的学生花名册信息
    void drawStudentList();

    // 将评级级别（1-4）转为带颜色的评级字串，并带高亮颜色
    std::wstring getColoredRatingText(int rating);

    // 获取精美的图形化进度条：1=差[■□□□], 2=中[■■□□], 3=良[■■■□], 4=优[■■■■]
    std::wstring getProgressBar(int rating);

    // 绘制单个学生的状态栏，支持突出显示
    void drawStudentCard(int studentId, const std::wstring &name, int dai, int ji, int shu, int zu);

    // 绘制做题界面的卡片框，包含星级难度指示
    void drawQuestionBox(const std::wstring &intro, const std::wstring &question, int difficulty);

    // 答题正误反馈（彩色输出，带渐变提示）
    void showAnswerFeedback(bool isCorrect, int reward);

    // 展示最终全优通关（AKIMO）庆祝界面
    void showVictoryScreen();

    // 跨平台睡眠，内部对接 std::this_thread::sleep_for
    void sleepMs(unsigned int ms);
}

#endif // UI_RENDERER_H
