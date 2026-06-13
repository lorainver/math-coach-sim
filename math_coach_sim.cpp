#include<bits/stdc++.h>
using namespace std;
long unsigned int tim = 1000;
int opt, zhong_lei;
int dai[1000] = {0, 1, 2, 1, 1}, ji[1000] = {0, 2, 1, 1, 3}, shu[1000] = {0, 2, 2, 2, 1}, zu[1000] = {0, 3, 1, 1, 1};
char op, ca;
int return_zhi = 0, choose_a = 0, choose_b = 0, hard = 1;
const int MAX_RATING = 4;

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

int ds() {
	if (hard == 1) {
		int a, b;
		wchar_t ww1[] = L"这是一道十分简单的代数题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"解方程：x^2+3x+2=0 ，请输入其的两根，从小到大";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a >> b;
		if (a == -2 && b == -1) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 1;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else if (hard == 2) {
		int a;
		wchar_t ww1[] = L"这是一道中等难度的代数题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"(1997全国高联试题）已知实数x,y满足：(x+1)^3+1997(x+1)=-1,(y+1)^3+1997(y+1)=1,则x+y=";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 2) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 1;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else {
		int a;
		wchar_t ww1[] = L"这是一道比较难的代数题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"（2025全国高联一试 填空5）f(x)是定义域为R的函数，g(x)=(x-1)f(x)，h(x)=f(x)+x，若g(x)为奇函数，h(x)为偶函数，记f(x)在R上的最大值为m，则4*m=？";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 1) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 2;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	}
	return 0;
}

int jh() {
	if (hard == 1) {
		int a;
		wchar_t ww1[] = L"这是一道十分简单的几何题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"在三角形ABC中，角A=25度，角B=35度，那么角C=多少度？";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 120) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 1;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else if (hard == 2) {
		int a;
		wchar_t ww1[] = L"这是一道中等难度的几何题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"在四边形ABCD中，连结对角线AC，BD交于M，角BAC=角BDC=90度，AB=AC=根号5，CD=1，则4*DM=？";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 2) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 1;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else {
		int a;
		wchar_t ww1[] = L"这是一道非常非非常常非常非常难的几何题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"（2025全国高联一试试题)正四面体ABCD各棱长为2，，P，Q，分别是棱AB，AC，上的动点（允许位于棱端点），AP+AQ=2，M为棱AD的中点。在三角形MPQ中，MH是PQ的高。设MH的最小值为x,3x^2=?";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 6) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 2;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	}
	return 0;
}

int sl() {
	if (hard == 1) {
		int a;
		wchar_t ww1[] = L"这是一道十分简单的数论题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"多位数123456789101112...202420252026除以9的余数为？";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 1) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 1;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else if (hard == 2) {
		int a;
		wchar_t ww1[] = L"这是一道中等难度的数论题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"方程 3^x+4^x=5^x 的正整数解有多少个（无解输出0，无穷解输出-1）？";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 1) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 1;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else {
		string a;
		wchar_t ww1[] = L"这是一道比较难的数论题（也许吧）：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"因式分解：x^3+4x^2+5x+2 （注：本题设置spj,须按\"()()\"的格式输入，例：(x+1)(x^2+1)(x+1)亦可输入(x+1)(x^2+1)(x+1)）";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == "(x+1)(x+1)(x+2)" || a == "(x+1)(x+2)(x+1)" || a == "(x+2)(x+1)(x+1)") {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 2;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	}
	return 0;
}

int zh() {
	if (hard == 1) {
		int a;
		wchar_t ww1[] = L"这是一道并不十分简单的组合题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"设r=p/q;0<r<1;p,q为正整数且(p,q)=1;pq|3600;求满足条件的r的个数?";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 112) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 1;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else if (hard == 2) {
		int a;
		wchar_t ww1[] = L"这是一道中等难度的组合题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"（2024全国高联一试试题）若三个正整数a,b,c的位数之和为8，且组成a,b,c的8个数码能排列为2，0，2，4，0，9，0，8，则称(a,b,c)为「幸运数组」，例如（9，8，202400）为「幸运数组」，则满足10<a<b<c的幸运数组个数为？";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 591) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 1;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	} else {
		int a;
		wchar_t ww1[] = L"这是一道非常非常非常非常非常非常非常非常难的组合题：";
		wprintf(L"%ls", ww1);
		cout << "\n";
		_sleep(tim);
		wchar_t ww2[] = L"（2025 CMO 试题）设n是正整数，有n张红色卡片与蓝色卡片，最初每张红色卡片上都写有一个实数0，每张蓝色卡片上都写有一个实数1，一次操作是指：选择一张红色卡片与一张蓝色卡片，满足红色卡片上的实数x小于蓝色卡片上的实数y，将这两个实数擦去，并在这两张卡片上都写下实数（x+y)/2,求最小的正整数n，使得可以适当地进行有限次操作，让所有n张红色卡片上的实数之和都大于100？";
		wprintf(L"%ls", ww2);
		cout << "\n";
		cin >> a;
		if (a == 106) {
			wchar_t ww3[] = L"恭喜你，答案正确";
			wprintf(L"%ls", ww3);
			cout << "\n";
			return 2;
		} else {
			wchar_t ww4[] = L"答案错误！！！";
			wprintf(L"%ls", ww4);
			return 0;
		}
	}
	return 0;
}

void safe_cin_int(int &var, const wchar_t *prompt) {
	while (!(cin >> var)) {
		cin.clear();
		cin.ignore(10000, '\n');
		wprintf(L"%ls", prompt);
	}
}

int main() {
	srand(time(0));
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
		if (zhong_lei == 1) {
			return_zhi = ds();
			_sleep(tim);
			system("CLS");
			dai[choose_a] += return_zhi;
			dai[choose_b] += return_zhi;
			clamp_rating(dai[choose_a]);
			clamp_rating(dai[choose_b]);
		} else if (zhong_lei == 2) {
			return_zhi = jh();
			_sleep(tim);
			system("CLS");
			ji[choose_a] += return_zhi;
			ji[choose_b] += return_zhi;
			clamp_rating(ji[choose_a]);
			clamp_rating(ji[choose_b]);
		} else if (zhong_lei == 3) {
			return_zhi = sl();
			_sleep(tim);
			system("CLS");
			shu[choose_a] += return_zhi;
			shu[choose_b] += return_zhi;
			clamp_rating(shu[choose_a]);
			clamp_rating(shu[choose_b]);
		} else {
			return_zhi = zh();
			_sleep(tim);
			system("CLS");
			zu[choose_a] += return_zhi;
			zu[choose_b] += return_zhi;
			clamp_rating(zu[choose_a]);
			clamp_rating(zu[choose_b]);
		}
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
