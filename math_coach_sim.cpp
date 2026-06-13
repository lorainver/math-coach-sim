#include<bits/stdc++.h>
using namespace std;
long unsigned int tim = 1000;
int opt, zhong_lei;
int dai[1000] = {0, 1, 2, 1, 1}, ji[1000] = {0, 2, 1, 1, 3}, shu[1000] = {0, 2, 2, 2, 1}, zu[1000] = {0, 3, 1, 1, 1};
char op, ca;
int return_zhi = 0, choose_a = 0, choose_b = 0, hard = 1;
const int MAX_RATING = 4;

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
	// 读取文件为 wstring
	wifstream fin(filename);
	if (!fin.is_open()) {
		wchar_t err[] = L"错误：无法打开题库文件 questions.json";
		wprintf(L"%ls", err);
		cout << "\n";
		return;
	}
	fin.imbue(locale(""));
	wstring json((istreambuf_iterator<wchar_t>(fin)), istreambuf_iterator<wchar_t>());
	fin.close();

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
	if (x == 1) {
		wchar_t lw1[] = L"甲子";
		wprintf(L"%ls", lw1);
	} else if (x == 2) {
		wchar_t lw2[] = L"乙丑";
		wprintf(L"%ls", lw2);
	} else if (x == 3) {
		wchar_t lw3[] = L"丙寅";
		wprintf(L"%ls", lw3);
	} else {
		wchar_t lw4[] = L"丁卯";
		wprintf(L"%ls", lw4);
	}
	cout << ":";
	wchar_t lw5[] = L"代数 ";
	wchar_t lw6[] = L"几何 ";
	wchar_t lw7[] = L"数论 ";
	wchar_t lw8[] = L"组合 ";
	wprintf(L"%ls", lw5);
	out_pj(dai[x]);
	cout << ",";
	wprintf(L"%ls", lw6);
	out_pj(ji[x]);
	cout << ",";
	wprintf(L"%ls", lw7);
	out_pj(shu[x]);
	cout << ",";
	wprintf(L"%ls", lw8);
	out_pj(zu[x]);
	cout << ".";
}

// ===== 做题逻辑（数据驱动） =====
int do_question(int cat_idx) {
	int diff_idx = hard - 1; // hard: 1~3, diff_idx: 0~2
	if (diff_idx < 0 || diff_idx > 2) diff_idx = 0;
	Question &q = questions[cat_idx][diff_idx];

	wprintf(L"%ls", q.intro.c_str());
	cout << "\n";
	_sleep(tim);
	wprintf(L"%ls", q.question.c_str());
	cout << "\n";

	if (q.answer_type == "int_pair") {
		int a, b;
		cin >> a >> b;
		if (a == q.answer_int_a && b == q.answer_int_b) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return q.reward;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else if (q.answer_type == "int") {
		int a;
		cin >> a;
		if (a == q.answer_int) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return q.reward;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else if (q.answer_type == "string_multi") {
		string a;
		cin >> a;
		for (size_t i = 0; i < q.answer_strings.size(); i++) {
			if (a == q.answer_strings[i]) {
				wchar_t ww3[] = L"恭喜你，答案正确";
				wprintf(L"%ls", ww3);
				cout << "\n";
				return q.reward;
			}
		}
		wchar_t ww4[] = L"答案错误！！！";
		wprintf(L"%ls", ww4);
		return 0;
	}

	wchar_t ww4[] = L"答案错误！！！";
	wprintf(L"%ls", ww4);
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

// ===== 主函数 =====
int main() {
	srand(time(0));

	// 加载题库
	loadQuestions("questions.json");

	cout << "                                              MO";
	setlocale(LC_ALL, "");
	wchar_t ws[] = L"教练模拟器";
	wprintf(L"%ls", ws);
	cout << "\n";
	_sleep(tim / 4);
	wchar_t wws[] = L"如果您想开始游戏，请输入1，否则请输入2。";
	wprintf(L"%ls", wws);
	cout << "\n";
	safe_cin_int(opt, L"输入无效，请重新输入（1开始，2退出）：");
	if (opt == 2) {
		system("CLS");
		wchar_t s[] = L"再见！";
		wprintf(L"%ls", s);
		cout << "\n";
		_sleep(tim);
		system("CLS");
		return 0;
	} else {
		system("CLS");
		wchar_t w[] = L"                                        开始游戏!";
		wprintf(L"%ls", w);
		cout << "\n";
		_sleep(tim);
		wchar_t ww[] = L"背景描述：时光荏苒，白云苍狗。不知不觉中，你教的学生已到了高一。而你，作为xx中学的数学竞赛主教练，将指导学生参加比赛，并争取获得成绩。";
		wprintf(L"%ls", ww);
		cout << "\n";
		_sleep(tim * 3);
		wchar_t w1[] = L"游戏介绍：这是一款模拟类游戏，玩家需根据状况做出选择，并尽量达成游戏目标";
		wprintf(L"%ls", w1);
		cout << "\n";
		_sleep(tim * 2);
		wchar_t www[] = L"游戏目标:协助学生AKIMO。你准备好了吗？好了请输入\"A\"（注意是大写的）";
		wprintf(L"%ls", www);
		cout << "\n";
		_sleep(tim);
		while (cin >> op) {
			if (op == 'A') {
				break;
			}
		}
	}
	system("CLS");
	wchar_t w2[] = L"那么，下面请选择学生。（注：为了简便，我们设置了代数，几何，数论，组合这四个能力指标，并有优，良，中，差四个评级）";
	wprintf(L"%ls", w2);
	cout << "\n";
	_sleep(tim);
	wchar_t w3[] = L"a.甲子：代数：差，几何：中，数论：中，组合：良";
	wchar_t w4[] = L"b.乙丑：代数：中，几何：差，数论：中，组合：差";
	wchar_t w5[] = L"c.丙寅：代数：差，几何：差，数论：中，组合：差";
	wchar_t w6[] = L"d.丁卯：代数：差，几何：良，数论：差，组合：差";
	wchar_t w7[] = L"你可以选择2个学生，输入他们的番号，一行一个，并在输入结束后输入\"A\"";
	wprintf(L"%ls", w3);
	cout << "\n";
	_sleep(tim);
	wprintf(L"%ls", w4);
	cout << "\n";
	_sleep(tim);
	wprintf(L"%ls", w5);
	cout << "\n";
	_sleep(tim);
	wprintf(L"%ls", w6);
	cout << "\n";
	_sleep(tim);
	wprintf(L"%ls", w7);
	cout << "\n";
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
	wchar_t w8[] = L"好的，下面正式开始游戏";
	wprintf(L"%ls", w8);
	cout << "\n";
	_sleep(tim);
	system("CLS");
	while (dai[choose_a] < MAX_RATING || ji[choose_a] < MAX_RATING || shu[choose_a] < MAX_RATING || zu[choose_a] < MAX_RATING) {
		wchar_t w9[] = L"(这是一个循环，当且仅当你的学生的能力都为\"优\"时才退出)\n你发现你的学生有些指标较弱，便想找一道题来提升他们的能力。但是，你需要做出来才能服众。你可以选择代数，几何，数论，组合四个板块（编号1，2，3，4）中一个（输入一行1个数，为题目类型, 但难度随机）";
		wprintf(L"%ls", w9);
		cout << "\n";
		_sleep(tim);
		safe_cin_int(zhong_lei, L"输入无效，请输入1-4的数字：");
		if (zhong_lei < 1 || zhong_lei > 4) {
			wchar_t err[] = L"无效的板块编号，请输入1-4！";
			wprintf(L"%ls", err);
			cout << "\n";
			_sleep(tim);
			system("CLS");
			continue;
		}
		system("CLS");
		wchar_t w10[] = L"下面请做题吧！";
		wprintf(L"%ls", w10);
		cout << "\n";
		_sleep(tim);
		system("CLS");
		hard = (rand() % 3) + 1;

		int cat_idx = zhong_lei - 1; // 0=代数 1=几何 2=数论 3=组合
		return_zhi = do_question(cat_idx);
		_sleep(tim);
		system("CLS");

		int *rating_arr;
		if (cat_idx == 0) rating_arr = dai;
		else if (cat_idx == 1) rating_arr = ji;
		else if (cat_idx == 2) rating_arr = shu;
		else rating_arr = zu;

		rating_arr[choose_a] += return_zhi;
		rating_arr[choose_b] += return_zhi;
		clamp_rating(rating_arr[choose_a]);
		clamp_rating(rating_arr[choose_b]);

		wchar_t w00[] = L"现在，";
		wprintf(L"%ls", w00);
		cout << "\n";
		_sleep(tim);
		out(choose_a);
		cout << "\n";
		_sleep(tim);
		out(choose_b);
		cout << "\n";
		_sleep(tim * 4);
		system("CLS");
	}
	system("CLS");
	wchar_t w11[] = L"恭喜你达成游戏目标：AKIMO\n游戏结束！！！！！！！！！！！！！！！！！！！！！";
	wprintf(L"%ls", w11);
	cout << "\n";
	_sleep(tim);
	return 0;
}
