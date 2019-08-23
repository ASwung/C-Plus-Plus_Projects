#ifndef  _GAME_H_
#define _GAME_H_
#include "stdafx.h"

#define IMAGENUM 12
#define WIDTH 650
#define HEIGHT 700
#define IMAGESIZE 150
#define SPACE 50
#define GAP 10
#define MARK 2048
#define CHOICEMODEWIDTH 200
#define CHOICEMODEHEIGHT 60
using namespace std;


enum Game_mode
{
	bronze_level = 4,
	silver_level,
	gold_level,
	platinum_level,
	gameLevel_num = 4,
};

enum Game_dir
{
	dir_Default = -1,
	dir_Up,
	dir_Down,
	dir_Left,
	dir_Right,
	dir_Space,
	dir_Esc,
};

enum Game_value
{
	value2 = 2,
	value4 = 4,
	value8 = 8,
	value16 = 16,
	value32 = 32,
	value64 = 64,
	value128 = 128,
	value256 = 256,
	value512 = 512,
	value1024 = 1024,
	value2048 = 2048,
};

class cGame
{
public:
	cGame() {}
	~cGame();

	void SetGameMode(Game_mode mode); 
	void CreateGame();
	void CreateGameValue();
	void ShowMap();
	void ShowTime();
	void GetDir();
	void Move();
	bool JudgeOver();
	void Reset();
	Game_mode m_mode;
	Game_dir m_dir;
	//vector<vector<int>> m_map;
	int **m_map;
	static int m_score;
	tm m_time;
	IMAGE img[gameLevel_num][IMAGENUM];
	LPCTSTR ImageName[gameLevel_num][IMAGENUM] =
	{
		{ "4x4_0", "4x4_1", "4x4_2", "4x4_3", "4x4_4", "4x4_5", "4x4_6", "4x4_7", "4x4_8", "4x4_9", "4x4_10", "4x4_11" },
		{ "5x5_0", "5x5_1", "5x5_2", "5x5_3", "5x5_4", "5x5_5", "5x5_6", "5x5_7", "5x5_8", "5x5_9", "5x5_10", "5x5_11" },
		{ "6x6_0", "6x6_1", "6x6_2", "6x6_3", "6x6_4", "6x6_5", "6x6_6", "6x6_7", "6x6_8", "6x6_9", "6x6_10", "6x6_11" },
		{ "7x7_0", "7x7_1", "7x7_2", "7x7_3", "7x7_4", "7x7_5", "7x7_6", "7x7_7", "7x7_8", "7x7_9", "7x7_10", "7x7_11" },
	};
	HWND hwnd;
	bool bWin = false;   // 判断输赢，默认为输
};

#endif // ! _GAME_H_
