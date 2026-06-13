#include<bits/stdc++.h>
#include "ui_renderer.h"
using namespace std;
long unsigned int tim = 1000;
int opt, zhong_lei;
int dai[1000] = {0, 1, 2, 1, 1}, ji[1000] = {0, 2, 1, 1, 3}, shu[1000] = {0, 2, 2, 2, 1}, zu[1000] = {0, 3, 1, 1, 1};
char op, ca;
int return_zhi = 0, choose_a = 0, choose_b = 0, hard = 1;
const int MAX_RATING = 4;

// ===== 函数前向声明 =====
void safe_cin_int(int &var, const wchar_t *prompt);
void safe_cin_int_pair(int &var1, int &var2, const wchar_t *prompt);

// ===== 题库数据结构 =====
struct Question {
	wstring intro;        // 难度提示语
	wstring question;     // 题目内容
	string answer_type;   // "int" / "int_pair" / "string_multi"
	int answer_int;       // answer_type == "int" 时的答案
	int answer_int_a, answer_int_b; // answer_type == "int_pair" 时的答案
	vector<string> answer_strings;  // answer_type == "string_multi" 时的所有合法答案
	int reward;           // 答对奖励
};

// 按 [category_index][difficulty] 存储, category: 0=代数 1=几何 2=数论 3=组合, difficulty: 0-based
Question questions[4][3];
int question_count[4] = {0, 0, 0, 0}; // 每个 category 已加载的题目数

// 读取带引号的 JSON 字符串值（简易解析，不支持转义）
wstring readJsonStringValue(const wstring &json, size_t &pos) {
	while (pos < json.size() && json[pos] != L'"') pos++;
	if (pos >= json.size()) return L"";
	pos++; // skip opening "
	wstring result;
	while (pos < json.size() && json[pos] != L'"') {
		if (json[pos] == L'\\' && pos + 1 < json.size()) {
			pos++;
			if (json[pos] == L'n') result += L'\n';
			else if (json[pos] == L'"') result += L'"';
			else if (json[pos] == L'\\') result += L'\\';
			else result += json[pos];
		} else {
			result += json[pos];
		}
		pos++;
	}
	if (pos < json.size()) pos++; // skip closing "
	return result;
}

string wstringToUtf8(const wstring &w) {
	string result;
	for (size_t i = 0; i < w.size(); i++) {
		if (w[i] < 0x80) {
			result += (char)w[i];
		} else if (w[i] < 0x800) {
			result += (char)(0xC0 | (w[i] >> 6));
			result += (char)(0x80 | (w[i] & 0x3F));
		} else {
			result += (char)(0xE0 | (w[i] >> 12));
			result += (char)(0x80 | ((w[i] >> 6) & 0x3F));
			result += (char)(0x80 | (w[i] & 0x3F));
		}
	}
	return result;
}

wstring utf8ToWstring(const string &str) {
	wstring result;
	size_t i = 0;
	while (i < str.size()) {
		unsigned char c = str[i];
		if (c < 0x80) {
			result += (wchar_t)c;
			i++;
		} else if ((c & 0xE0) == 0xC0) {
			if (i + 1 >= str.size()) break;
			wchar_t wc = ((c & 0x1F) << 6) | (str[i + 1] & 0x3F);
			result += wc;
			i += 2;
		} else if ((c & 0xF0) == 0xE0) {
			if (i + 2 >= str.size()) break;
			wchar_t wc = ((c & 0x0F) << 12) | ((str[i + 1] & 0x3F) << 6) | (str[i + 2] & 0x3F);
			result += wc;
			i += 3;
		} else if ((c & 0xF8) == 0xF0) {
			if (i + 3 >= str.size()) break;
			unsigned int codepoint = ((c & 0x07) << 18) | ((str[i + 1] & 0x3F) << 12) | ((str[i + 2] & 0x3F) << 6) | (str[i + 3] & 0x3F);
			if (sizeof(wchar_t) == 2) {
				codepoint -= 0x10000;
				result += (wchar_t)((codepoint >> 10) + 0xD800);
				result += (wchar_t)((codepoint & 0x3FF) + 0xDC00);
			} else {
				result += (wchar_t)codepoint;
			}
			i += 4;
		} else {
			i++;
		}
	}
	return result;
}

string readJsonStringUtf8(const wstring &json, size_t &pos) {
	wstring w = readJsonStringValue(json, pos);
	return wstringToUtf8(w);
}

int readJsonInt(const wstring &json, size_t &pos) {
	while (pos < json.size() && (json[pos] == L' ' || json[pos] == L':' || json[pos] == L',')) pos++;
	int sign = 1;
	if (pos < json.size() && json[pos] == L'-') { sign = -1; pos++; }
	int val = 0;
	while (pos < json.size() && json[pos] >= L'0' && json[pos] <= L'9') {
		val = val * 10 + (json[pos] - L'0');
		pos++;
	}
	return sign * val;
}

// 加载题库
void loadQuestions(const string &filename) {
	// 读取文件为 UTF-8 string 并转换为 wstring，避免 locale 导致的中文乱码问题
	ifstream fin(filename, ios::binary);
	if (!fin.is_open()) {
		wchar_t err[] = L"错误：无法打开题库文件 questions.json";
		wprintf(L"%ls", err);
		cout << "\n";
		return;
	}
	string json_utf8((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	fin.close();

	wstring json = utf8ToWstring(json_utf8);

	size_t pos = 0;
	while (pos < json.size()) {
		// 找到每个题目对象的开始 {
		while (pos < json.size() && json[pos] != L'{') pos++;
		if (pos >= json.size()) break;
		pos++; // skip {

		wstring category;
		int difficulty = 0;
		Question q;

		// 解析字段
		while (pos < json.size()) {
			// 跳过空白和逗号
			while (pos < json.size() && (json[pos] == L' ' || json[pos] == L'\n' || json[pos] == L'\r' || json[pos] == L'\t' || json[pos] == L',')) pos++;
			if (pos >= json.size() || json[pos] == L'}') break;

			// 读 key
			wstring key = readJsonStringValue(json, pos);

			// 跳过冒号
			while (pos < json.size() && json[pos] != L':') pos++;
			pos++;

			if (key == L"category") {
				category = readJsonStringValue(json, pos);
			} else if (key == L"difficulty") {
				difficulty = readJsonInt(json, pos);
			} else if (key == L"intro") {
				q.intro = readJsonStringValue(json, pos);
			} else if (key == L"question") {
				q.question = readJsonStringValue(json, pos);
			} else if (key == L"answer_type") {
				q.answer_type = readJsonStringUtf8(json, pos);
			} else if (key == L"answer") {
				// 跳过空白
				while (pos < json.size() && (json[pos] == L' ' || json[pos] == L'\n' || json[pos] == L'\r' || json[pos] == L'\t')) pos++;
				if (pos < json.size() && json[pos] == L'[') {
					// 数组形式: int_pair 或 string_multi
					pos++; // skip [
					if (q.answer_type == "int_pair") {
						q.answer_int_a = readJsonInt(json, pos);
						q.answer_int_b = readJsonInt(json, pos);
					} else if (q.answer_type == "string_multi") {
						while (pos < json.size() && json[pos] != L']') {
							if (json[pos] == L'"') {
								string s = readJsonStringUtf8(json, pos);
								q.answer_strings.push_back(s);
							} else {
								pos++;
							}
						}
					} else {
						// single int in array
						q.answer_int = readJsonInt(json, pos);
					}
					while (pos < json.size() && json[pos] != L']') pos++;
					if (pos < json.size()) pos++; // skip ]
				} else {
					// 单个整数
					q.answer_int = readJsonInt(json, pos);
				}
			} else if (key == L"reward") {
				q.reward = readJsonInt(json, pos);
			} else {
				// 跳过未知字段
				while (pos < json.size() && json[pos] != L',' && json[pos] != L'}') pos++;
			}
		}

		// 映射 category 到 index
		int cat_idx = -1;
		if (category == L"代数") cat_idx = 0;
		else if (category == L"几何") cat_idx = 1;
		else if (category == L"数论") cat_idx = 2;
		else if (category == L"组合") cat_idx = 3;

		if (cat_idx >= 0 && difficulty >= 1 && difficulty <= 3) {
			questions[cat_idx][difficulty - 1] = q;
			question_count[cat_idx]++;
		}

		// 跳过 }
		while (pos < json.size() && json[pos] != L'}') pos++;
		if (pos < json.size()) pos++;
	}
}

// ===== 显示函数 =====
void out_pj(int x) {
	if (x <= 1) {
		wchar_t lw1[] = L"差";
		wprintf(L"%ls", lw1);
	} else if (x == 2) {
		wchar_t lw2[] = L"中";
		wprintf(L"%ls", lw2);
	} else if (x == 3) {
		wchar_t lw3[] = L"良";
		wprintf(L"%ls", lw3);
	} else {
		wchar_t lw4[] = L"优";
		wprintf(L"%ls", lw4);
	}
}

void clamp_rating(int &v) {
	if (v > MAX_RATING) v = MAX_RATING;
}

void out(int x) {
	if (x <= 0 || x > 4) {
		cout << "???";
		return;
	}
	wstring name;
	if (x == 1) name = L"甲子";
	else if (x == 2) name = L"乙丑";
	else if (x == 3) name = L"丙寅";
	else name = L"丁卯";
	ui::drawStudentCard(x, name, dai[x], ji[x], shu[x], zu[x]);
}

// ===== 做题逻辑（数据驱动） =====
int do_question(int cat_idx) {
	int diff_idx = hard - 1; // hard: 1~3, diff_idx: 0~2
	if (diff_idx < 0 || diff_idx > 2) diff_idx = 0;
	Question &q = questions[cat_idx][diff_idx];

	ui::drawQuestionBox(q.intro, q.question, hard);

	if (q.answer_type == "int_pair") {
		int a, b;
		safe_cin_int_pair(a, b, L"输入无效，请输入两个用空格分隔的整数（两根从小到大）：");
		if (a == q.answer_int_a && b == q.answer_int_b) {
			ui::showAnswerFeedback(true, q.reward);
			return q.reward;
		} else {
			ui::showAnswerFeedback(false, 0);
			return 0;
		}
	} else if (q.answer_type == "int") {
		int a;
		safe_cin_int(a, L"输入无效，请输入一个整数数字：");
		if (a == q.answer_int) {
			ui::showAnswerFeedback(true, q.reward);
			return q.reward;
		} else {
			ui::showAnswerFeedback(false, 0);
			return 0;
		}
	} else if (q.answer_type == "string_multi") {
		string a;
		cin >> a;
		for (size_t i = 0; i < q.answer_strings.size(); i++) {
			if (a == q.answer_strings[i]) {
				ui::showAnswerFeedback(true, q.reward);
				return q.reward;
			}
		}
		ui::showAnswerFeedback(false, 0);
		return 0;
	}

	ui::showAnswerFeedback(false, 0);
	return 0;
}

// ===== 安全输入 =====
void safe_cin_int(int &var, const wchar_t *prompt) {
	while (!(cin >> var)) {
		cin.clear();
		cin.ignore(10000, '\n');
		wprintf(L"%ls", prompt);
	}
}

void safe_cin_int_pair(int &var1, int &var2, const wchar_t *prompt) {
	while (!(cin >> var1 >> var2)) {
		cin.clear();
		cin.ignore(10000, '\n');
		wprintf(L"%ls", prompt);
	}
}

// ===== 主函数 =====
int main() {
	srand(time(0));

	// 初始化控制台并开启 UTF-8 & ANSI 颜色
	ui::initConsole();

	// 加载题库
	loadQuestions("questions.json");

	// 绘制 Banner
	ui::drawBanner();
	ui::sleepMs(tim / 4);

	ui::drawTextBox(L"系统选项", L"如果您想开始游戏，请输入 1，否则请输入 2。", "info");
	safe_cin_int(opt, L"输入无效，请重新输入（1开始，2退出）：");
	if (opt == 2) {
		ui::clearScreen();
		ui::drawTextBox(L"系统消息", L"再见！", "info");
		ui::sleepMs(tim);
		ui::clearScreen();
		return 0;
	} else {
		ui::clearScreen();
		ui::drawTextBox(L"系统消息", L"开始游戏！", "success");
		ui::sleepMs(tim);
		ui::drawTextBox(L"背景描述", L"时光荏苒，白云苍狗。不知不觉中，你教的学生已到了高一。而你，作为 xx 中学的数学竞赛主教练，将指导学生参加比赛，并争取获得成绩。", "info");
		ui::sleepMs(tim * 3);
		ui::drawTextBox(L"游戏介绍", L"这是一款模拟类游戏，玩家需根据状况做出选择，并尽量达成游戏目标。", "info");
		ui::sleepMs(tim * 2);
		ui::drawTextBox(L"游戏目标", L"协助学生 AKIMO。你准备好了吗？好了请输入\"A\"（注意是大写的）", "warn");
		ui::sleepMs(tim);
		while (cin >> op) {
			if (op == 'A') {
				break;
			}
		}
	}
	ui::clearScreen();
	ui::drawTextBox(L"规则提示", L"下面请选择学生。（注：代数，几何，数论，组合这四个能力指标有差、中、良、优四个评级）\n你可以选择 2 个学生，输入他们的番号（a-d），一行一个，并在输入结束后输入 \"A\"", "info");
	ui::sleepMs(tim);

	// 使用更美观的花名册界面
	ui::drawStudentList();

	while (cin >> ca) {
		if (ca == 'A') break;
		int idx = int(ca) - 'a' + 1;
		if (idx < 1 || idx > 4) continue;
		if (choose_a == 0) {
			choose_a = idx;
		} else if (choose_b == 0) {
			if (idx == choose_a) continue;
			choose_b = idx;
		} else {
			break;
		}
	}
	ui::drawTextBox(L"系统消息", L"好的，学生选择完成，下面正式开始游戏！", "success");
	ui::sleepMs(tim);
	ui::clearScreen();
	while (dai[choose_a] < MAX_RATING || ji[choose_a] < MAX_RATING || shu[choose_a] < MAX_RATING || zu[choose_a] < MAX_RATING) {
		ui::drawTextBox(L"当前任务提示", L"你发现你的学生有些指标较弱，便想找一道题来提升他们的能力。但是，你需要自己先做出来才能服众。\n请输入题目板块编号 [1:代数, 2:几何, 3:数论, 4:组合] (难度会随机决定)：", "info");
		ui::sleepMs(tim);
		safe_cin_int(zhong_lei, L"输入无效，请输入1-4的数字：");
		if (zhong_lei < 1 || zhong_lei > 4) {
			ui::drawTextBox(L"输入错误", L"无效的板块编号，请输入 1-4 之间的数字！", "error");
			ui::sleepMs(tim);
			ui::clearScreen();
			continue;
		}
		ui::clearScreen();
		ui::drawTextBox(L"系统消息", L"题目加载中，下面请开始做题吧！", "info");
		ui::sleepMs(tim);
		ui::clearScreen();
		hard = (rand() % 3) + 1;

		int cat_idx = zhong_lei - 1; // 0=代数 1=几何 2=数论 3=组合
		return_zhi = do_question(cat_idx);
		ui::sleepMs(tim);
		ui::clearScreen();

		int *rating_arr;
		if (cat_idx == 0) rating_arr = dai;
		else if (cat_idx == 1) rating_arr = ji;
		else if (cat_idx == 2) rating_arr = shu;
		else rating_arr = zu;

		rating_arr[choose_a] += return_zhi;
		rating_arr[choose_b] += return_zhi;
		clamp_rating(rating_arr[choose_a]);
		clamp_rating(rating_arr[choose_b]);

		ui::drawTextBox(L"成绩更新", L"学生能力值更新完毕：", "success");
		ui::sleepMs(tim / 2);
		out(choose_a);
		cout << "\n";
		ui::sleepMs(tim / 2);
		out(choose_b);
		cout << "\n";
		ui::sleepMs(tim * 4);
		ui::clearScreen();
	}
	ui::clearScreen();
	ui::showVictoryScreen();
	ui::sleepMs(tim * 2);
	return 0;
}
