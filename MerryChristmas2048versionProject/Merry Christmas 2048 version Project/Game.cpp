#include "stdafx.h"
#include "Game.h"
#include "resource.h"

//#pragma comment(linker, "/subsystem:\"windows\"   /entry:\"mainCRTStartup\" ")​


/*
	
*/

int cGame::m_score = 0;
time_t Time;

cGame::~cGame()
{
	for (int i = 0; i < m_mode; ++i)
	{
		delete[]m_map[i];
	}
	delete[] m_map;
}
void cGame::SetGameMode(Game_mode mode)   // 通过用户的输入来创建游戏的地图大小
{
	/*  // vector<vector<int>> m_map;   使用 vector 容器构造二维数组的方法
	m_map.resize(m_mode);
	for (unsigned int i = 0; i < m_map.size(); ++i)
	{
	m_map[i].resize(m_mode);
	}
	*/										// 通过用户的输入，来在类里创建二维数组
	// 使用 new 的方式创建二维数组  m_map 为 int ** 类型
	m_mode = mode;
	m_map = new int*[m_mode];  // 先构造出 int 型指针数组，每一个元素都是指向 int 类型的指针
	for (int i = 0; i < m_mode; ++i)
	{
		m_map[i] = new int[m_mode];	  // 再为每个元素分配 m_mode 个 int 的内存
	}
	for (int i = 0; i < m_mode; ++i)
	{
		for (int j = 0; j < m_mode; ++j)
		{
			m_map[i][j] = 0;   // 把 new 出来的二维数组里的每一个元素都赋为 0
		}
	}
}
void cGame::CreateGame()
{
	/*
	首页的选择模式窗口 与 游戏界面的窗口一样，都为: HEIGHT(750)，WIDTH(650)
	宽度的分解: WIDTH(650) == 100 + 200 + 50 + 200 + 100
	高度的分解: HEIGHT(750) == 290 + 60 + 150 + 60 + 142

	4x4 左顶点 与 右下点坐标(按照数学的坐标系计算):  (100,290) -- (300,350)
	5x5 左顶点 与 右下点坐标(按照数学的坐标系计算):  (350,290) -- (550,350)
	6x6 左顶点 与 右下点坐标(按照数学的坐标系计算):  (100,500) -- (300,560)
	7x7 左顶点 与 右下点坐标(按照数学的坐标系计算):  (350,500) -- (550,560)

	*/
	const int modeWidth = 200;
	const int modeHeight = 60;
	const int lineMargin = 290;
	const int columnMargin = 100;
	const int lineSpace = 150;
	const int columnSpace = 50;
	initgraph(WIDTH, HEIGHT);
	hwnd = GetHWnd();
	SetWindowPos(this->hwnd, HWND_TOPMOST, 0, 0, WIDTH + 15, HEIGHT + 36, SWP_SHOWWINDOW);
//	MoveWindow(GetConsoleWindow(), 0, 0, WIDTH + 15, HEIGHT + 36, FALSE);
//	MoveWindow(this->hwnd, 0, 0, WIDTH + 15, HEIGHT + 36, FALSE);   // 是以窗口的最左上角为原点来计量尺寸的 
	// (而 initgraph() 是以黑窗口为原点计量的   所以宽应加上15，高加上36)
	SetWindowText(hwnd, "ASwung的2048");
	IMAGE choiceModeImg;
	loadimage(&choiceModeImg,"IMAGE", "ChoiceMode", WIDTH, HEIGHT);
	putimage(0, 0, &choiceModeImg);

	MOUSEMSG msg;

	while (1)
	{
		int IsChoice = 0;
		msg = GetMouseMsg();
		/*if (pp == 1)
		{
		msg.uMsg = WM_LBUTTONUP;
		msg.x = 380;
		msg.y = 320;
		}*/
		if (msg.uMsg == WM_LBUTTONUP)
		{
			if ((msg.x >= columnMargin && msg.x <= columnMargin + modeWidth) && (msg.y >= lineMargin && msg.y <= lineMargin + modeHeight))
			{
				this->SetGameMode(bronze_level);   // 4x4
				IsChoice = 1;
			}
			if ((msg.x >= columnMargin + modeWidth + columnSpace && msg.x <= columnMargin + modeWidth + columnSpace + modeWidth) && (msg.y >= lineMargin && msg.y <= lineMargin + modeHeight))
			{
				this->SetGameMode(silver_level);   // 5x5
				IsChoice = 1;
			}
			if ((msg.x >= columnMargin && msg.x <= columnMargin + modeWidth) && (msg.y >= lineMargin + modeHeight + lineSpace && msg.y <= lineMargin + modeHeight + lineSpace + modeWidth))
			{
				this->SetGameMode(gold_level);	   // 6x6
				IsChoice = 1;
			}
			if ((msg.x >= columnMargin + modeWidth + columnSpace && msg.x <= columnMargin + modeWidth + columnSpace + modeWidth) && (msg.y >= lineMargin + modeHeight + lineSpace && msg.y <= lineMargin + modeHeight + lineSpace + modeWidth))
			{
				this->SetGameMode(platinum_level); // 7x7
				IsChoice = 1;
			}
			if (IsChoice)
			{
				break;
			}
		}
	}
	FlashWindow(hwnd, true);
	PlaySound(NULL, NULL, SND_PURGE);  // 用于结束上一次游戏的配音，若是第一次游戏，则无作用，因为下面会有配音
									   // (只有第一次或者判断游戏结束了后才会运行 CreateGame() 这个函数)
	PlaySound("ContraStarted", GetModuleHandle(NULL), SND_RESOURCE | SND_NOSTOP);

	FlashWindow(hwnd, false);
	PlaySound(NULL, NULL, SND_PURGE);
	Time = time(NULL);  // 获得游戏开始时间
	
	PlaySound(_T("impromptu"), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP | SND_FILENAME | SND_ASYNC);
	//mciSendString("open 背景1.wav alias BackgroundMusic", 0, 0, hwnd);
	//mciSendString("play BackgroundMusic", 0, 0, hwnd);
	CreateGameValue();
	CreateGameValue();


	setbkcolor(DARKGRAY);
	cleardevice();

	/*
	创建两个矩形 用于存放 时间与分数 （不好看）
	setfillcolor(LIGHTCYAN);
	cleardevice();
	solidrectangle(125, 10, 215, 50);

	solidrectangle(445, 10, 535, 50);
	*/
	for (int i = 0; i<sizeof(this->ImageName[0]) / sizeof(this->ImageName[0][0]); ++i)
	{
		loadimage(&this->img[this->m_mode - gameLevel_num][i], "IMAGE",this->ImageName[this->m_mode - gameLevel_num][i], (WIDTH - GAP * (this->m_mode + 1)) / this->m_mode, (HEIGHT - SPACE - GAP * (this->m_mode + 1)) / this->m_mode);
	}
	/*
	for (int i = 0; i < IMAGENUM; ++i)
	{
	this->img[i]->Resize((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode, (HEIGHT - SPACE - GAP * (this->m_mode + 1)) / this->m_mode);
	}
	4x4: 宽 150  高 150
	4x4: 宽 118  高 118
	4x4: 宽 96   高 96
	4x4: 宽 81   高 81

	这样能是可以把每张图片的宽、高 设置与游戏模式对等的图片尺寸，但是由于减小了尺寸后把图片的内容给剪了，再就是，
	从7x7模式切换到4x4模式时，由于图片已经被剪成了7x7模式的尺寸，(从81切换到150)尺寸不够只能自动填充默认黑色。。

	因此，解决办法：把每一种模式的图片都制作一份，因此 this->img[gameLevel_num][IMAGENUM] 为一个二维数组，有4个游戏模式
	每个模式有十二张图片，因此对应的 ImageName[gameLevel_num][IMAGENUM] 也是一个二维数组，一共有四种游戏模式，
	每一种游戏模式有十二个图片名称

	*/



	/*  用于从控制台输入，来创建对应的游戏模式
	cout << "please input play mode(a:4*4\tb:5*5\tc:6*6)" << endl;
	char ch;
	while (cin >> ch)
	{
	if (strchr("abc", ch))
	{
	switch (ch)
	{
	case 'a': this->SetGameMode(bronze_level); break;
	case'b': this->SetGameMode(silver_level); break;
	case'c': this->SetGameMode(gold_level); break;
	}
	break;
	}
	else
	{
	cout << "input error,please re-enter:" << endl;
	continue;
	}
	}
	*/
}

void cGame::CreateGameValue()
{
	//vector<int> mapinfo;
	//int num = 0;
	//for (int i = 0; i < this->m_mode; ++i)
	//{
	//	for (int j = 0; j < this->m_mode; ++j)
	//	{
	//		if (this->m_map[i][j] == 0)
	//		{
	//			mapinfo.push_back(i * this->m_mode + j);  // 保存每个为0格子的下标
	//			// 二维数组的元素下标转换为一维数组 的下标
	//		}
	//	}
	//}
	//int randvalue = rand() % mapinfo.size();
	//// 把下标转化为 二维数组中对应 的行列
	//int row = mapinfo.at(randvalue) / this->m_mode;
	//int col = mapinfo[randvalue] % this->m_mode;
	//randvalue = rand();
	//if (randvalue % 2 == 0)
	//{
	//	this->m_map[row][col] = 2;
	//}
	//else this->m_map[row][col] = 4;
	int flag = 0;
	while (1)
	{
		for (int i = 0; i < this->m_mode; ++i)
		{
			for (int j = 0; j < this->m_mode; ++j)
			{
				if (this->m_map[i][j] == 0)
				{  // 遍历所有元素，如果其中有一个为0，则 flag 等于1
					flag = 1;
					break;
				}
			}
			if (flag == 1)
			{
				break;
			}
		}
		if (flag == 0)   // 如果 flag 等于 0  则证明数组中没有为0的元素，也就是没有空格子，所以不需要创建值
		{
			return;
		}
		int randRow = rand() % this->m_mode;
		int randCol = rand() % this->m_mode;
		if (this->m_map[randRow][randCol] == 0)
		{
			if (rand() % 2 == 0)
			{
				this->m_map[randRow][randCol] = 2;
			}
			else this->m_map[randRow][randCol] = 4;
			break;
		}
		else continue;
	}
}
void cGame::ShowMap()
{
	for (int i = 0; i < this->m_mode; ++i)
	{
		for (int j = 0; j < this->m_mode; ++j)
		{
			switch (this->m_map[i][j])
			{ // void putimage(int dstx, int dsty, image *psrcimg, dword dwrop = srccopy);          
			  // 绘制位置的 x 坐标 、 绘制位置的 y 坐标 、 要绘制的 image 对象指针 、 三元光栅操作码（详见备注）
			  // 所以第一个参数对应j  第二个参数对应i
			case value2:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][1]);
				break;
				
			case value4:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][2]);
				break;
			case value8:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][3]);
				break;
			case value16:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][4]);
				break;
			case value32:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][5]);
				break;
			case value64:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][6]);
				break;
			case value128:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][7]);
				break;
			case value256:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][8]);
				break;
			case value512:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][9]);
				break;
			case value1024:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][10]);
				break;
			case value2048:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][11]);
				break;
			default:
				putimage((WIDTH - GAP * (this->m_mode + 1)) / this->m_mode * j + GAP * (j + 1), (HEIGHT - GAP * (this->m_mode + 1) - SPACE) / this->m_mode * i + GAP * (i + 1) + SPACE, &this->img[this->m_mode - gameLevel_num][0]);
				break;
			}
		}
	}
}

void cGame::ShowTime()
{
	this->m_time.tm_sec = time(NULL) - Time;

	settextstyle(40, 15, "微软雅黑", 0, 0, FW_NORMAL, 0, 0, 0);
	char strTime[9] = { 0 };
	char strScore[10] = { 0 };

	_itoa_s(this->m_score, strScore, 10);
	outtextxy(115, 10, "时间:");
	sprintf_s(strTime, "%02d:%02d:%02d", this->m_time.tm_sec / 60 / 60, this->m_time.tm_sec / 60, this->m_time.tm_sec % 60);
	outtextxy(190, 12, strTime);
	/*++this->m_time.tm_sec;

	if (this->m_time.tm_sec >= 60)
	{
		++this->m_time.tm_min;
		this->m_time.tm_sec = 0;
	}
	if (this->m_time.tm_min >= 60)
	{
		++this->m_time.tm_hour;
		this->m_time.tm_min = 0;
	}*/

	outtextxy(435, 10, "分数:");
	outtextxy(510, 12, strScore);	// 因为 outtextxy() 第三个参数接受的是 unicode 字符格式，而_itoa_s() 第二个参数接受的是 char*
								// 因此使用的多字节字符编码，让它们都能使用 char*   (可使用 把 char* 转换成 LPCTSTR 的函数实现，常量只需在前面加 L，变量不知)
}

void cGame::GetDir()
{
	while (1)
	{
			ShowTime();
		
		int bUp = GetAsyncKeyState(VK_UP);   // 按下 上键时，bup 等于非零   否则等于零
		if (bUp)
		{
			this->m_dir = dir_Up;
			return;
		}
		int bDown = GetAsyncKeyState(VK_DOWN);   // 按下 上键时，bup 等于非零   否则等于零
		if (bDown)
		{
			this->m_dir = dir_Down;
			return;
		}
		int bLeft = GetAsyncKeyState(VK_LEFT);   // 按下 上键时，bup 等于非零   否则等于零
		if (bLeft)
		{
			this->m_dir = dir_Left;
			return;
		}
		int bRight = GetAsyncKeyState(VK_RIGHT);   // 按下 上键时，bup 等于非零   否则等于零
		if (bRight)
		{
			this->m_dir = dir_Right;
			return;
		}
		
		//int bSpace = GetAsyncKeyState(VK_SPACE);   // 按下 上键时，bup 等于非零   否则等于零
		//if (bSpace)
		//{
		//		setfillcolor(RED);
		//		solidroundrect(0, HEIGHT / 2 - 15, WIDTH, HEIGHT / 2 + 15, 3, 3);
		//		settextstyle(20, 15, "黑体", 0, 0, FW_EXTRALIGHT, 0, 0, 0);
		//		setbkmode(TRANSPARENT);
		//		outtextxy(190, HEIGHT / 2 - 15 + 5, "按空格键继续！！！");
		//		while (1)
		//		{
		//			bSpace = GetAsyncKeyState(VK_SPACE);   // 按下 上键时，bup 等于非零   否则等于零
		//			if (bSpace)
		//			{
		//				InvalidateRect(this->hwnd, NULL, TRUE);
		//				Time += time(NULL) - Time;
		//				Time -= this->m_time.tm_sec - this->m_time.tm_min * 60 - this->m_time.tm_hour * 60 * 60;
		//				setbkmode(OPAQUE);
		//				break;
		//			}
		//			Sleep(200);
		//		}			
		//}
		int bEsc = GetAsyncKeyState(VK_ESCAPE);   // 按下 上键时，bup 等于非零   否则等于零
		if (bEsc)
		{
			this->m_dir = dir_Esc;
			return;
		}
	}
}


void cGame::Move()
{
	int temp;
	switch (this->m_dir)
	{

	case dir_Up:
	{
		// 先加  再移动
		for (int i = 0; i < this->m_mode - 1; ++i)
		{
			for (int j = 0; j < this->m_mode; ++j)
			{// 从头开始遍历，如果当前元素值等于0 则不需要操作，直接跳至下一个元素值
				if (this->m_map[i][j] != 0)
				{
					for (int k = i + 1; k < this->m_mode; ++k)
					{	// 如果当前元素值不等于0 则从下一行的这一列开始遍历，如果下一行的这一列等于0，继续跳至下一行
						if (this->m_map[k][j] == 0)
						{
							continue;
						}
						if (this->m_map[k][j] != this->m_map[i][j] && this->m_map[k][j] != 0)
						{ // 如果当前的这一行 不等于 下一行  并且下一行不等于0   就结束此次遍历，跳至下一个元素值
							break;
						}
						if (this->m_map[i][j] == this->m_map[k][j])
						{ // 如果当前这一行 等于 下一行(同为0的情况已被排除)，就把下一行的值加到当前这一行，并把下一行赋为0
						  //初始化背景音乐
						  //playsound(l"dub.wav", null, snd_nostop | snd_filename); //播放声音 wav
						  /*
						  bool playsound(lpctstr pszsound, hmodule hmod, dword fdwsound);

						  snd_async		用异步方式播放声音，playsound函数在开始播放后立即返回。（播放一次就返回）
						  snd_filename	pszsound参数指定了wave文件名。
						  snd_loop		重复播放声音，必须与snd_async标志一块使用。
						  snd_nostop		playsound不打断原来的声音播出并立即返回false。 (如果已经有声音，就不播放)
						  snd_sync		同步播放声音，在播放完后playsound函数才返回。(播放后 停顿才返回)
						  snd_memory		播放载入到内存中的声音，此时pszsound是指向声音数据的指针
						  */
							this->m_map[i][j] += this->m_map[k][j];
							this->m_map[k][j] = 0;
							this->m_score += 10;
							for (int m = k; m < this->m_mode - 1; ++m)
							{  // 再以下一行的行标为参照，继续遍历，把刚设为0的那个元素通过互换的方式，移至最后一行
								temp = this->m_map[m][j];
								this->m_map[m][j] = this->m_map[m + 1][j];
								this->m_map[m + 1][j] = temp;
							}
							break;  // 跳出，继续遍历下一个元素值
						}
					}
				}
			}
		}
		for (int i = 0; i < this->m_mode - 1; ++i)
		{
			for (int j = 0; j < this->m_mode; ++j)
			{
				if (this->m_map[i][j] == 0)
				{/* 从头开始遍历，如果当前这一行的元素值等于0，就继续找下一行(小于等于最后一行)，如果下一行不等于0，就把下一行的值
				 赋给这一行，并把下一行的值赋为0 */
					for (int m = i + 1; m < this->m_mode; ++m)
					{
						if (this->m_map[m][j] != 0)
						{
							this->m_map[i][j] = this->m_map[m][j];
							this->m_map[m][j] = 0;
							break;   // 这里还可以再优化
						}
					}
				}
			}
		}
		CreateGameValue();
		break;
	}
	case dir_Down:
	{// 先加  再移动
		for (int i = this->m_mode - 1; i > 0; --i)
		{
			for (int j = this->m_mode - 1; j >= 0; --j)
			{
				if (this->m_map[i][j] != 0)
				{
					for (int k = i - 1; k >= 0; --k)
					{	// 如果当前元素值不等于0 则从下一行的这一列开始遍历，如果下一行的这一列等于0，继续跳至下一行
						if (this->m_map[k][j] == 0)
						{
							continue;
						}
						if (this->m_map[k][j] != this->m_map[i][j] && this->m_map[k][j] != 0)
						{ // 如果当前的这一行 不等于 下一行  并且下一行不等于0   就结束此次遍历，跳至下一个元素值
							break;
						}
						if (this->m_map[i][j] == this->m_map[k][j])
						{ // 如果当前这一行 等于 下一行(同为0的情况已被排除)，就把下一行的值加到当前这一行，并把下一行赋为0
						  //		playsound(l"dub.wav", null, snd_async); //播放声音 wav
							this->m_map[i][j] += this->m_map[k][j];
							this->m_map[k][j] = 0;
							this->m_score += 10;
							for (int m = k; m > 0; --m)
							{  // 再以下一行的行标为参照，继续遍历，把刚设为0的那个元素通过互换的方式，移至最后一行
								temp = this->m_map[m][j];
								this->m_map[m][j] = this->m_map[m - 1][j];
								this->m_map[m - 1][j] = temp;
							}
							break;  // 跳出，继续遍历下一个元素值
						}
					}
				}
			}
		}
		for (int i = 0; i < this->m_mode - 1; ++i)
		{
			for (int j = 0; j < this->m_mode; ++j)
			{
				if (this->m_map[i][j] != 0)
				{/* 从头开始遍历，如果当前这一行的元素值不等于0，就继续找下一行(小于等于最后一行)，如果下一行等于0，就把这一行的值
				 赋给下一行，并把这一行的值赋为0 */
					for (int m = i + 1; m < this->m_mode; ++m)
					{
						if (this->m_map[m][j] == 0)
						{
							this->m_map[m][j] = this->m_map[i][j];
							this->m_map[i][j] = 0;
							break;
						}
					}
				}
			}
		}
		CreateGameValue();
		break;
	}
	case dir_Left:
	{// 先加  再移动
		for (int i = 0; i < this->m_mode; ++i)
		{
			for (int j = 0; j < this->m_mode - 1; ++j)
			{// 从头开始遍历，如果当前元素值等于0 则不需要操作，直接跳至下一个元素值
				if (this->m_map[i][j] != 0)
				{
					for (int k = j + 1; k < this->m_mode; ++k)
					{	// 如果当前元素值不等于0 则从下一行的这一列开始遍历，如果下一行的这一列等于0，继续跳至下一行
						if (this->m_map[i][k] == 0)
						{
							continue;
						}
						if (this->m_map[i][k] != this->m_map[i][j] && this->m_map[i][k] != 0)
						{ // 如果当前的这一行 不等于 下一行  并且下一行不等于0   就结束此次遍历，跳至下一个元素值
							break;
						}
						if (this->m_map[i][j] == this->m_map[i][k])
						{ // 如果当前这一行 等于 下一行(同为0的情况已被排除)，就把下一行的值加到当前这一行，并把下一行赋为0
						  //		playsound(l"dub.wav", null, snd_async); //播放声音 wav
							this->m_map[i][j] += this->m_map[i][k];
							this->m_map[i][k] = 0;
							this->m_score += 10;
							for (int m = k; m < this->m_mode - 1; ++m)
							{  // 再以下一行的行标为参照，继续遍历，把刚设为0的那个元素通过互换的方式，移至最后一行
								temp = this->m_map[i][m];
								this->m_map[i][m] = this->m_map[i][m + 1];
								this->m_map[i][m + 1] = temp;
							}
							break;  // 跳出，继续遍历下一个元素值
						}
					}
				}
			}
		}
		for (int i = 0; i < this->m_mode; ++i)
		{
			for (int j = 0; j < this->m_mode - 1; ++j)
			{
				if (this->m_map[i][j] == 0)
				{/* 从头开始遍历，如果当前这一行这一列的元素值等于0，就继续找下一列(小于等于最后一列)，如果下一列不等于0，就把下一列的值
				 赋给这一列，并把下一列的值赋为0 */
					for (int m = j + 1; m < this->m_mode; ++m)
					{
						if (this->m_map[i][m] != 0)
						{
							this->m_map[i][j] = this->m_map[i][m];
							this->m_map[i][m] = 0;
							break;
						}
					}
				}
			}
		}
		CreateGameValue();
		break;
	}
	case dir_Right:
	{
		for (int i = this->m_mode - 1; i >= 0; --i)
		{
			for (int j = this->m_mode - 1; j > 0; --j)
			{
				if (this->m_map[i][j] != 0)
				{
					for (int k = j - 1; k >= 0; --k)
					{	// 如果当前元素值不等于0 则从下一行的这一列开始遍历，如果下一行的这一列等于0，继续跳至下一行
						if (this->m_map[i][k] == 0)
						{
							continue;
						}
						if (this->m_map[i][k] != this->m_map[i][j] && this->m_map[i][k] != 0)
						{ // 如果当前的这一行 不等于 下一行  并且下一行不等于0   就结束此次遍历，跳至下一个元素值
							break;
						}
						if (this->m_map[i][j] == this->m_map[i][k])
						{ // 如果当前这一行 等于 下一行(同为0的情况已被排除)，就把下一行的值加到当前这一行，并把下一行赋为0
							this->m_map[i][j] += this->m_map[i][k];
							this->m_map[i][k] = 0;
							this->m_score += 10;
							//			playsound(l"dub.wav", null, snd_async); //播放声音 wav
							for (int m = k; m > 0; --m)
							{  // 再以下一行的行标为参照，继续遍历，把刚设为0的那个元素通过互换的方式，移至最后一行
								temp = this->m_map[i][m];
								this->m_map[i][m] = this->m_map[i][m - 1];
								this->m_map[i][m - 1] = temp;
							}
							break;  // 跳出，继续遍历下一个元素值
						}
					}
				}
			}
		}
		for (int i = 0; i < this->m_mode; ++i)
		{
			for (int j = 0; j < this->m_mode - 1; ++j)
			{
				if (this->m_map[i][j] != 0)
				{/* 从头开始遍历，如果当前这一行这一列的元素值等于0，就继续找下一列(小于等于最后一列)，如果下一列不等于0，就把下一列的值
				 赋给这一列，并把下一列的值赋为0 */
					for (int m = j + 1; m < this->m_mode; ++m)
					{
						if (this->m_map[i][m] == 0)
						{
							this->m_map[i][m] = this->m_map[i][j];
							this->m_map[i][j] = 0;
							break;
						}
					}
				}
			}
		}
		CreateGameValue();
		break;
	}
	case dir_Esc:
		exit(EXIT_FAILURE);
	}
}

bool cGame::JudgeOver()
{
	for (int i = 0; i < this->m_mode; ++i)
	{
		for (int j = 0; j < this->m_mode; ++j)
		{
			if (this->m_map[i][j] == MARK)
			{
				bWin = true;
				return true;
			}
		}
	}
	for (int i = 0; i < this->m_mode; ++i)
	{
		for (int j = 0; j < this->m_mode; ++j)
		{
			if (this->m_map[i][j] == 0)  // 只要还有零  游戏就没有结束
			{
				return false;
			}
		}
	}
	for (int i = 0; i < this->m_mode; ++i)
	{
		for (int j = 0; j < this->m_mode - 1; ++j)	// 最后一列不需要判断
		{
			if (this->m_map[i][j] == this->m_map[i][j + 1])	// 只要有一行中两个邻近的值相等，游戏就没有结束
			{
				return false;
			}
		}
	}
	for (int i = 0; i < this->m_mode - 1; ++i)   // 最后一行不需要判断
	{
		for (int j = 0; j < this->m_mode; ++j)
		{
			if (this->m_map[i][j] == this->m_map[i + 1][j])   // 只要有一列中两个邻近的值相等，游戏就没有结束
			{
				return false;
			}
		}
	}
	return true;
}

void cGame::Reset()
{
	closegraph();
	this->bWin = false;
	memset(&this->m_time, 0, sizeof(tm));

	this->m_score = 0;
	//memset(this->m_map[0], 0, this->m_mode*this->m_mode * sizeof(int));  报错
	for (int i = 0; i < this->m_mode; ++i)
	{
		for (int j = 0; j < this->m_mode; ++j)
		{
			this->m_map[i][j] = 0;
		}
	}

	//	memset(game.img, 0, sizeof(IMAGE)*IMAGENUM);  没有用
}
