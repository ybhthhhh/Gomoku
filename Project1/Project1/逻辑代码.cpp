#include "twj.h"
void DrawSecondScreen();
void DrawThirdScreen();
void DrawFourthScreen();
void DrawFirstScreen();
void DoubleRun();
void DrawSixthScreen();
void BotBattle1();
void BotBattle2();
bool Forbidden(int a,int b);

struct Button {
	int x, y;       // 按钮左上角坐标
	int width, height; // 按钮宽度和高度
	const char* text;  // 按钮文本
	COLORREF textColor; // 按钮文本颜色

	void draw() {
		setlinecolor(RGB(173, 173, 173)); // 按钮边框颜色
		setfillcolor(RGB(225, 225, 225)); // 按钮填充颜色
		bar(x, y, x + width, y + height); // 绘制矩形
		setcolor(textColor);
		settextstyle(20, 0, _T("宋体"));
		setbkmode(TRANSPARENT);
		outtextxy(x + 10, y + 10, text); // 绘制文本
	}
	//画按键

	bool isClicked(int mouseX, int mouseY) {
		return (mouseX >= x && mouseX <= x + width &&
			mouseY >= y && mouseY <= y + height);
	}
	// 判断鼠标是否点击按钮
};
//按键结构体

typedef struct point {
	int row;
	int col;
} point_t; 
//AI用结构体

void BGM() {
	mciSendString("open \"music.mp3\" type mpegvideo alias music", NULL, 0, NULL);
	mciSendString("play music repeat", NULL, 0, NULL);
}
//音乐

void change3() {
	HWND hand = GetHWnd();
	MessageBox(hand, "存档成功", "提示", MB_OKCANCEL);
}
//结算小窗口

void calculateScore() {
	int personNum = 0; // 玩家连成子的个数
	int botNum = 0; // AI连成子的个数
	int emptyNum = 0; // 各方向空白位的个数
	memset(scoreMap, 0, sizeof(scoreMap));
	for (int row = 0; row < Max; row++)
		for (int col = 0; col < Max; col++) {
			if (row >= 0 && col >= 0 && Chess_broad[row][col] == 0) {
				// 遍历周围4个方向，分别计算正反两个方向
				int directs[4][2] = { {1,0}, {1,1}, {0,1}, {-1,1 } };
				for (int k = 0; k < 4; k++) {
					int x = directs[k][0];
					int y = directs[k][1];
					personNum = 0;
					botNum = 0;
					emptyNum = 0;
					// 对黑棋评分(正向）
					for (int i = 1; i <= 4; i++) {
						if (row + i * y >= 0 && row + i * y < Max &&col + i * x >= 0 && col + i * x < Max &&Chess_broad[row + i * y][col + i * x] == 1) { // 真人玩家的子
							personNum++;
						}
						else if (row + i * y >= 0 && row + i * y < Max &&col + i * x >= 0 && col + i * x < Max &&Chess_broad[row + i * y][col + i * x] == 0) { // 空白位
							emptyNum++;
							break;     // 遇到空白位置，停止该方向的搜索
						}
						else            // 出边界，或者遇到白棋，就停止该方向的搜索
							break;
					}
					// 对黑棋评分(反向）
					for (int i = 1; i <= 4; i++) {
						if (row - i * y >= 0 && row - i * y < Max &&col - i * x >= 0 && col - i * x < Max &&Chess_broad[row - i * y][col - i * x] == 1) { // 玩家的子
							personNum++;
						}
						else if (row - i * y >= 0 && row - i * y < Max &&col - i * x >= 0 && col - i * x < Max &&Chess_broad[row - i * y][col - i * x] == 0) { // 空白位
							emptyNum++;
							break;
						}
						else            // 出边界，或者有AI自己的棋子
							break;
					}
					if (personNum == 1)                      // 杀二
						scoreMap[row][col] += 10;
					else if (personNum == 2) {                // 杀三
						if (emptyNum == 1)     // 死三
							scoreMap[row][col] += 30;
						else if (emptyNum == 2) // 活三
							scoreMap[row][col] += 40;
					}
					else if (personNum == 3) {              // 杀四
						if (emptyNum == 1)    //死四
							scoreMap[row][col] += 60;
						else if (emptyNum == 2) //活四
							scoreMap[row][col] += 200;
					}
					else if (personNum == 4)                 // 杀五
						scoreMap[row][col] += 20000;
					emptyNum = 0;
					// 对白棋评分(正向）
					for (int i = 1; i <= 4; i++) {
						if (row + i * y > 0 && row + i * y < Max &&col + i * x > 0 && col + i * x < Max &&Chess_broad[row + i * y][col + i * x] == 2) { // 玩家的子
							botNum++;
						}
						else if (row + i * y > 0 && row + i * y < Max &&col + i * x > 0 && col + i * x < Max &&Chess_broad[row + i * y][col + i * x] == 0) { // 空白位
							emptyNum++;
							break;
						}
						else
							break;
					}
					// 对白棋评分(反向）
					for (int i = 1; i <= 4; i++) {
						if (row - i * y > 0 && row - i * y < Max &&col - i * x > 0 && col - i * x < Max &&Chess_broad[row - i * y][col - i * x] == 2) { // AI的子
							botNum++;
						}
						else if (row - i * y > 0 && row - i * y < Max &&col - i * x > 0 && col - i * x < Max &&Chess_broad[row - i * y][col - i * x] == 0) { // 空白位
							emptyNum++;
							break;
						}
						else            // 出边界
							break;
					}
					if (botNum == 0)                      // 普通下子
						scoreMap[row][col] += 5;
					else if (botNum == 1)                 // 活二
						scoreMap[row][col] += 10;
					else if (botNum == 2) {
						if (emptyNum == 1)                // 死三
							scoreMap[row][col] += 25;
						else if (emptyNum == 2)
							scoreMap[row][col] += 50;  // 活三
					}
					else if (botNum == 3) {
						if (emptyNum == 1)                // 死四
							scoreMap[row][col] += 55;
						else if (emptyNum == 2)
							scoreMap[row][col] += 300; // 活四
					}
					else if (botNum >= 4)
						scoreMap[row][col] += 30000;   // 活五，应该具有最高优先级
				}       
			}
		}
}
//计算权值 先手黑棋

void calculateScore_BLACK() {
	int personNum = 0; // 玩家连成子的个数
	int botNum = 0; // AI连成子的个数
	int emptyNum = 0; // 各方向空白位的个数
	memset(scoreMap, 0, sizeof(scoreMap));
	for (int row = 0; row < Max; row++)
		for (int col = 0; col < Max; col++) {
			if (row >= 0 && col >= 0 && Chess_broad[row][col] == 0) {
				if (Forbidden(row, col)) {
					scoreMap[row][col] = -100000;
					continue;
				}
				// 遍历周围4个方向，分别计算正反两个方向
				int directs[4][2] = { {1,0}, {1,1}, {0,1}, {-1,1 } };
				for (int k = 0; k < 4; k++) {
					int x = directs[k][0];
					int y = directs[k][1];
					personNum = 0;
					botNum = 0;
					emptyNum = 0;
					// 对白棋评分(正向）
					for (int i = 1; i <= 4; i++) {
						if (row + i * y >= 0 && row + i * y < Max && col + i * x >= 0 && col + i * x < Max && Chess_broad[row + i * y][col + i * x] == 2) { // 真人玩家的子
							personNum++;
						}
						else if (row + i * y >= 0 && row + i * y < Max && col + i * x >= 0 && col + i * x < Max && Chess_broad[row + i * y][col + i * x] == 0) { // 空白位
							emptyNum++;
							break;     // 遇到空白位置，停止该方向的搜索
						}
						else            // 出边界，或者遇到白棋，就停止该方向的搜索
							break;
					}
					// 对白棋评分(反向）
					for (int i = 1; i <= 4; i++) {
						if (row - i * y >= 0 && row - i * y < Max && col - i * x >= 0 && col - i * x < Max && Chess_broad[row - i * y][col - i * x] == 2) { // 玩家的子
							personNum++;
						}
						else if (row - i * y >= 0 && row - i * y < Max && col - i * x >= 0 && col - i * x < Max && Chess_broad[row - i * y][col - i * x] == 0) { // 空白位
							emptyNum++;
							break;
						}
						else            // 出边界，或者有AI自己的棋子
							break;
					}
					if (personNum == 1)                      // 杀二
						scoreMap[row][col] += 10;
					else if (personNum == 2) {                // 杀三
						if (emptyNum == 1)     // 死三
							scoreMap[row][col] += 30;
						else if (emptyNum == 2) // 活三
							scoreMap[row][col] += 40;
					}
					else if (personNum == 3) {              // 杀四
						if (emptyNum == 1)    //死四
							scoreMap[row][col] += 60;
						else if (emptyNum == 2) //活四
							scoreMap[row][col] += 200;
					}
					else if (personNum == 4)                 // 杀五
						scoreMap[row][col] += 20000;
					emptyNum = 0;
					// 对黑棋评分(正向）
					for (int i = 1; i <= 4; i++) {
						if (row + i * y > 0 && row + i * y < Max && col + i * x > 0 && col + i * x < Max && Chess_broad[row + i * y][col + i * x] == 1) { // 玩家的子
							botNum++;
						}
						else if (row + i * y > 0 && row + i * y < Max && col + i * x > 0 && col + i * x < Max && Chess_broad[row + i * y][col + i * x] == 0) { // 空白位
							emptyNum++;
							break;
						}
						else
							break;
					}
					// 对黑棋评分(反向）
					for (int i = 1; i <= 4; i++) {
						if (row - i * y > 0 && row - i * y < Max && col - i * x > 0 && col - i * x < Max && Chess_broad[row - i * y][col - i * x] == 1) { // AI的子
							botNum++;
						}
						else if (row - i * y > 0 && row - i * y < Max && col - i * x > 0 && col - i * x < Max && Chess_broad[row - i * y][col - i * x] == 0) { // 空白位
							emptyNum++;
							break;
						}
						else            // 出边界
							break;
					}
					if (botNum == 0)                      // 普通下子
						scoreMap[row][col] += 5;
					else if (botNum == 1)                 // 活二
						scoreMap[row][col] += 10;
					else if (botNum == 2) {
						if (emptyNum == 1)                // 死三
							scoreMap[row][col] += 25;
						else if (emptyNum == 2)
							scoreMap[row][col] += 50;  // 活三
					}
					else if (botNum == 3) {
						if (emptyNum == 1)                // 死四
							scoreMap[row][col] += 55;
						else if (emptyNum == 2)
							scoreMap[row][col] += 300; // 活四
					}
					else if (botNum >= 4)
						scoreMap[row][col] += 30000;   // 活五，应该具有最高优先级
				}
			}
		}
}
//计算权值 后手白棋

bool Forbidden(int row,int col) {
	int count1 = 0;int count2 = 0;int count3 = 0;
	int directs[4][2] = { {1,0}, {1,1}, {0,1}, {-1,1 } };
	for (int k = 0; k < 4; k++) {
		int x = directs[k][0];
		int y = directs[k][1];
		int personNum = 0;     
		int botNum = 0;
		int emptyNum = 0; 
		int LeftNum = 0;
		int RightNum = 0;
		int Mark = 0;//标记是否遇到过一次空格
		for (int i = 1; i <= 4; i++) {
			if (row + i * y >= 0 && row + i * y < Max && col + i * x >= 0 && col + i * x < Max && Chess_broad[row + i * y][col + i * x] == 1) { // 真人玩家的子
				if (Mark == 0)
					personNum++;
				else
					LeftNum++;
			}
			else if (row + i * y >= 0 && row + i * y < Max && col + i * x >= 0 && col + i * x < Max && Chess_broad[row + i * y][col + i * x] == 0) { // 空白位
				if (Mark == 0) {
					emptyNum++;
					Mark = 1;
				}
				else break;     // 遇到2次空白位置，停止该方向的搜索
			}
			else            // 出边界，或者遇到白棋，就停止该方向的搜索
				break;
		}
		Mark = 0;
		for (int i = 1; i <= 4; i++) {
			if (row - i * y >= 0 && row - i * y < Max && col - i * x >= 0 && col - i * x < Max && Chess_broad[row - i * y][col - i * x] == 1) { // 玩家的子
				if (Mark == 0)
					personNum++;
				else
					RightNum++;
			}
			else if (row - i * y >= 0 && row - i * y < Max && col - i * x >= 0 && col - i * x < Max && Chess_broad[row - i * y][col - i * x] == 0) { // 空白位
				if (Mark == 0) {
					emptyNum++;
					Mark = 1;
				}
				else break;
			}
			else            
				break;
		}
		if (personNum + RightNum == 3 || personNum + LeftNum == 3)
			count2++;//跳四
		if (personNum == 2 && emptyNum == 2)
			count1++;  //活三
		if (personNum == 3)
			count2++;  //死四 活四
		if (personNum == 5)
			count3++;  //长连
	}  //活三 死四 活四 长连判断
	if (count1 >= 2||count2>=2||count3>=1)
		return true;
	else return false;
}
//禁手

Button button3 = { 800, 400, 100, 45, "  退出  ", BLACK };
Button button4 = { 800, 300, 100, 45, "  清空  ", BLACK };
Button button5 = { 800, 200, 100, 45, "  存档  ", BLACK };
Button button6 = { 600, 250, 115, 45, "  存档1  ", BLACK };
Button button7 = { 600, 325, 115, 45, "  存档2  ", BLACK };
Button button8 = { 600, 400, 115, 45, "  存档3  ", BLACK };
Button button9 = { 600, 475, 115, 45, "   退出  ", BLACK };
Button button15 = { 600, 250, 115, 45, "双人对战", BLACK };
Button button16 = { 600, 325, 115, 45, "人机对战", BLACK };
Button button17 = { 600, 400, 115, 45, "   返回  ", BLACK };

void saveArrayToFile(const std::string& filename) {
	ofstream outFile(filename, ios::binary);
	outFile.write(reinterpret_cast<const char*>(Chess_broad), sizeof(int) * Max * Max);
	outFile.close();
	change3();
}
//双人对战单个存档

void loadArrayFromFile(const std::string& filename) {
	ifstream inFile(filename, ios::binary);
	inFile.read(reinterpret_cast<char*>(Chess_broad), sizeof(int) * Max * Max);
	inFile.close();
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage1;
	IMAGE mage2;
	loadimage(&mage1, "背景.png");
	loadimage(&mage2, "棋盘.jpg");
	putimage(0, 0, &mage1);
	putimage(0, 0, &mage2);      //棋盘图片
	button3.draw();
	button4.draw();
	button5.draw();
	Step = 0;
	for (int i = 0;i < Max;i++) {
		for (int j = 0;j < Max;j++) {
			if (Chess_broad[i][j] == 1) {
				Step++;
				setlinecolor(BLACK);
				setfillcolor(BLACK);
				fillcircle(28 + i * Unit, 28 + j * Unit, 20);
			}
			else if (Chess_broad[i][j] == 2) {
				Step++;
				setlinecolor(WHITE);
				setfillcolor(WHITE);
				fillcircle(28 + i * Unit, 28 + j * Unit, 20);
				x_m = i;y_m = j;
			}
		}
	}
	DoubleRun();
	DrawFirstScreen();
}
//双人对战单个读档 并加载页面

void loadArrayFromFile0(const std::string& filename) {
	ifstream inFile(filename, ios::binary);
	inFile.read(reinterpret_cast<char*>(Chess_broad), sizeof(int) * Max * Max);
	inFile.close();
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage1;
	IMAGE mage2;
	loadimage(&mage1, "背景.png");
	loadimage(&mage2, "棋盘.jpg");
	putimage(0, 0, &mage1);
	putimage(0, 0, &mage2);      //棋盘图片
	button3.draw();
	button4.draw();
	button5.draw();
	Step = 0;
	for (int i = 0;i < Max;i++) {
		for (int j = 0;j < Max;j++) {
			if (Chess_broad[i][j] == 1) {
				Step++;
				setlinecolor(BLACK);
				setfillcolor(BLACK);
				fillcircle(28 + i * Unit, 28 + j * Unit, 20);
			}
			else if (Chess_broad[i][j] == 2) {
				Step++;
				setlinecolor(WHITE);
				setfillcolor(WHITE);
				fillcircle(28 + i * Unit, 28 + j * Unit, 20);
				x_m = i;y_m = j;
			}
		}
	}
	if (Step%2==0)
	BotBattle1();
	else BotBattle2();
	DrawFirstScreen();
}
//人机对战单个读档 并加载页面

void Save() {
	while (1) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			if (button6.isClicked(m.x, m.y)) {
				saveArrayToFile("双人对战存档1");
			}
			else if (button7.isClicked(m.x, m.y)) {
				saveArrayToFile("双人对战存档2");
			}
			else if (button8.isClicked(m.x, m.y)) {
				saveArrayToFile("双人对战存档3");
			}
			else if (button9.isClicked(m.x, m.y)) {
				DrawFirstScreen();
				break;
			}
		}
	}
}
//双人对战多按键存档中转

void Save0() {
	while (1) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			if (button6.isClicked(m.x, m.y)) {
				saveArrayToFile("人机对战存档1");
			}
			else if (button7.isClicked(m.x, m.y)) {
				saveArrayToFile("人机对战存档2");
			}
			else if (button8.isClicked(m.x, m.y)) {
				saveArrayToFile("人机对战存档3");
			}
			else if (button9.isClicked(m.x, m.y)) {
				DrawFirstScreen();
				break;
			}
		}
	}
}
//人机对战多按键存档中转

void Load() {
	while (1) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			if (button6.isClicked(m.x, m.y)) {
				loadArrayFromFile("双人对战存档1");
			}
			else if (button7.isClicked(m.x, m.y)) {
				loadArrayFromFile("双人对战存档2");
			}
			else if (button8.isClicked(m.x, m.y)) {
				loadArrayFromFile("双人对战存档3");
			}
			else if (button9.isClicked(m.x, m.y)) {
				DrawFirstScreen();
				break;
			}
		}
	}
}
//双人对战多按键读档中转

void Load0() {
	while (1) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			if (button6.isClicked(m.x, m.y)) {
				loadArrayFromFile0("人机对战存档1");
			}
			else if (button7.isClicked(m.x, m.y)) {
				loadArrayFromFile0("人机对战存档2");
			}
			else if (button8.isClicked(m.x, m.y)) {
				loadArrayFromFile0("人机对战存档3");
			}
			else if (button9.isClicked(m.x, m.y)) {
				DrawFirstScreen();
				break;
			}
		}
	}
}
//人机对战多按键读档中转

void Middle(COLORREF Color,int a) {
	if (Chess_broad[x_m][y_m] == 0 && x_m < Max && y_m < Max) {
		Chess_broad[x_m][y_m] = a;
		setlinecolor(Color);
		setfillcolor(Color);
		fillcircle(28 + x_m * Unit, 28 + y_m * Unit, 20);
		Step++;
	}
	else if (button3.isClicked(m.x, m.y)) {
		DrawFirstScreen();
	}
	else if (button4.isClicked(m.x, m.y)) {
		memset(Chess_broad, 0, sizeof(Chess_broad));
		DrawThirdScreen();
	}
	else if (button5.isClicked(m.x, m.y)) {
		DrawFourthScreen();
		Save();
	}
}
//双人对战界面按键

void Middle0(COLORREF Color, int a){
	if (Chess_broad[x_m][y_m] == 0 && x_m < Max && y_m < Max) {
		Chess_broad[x_m][y_m] = a;
		setlinecolor(Color);
		setfillcolor(Color);
		fillcircle(28 + x_m * Unit, 28 + y_m * Unit, 20);
		Step++;
	}
	else if (button3.isClicked(m.x, m.y)) {
		DrawFirstScreen();
	}
	else if (button4.isClicked(m.x, m.y)) {
		memset(Chess_broad, 0, sizeof(Chess_broad));
		DrawSixthScreen();
	}
	else if (button5.isClicked(m.x, m.y)) {
		DrawFourthScreen();
		Save0();
	}
}
//人机对战界面按键 

void Mouse1() {
	while (1) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			x_m = (int)(((double)m.x - 28) / Unit + 0.5);
			y_m = (int)(((double)m.y - 28) / Unit + 0.5);
			Middle(BLACK, 1);
			break;
		}
	}
} 
//下黑棋  引向按键  双人对战

void Mouse3() {
	while (1) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			x_m = (int)(((double)m.x - 28) / Unit + 0.5);
			y_m = (int)(((double)m.y - 28) / Unit + 0.5);
			Middle0(BLACK, 1);
			break;
		}
	}
}
//下黑棋  引向按键  人机对战

void Mouse4() {
	while (1) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			x_m = (int)(((double)m.x - 28) / Unit + 0.5);
			y_m = (int)(((double)m.y - 28) / Unit + 0.5);
			Middle0(WHITE, 2);
			break;
		}
	}
}
//下黑棋  引向按键  人机对战

void Mouse2() {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			x_m = (int)(((double)m.x - 28) / Unit + 0.5);
			y_m = (int)(((double)m.y - 28) / Unit + 0.5);
			Middle(WHITE,2);
		}
}   
//下白棋  引向按键  双人对战

point_t actionByAI(){
	calculateScore();
	int maxScore = 0;
	point_t maxPoints[Max * Max] = { 0, };
	int k = 0;
	for (int row = 0; row < Max; row++)
		for (int col = 0; col < Max; col++)
		{
			if (Chess_broad[row][col] == 0){
				if (scoreMap[row][col] > maxScore){
					memset(maxPoints, 0, sizeof(maxPoints));
					k = 0;
					maxScore = scoreMap[row][col];
					maxPoints[k].row = row;
					maxPoints[k].col = col;
					k++;
				}
				else if (scoreMap[row][col] == maxScore) {   // 如果有多个最大的数，都存起来
					maxPoints[k].row = row;
					maxPoints[k].col = col;
					k++;
				}
			}
		}
	srand((unsigned)time(0));
	int index = rand() % k;
	return maxPoints[index];
}
//根据得分返回AI应下坐标

point_t actionByAI_BLACK() {
	calculateScore_BLACK();
	int maxScore = 0;
	point_t maxPoints[Max * Max] = { 0, };
	int k = 0;
	for (int row = 0; row < Max; row++)
		for (int col = 0; col < Max; col++)
		{
			if (Chess_broad[row][col] == 0) {
				if (scoreMap[row][col] > maxScore) {
					memset(maxPoints, 0, sizeof(maxPoints));
					k = 0;
					maxScore = scoreMap[row][col];
					maxPoints[k].row = row;
					maxPoints[k].col = col;
					k++;
				}
				else if (scoreMap[row][col] == maxScore) {   // 如果有多个最大的数，都存起来
					maxPoints[k].row = row;
					maxPoints[k].col = col;
					k++;
				}
			}
		}
	srand((unsigned)time(0));
	int index = rand() % k;
	return maxPoints[index];
}
//根据得分返回AI应下坐标

void AI_WHITE() {
	point_t point = actionByAI();
	x_m = point.row;
	y_m = point.col;
	Chess_broad[x_m][y_m] = 2;
	setlinecolor(WHITE);
	setfillcolor(WHITE);
	fillcircle(28 + x_m * Unit, 28 + y_m * Unit, 20);
}
//人机 白棋

void AI_BLACK() {
	point_t point = actionByAI_BLACK();
	x_m = point.row;
	y_m = point.col;
	Chess_broad[x_m][y_m] = 1;
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillcircle(28 + x_m * Unit, 28 + y_m * Unit, 20);
}
//人机 黑棋

bool Judge(int a,int b) {
	if (Chess_broad[a][b] == 0)
		return false;
	int num = 0;
	for (int i = a+1;i<15;i++) {
		if (Chess_broad[i][b] != Chess_broad[a][b])
			break;
		else num++;
	}
	for (int i = a-1;i >=0;i--) {
		if (Chess_broad[i][b] != Chess_broad[a][b])
			break;
		else num++;
	}
	if (num == 4)
		return 1;     //纵向
	num = 0;
	for (int i = b + 1;i < 15;i++) {
		if (Chess_broad[a][i] != Chess_broad[a][b])
			break;
		else num++;
	}
	for (int i = b - 1;i >= 0;i--) {
		if (Chess_broad[a][i] != Chess_broad[a][b])
			break;
		else num++;
	}
	if (num == 4)
		return 1;    //横向
	num = 0;
	for (int i = 1;a + i < 15 && b + i < 15;i++) {
		if (Chess_broad[a + i][b + i] != Chess_broad[a][b])
			break;
		else num++;
	}
	for (int i = 1;a - i >= 0 && b - i >= 0;i++) {
		if (Chess_broad[a - i][b - i] != Chess_broad[a][b])
			break;
		else num++;
	}
	if (num == 4)
		return 1;   //右上左下
	num = 0;
	for (int i = 1;a + i < 15 && b - i >= 0;i++) {
		if (Chess_broad[a + i][b - i] != Chess_broad[a][b])
			break;
		else num++;
	}
	for (int i = 1;a - i >= 0 && b + i < 15;i++) {
		if (Chess_broad[a - i][b + i] != Chess_broad[a][b])
			break;
		else num++;
	}
	if (num == 4)
		return 1;   //左上右下
	return 0;
}        
//判断五子

void change1() {
	HWND hand = GetHWnd();
	MessageBox(hand, "黑棋获胜", "提示", MB_OKCANCEL);
	DrawFirstScreen();
}
void change2() {
	HWND hand = GetHWnd();
	MessageBox(hand, "白棋获胜", "提示", MB_OKCANCEL);
	DrawFirstScreen();
}        
void change4() {
	HWND hand = GetHWnd();
	MessageBox(hand, "违反禁手 白棋获胜", "提示", MB_OKCANCEL);
	DrawFirstScreen();
}
//结算小窗口

void DoubleRun() {
	while (1) {
		if (Step % 2 == 0) {
			Mouse1();
			if (Forbidden(x_m, y_m)) {
				change4();
				break;
			 }
			 if (Judge(x_m, y_m)) {
				change1();
				break;
			 }
		}
		else {
			Mouse2();
			if (Judge(x_m, y_m)) {
				change2();
				break;
			}
		}
	}  //下棋 
}
//双人对战

void DrawFourthScreen() {
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage;
	loadimage(&mage, "初始界面.png");
	putimage(0, 0, &mage);
	button6.draw();
	button7.draw();
	button8.draw();
	button9.draw();
}
//画存盘 读盘界面

void DrawThirdScreen() {
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage1;
	IMAGE mage2;
	loadimage(&mage1, "背景.png");
	loadimage(&mage2, "棋盘.jpg");
	putimage(0, 0, &mage1);
	putimage(0, 0, &mage2);      //棋盘图片
	button3.draw();
	button4.draw();
	button5.draw();
	Step = 0;
	DoubleRun();
}
//画棋盘 (引向双人对战)

void BotBattle1() {
	Step = 0;
	while (1) {
		if (Step % 2 == 0) {
			Mouse3();
			if (Forbidden(x_m, y_m)) {
				change4();
				break;
			}
			if (Judge(x_m, y_m)) {
				change1();
				break;
			}
		}
		else {
			AI_WHITE();
			if (Judge(x_m, y_m)) {
				change2();
				break;
			}
			Step++;
		}
	}  //下棋 
}
//人机对战先手黑棋

void BotBattle2() {
	if (Chess_broad[7][7] != 1) {
		Chess_broad[7][7] = 1;
		setlinecolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(28 + 7 * Unit, 28 + 7 * Unit, 20);
		Step = 1;
	}
	while (1) {
		if (Step % 2 == 0) {
			AI_BLACK();
			if (Judge(x_m, y_m)) {
				change1();
				break;
			}
			Step++;
		}
		else {
			Mouse4();
			if (Judge(x_m, y_m)) {
				change2();
				break;
			}
		}
	}  //下棋 
}
//人机对战后手白棋

void DrawSixthScreen() {
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage1;
	IMAGE mage2;
	loadimage(&mage1, "背景.png");
	loadimage(&mage2, "棋盘.jpg");
	putimage(0, 0, &mage1);
	putimage(0, 0, &mage2);      //棋盘图片
	button3.draw();
	button4.draw();
	button5.draw();
	BotBattle1();
}
//画棋盘 (引向先手黑棋人机对战)

void Draw9thScreen() {
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage1;
	IMAGE mage2;
	loadimage(&mage1, "背景.png");
	loadimage(&mage2, "棋盘.jpg");
	putimage(0, 0, &mage1);
	putimage(0, 0, &mage2);
	button3.draw();
	button4.draw();
	button5.draw();
	BotBattle2();
}
//画棋盘 (引向后手白棋人机对战)

void Draw8thScreen() {
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage;
	loadimage(&mage, "初始界面.png");
	putimage(0, 0, &mage);
	Button button18 = { 600, 250, 100, 45, "先手黑棋", BLACK };
	Button button19 = { 600, 320, 100, 45, "后手白棋", BLACK };
	Button button20 = { 600, 400, 100, 45, "  返回  ", BLACK };
	button18.draw();
	button19.draw();
	button20.draw();   //画按键
	while (1) {
		if (MouseHit()) { // 检查是否有鼠标事件
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN) { // 左键点击
				if (button18.isClicked(msg.x, msg.y)) {
					DrawSixthScreen();
				}  //先手黑棋
				else if (button19.isClicked(msg.x, msg.y)) {
					Draw9thScreen();
				}  //后手白棋
				else if (button20.isClicked(msg.x, msg.y)) {
					DrawFirstScreen();
				}         //返回
			}
		}
	}
}
//选择先手黑棋 后手白棋

void DrawSecondScreen() {
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage;
	loadimage(&mage, "初始界面.png");
	putimage(0, 0, &mage);
	Button button1 = { 600, 250, 100, 45, "双人对战", BLACK };
	Button button2 = { 600, 400, 100, 45, "  返回  ", BLACK };
	Button button11 = { 600, 320, 100, 45, "人机对战", BLACK };
	button11.draw();
	button2.draw();
	button1.draw();   //画按键
	while (1) {
		if (MouseHit()) { // 检查是否有鼠标事件
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN) { // 左键点击
				if (button1.isClicked(msg.x, msg.y)) {
					DrawThirdScreen();
					break;
				}
				else if (button2.isClicked(msg.x, msg.y)) {
					DrawFirstScreen();
					break;
				}
				else if (button11.isClicked(msg.x, msg.y)) {
					//DrawSixthScreen();
					Draw8thScreen();
				}
			}
		}
	}
}
//双人对战 人机对战选择

void DrawSeventhScreen() {
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage;
	loadimage(&mage, "初始界面.png");
	putimage(0, 0, &mage);
	button15.draw();
	button16.draw();
	button17.draw();
	while (1) {
		if (MouseHit()) { // 检查是否有鼠标事件
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN) { // 左键点击
				if (button17.isClicked(msg.x, msg.y)) {
					DrawFirstScreen();  
				}//返回
				else if (button15.isClicked(msg.x, msg.y)) {
					DrawFourthScreen();
					Load();
				}//双人对战读档
				else if (button16.isClicked(msg.x, msg.y)) {
					DrawFourthScreen();
					Load0();
				}//人机对战读档
			}
		}
	}
}

void DrawFirstScreen() {
	memset(Chess_broad, 0, sizeof(Chess_broad));
	setbkcolor(WHITE);
	cleardevice();
	IMAGE mage;
	loadimage(&mage, "初始界面.png");
	putimage(0, 0, &mage);
	Button button1 = { 600, 250, 100, 45, "开始游戏", BLACK };
	Button button2 = { 600, 400, 100, 45, "  退出  ", BLACK };
	Button button10 = { 600, 325, 100, 45, "  读档  ", BLACK };
	button2.draw();
	button10.draw();
	button1.draw();   //画按键
	while (1) {
		if (MouseHit()) { // 检查是否有鼠标事件
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN) { // 左键点击
				if (button1.isClicked(msg.x, msg.y)) {
					DrawSecondScreen();
					break;  //开始游戏
				}
				else if (button2.isClicked(msg.x, msg.y)) {
					closegraph();
					break;     //退出
				}
				else if (button10.isClicked(msg.x, msg.y)) {
					DrawSeventhScreen();  
				}         //读档
			}
		}
	}
}
//画初始界面