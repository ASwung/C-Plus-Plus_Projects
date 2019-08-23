/***************************************************
文件名称: 2048 project
文件描述: 实现一个2048的游戏
编译环境: vs2017  vs2008-vs2017
最后修改:
<2017年12月20日>  <创建工程>  <aswung>


PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y))
****************************************************/

#include "stdafx.h"
#include "Game.h"
int main()
{
	static cGame game;	 // 静态的游戏类，用来存储游戏的所有信息（地图、图片、地图上每个元素的值、用户输入的方向、用户选择的游戏模式）
	srand(unsigned(time(NULL)));
	game.CreateGame();

	while (1)
	{
		game.ShowTime();
		game.ShowMap();
		Sleep(500);

		game.GetDir();
		game.Move();
		
		if (game.JudgeOver())    // 遍历数组 判断游戏是否结束
		{
			game.ShowMap();
			if (game.bWin == true)
			{
				PlaySound("Victory", GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
				MessageBox(game.hwnd, "哇！尊敬的大神，您赢啦！！！", "跪伏", MB_OK);
				closegraph();
				break;
			}
			else
			{
				PlaySound("LandlordsLose", GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
				// playsound(l"失败.wav", null, snd_async);
				if ((MessageBox(game.hwnd, "哼，弱小的人类，你输了。。。", "鄙视", MB_RETRYCANCEL) == IDRETRY))
				{
					game.Reset();
					game.CreateGame();
				}
				else
				{
					PlaySound(NULL, NULL, SND_PURGE);
					closegraph();
					break;
				}
			}
		}
	}
	system("pause");
	return 0;
}










