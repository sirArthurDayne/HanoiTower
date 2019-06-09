#pragma once
#include"olcConsoleGameEngineOOP.h"

enum STATES { START, RECURSIVEGAME, STACKGAME, STACKAUTO, SCORES, CREDITS, EXIT };

class HanoiGame : public olcConsoleGameEngineOOP {
public:
	//set machine
	STATES state = START;
	std::vector<int> towers[3];
	int diskAmount;

	//interation
	int mode;//0: start 1: activa recursividad 2:stack 3: stackAuto 4:creditos 5:scoreboard 6:salir
	int mouseHolder;//size of disk pick by the mouse


	//scores related 
	std::vector<std::wstring> players;
	bool gettingName;
	std::vector<int> pointsplayers;
	int movesTotal;
	const int MAXPOINTS = 10000;
	int points;

public:
	HanoiGame();

	bool OnUserCreate() override;


	bool OnUserUpdate(float fElapsedTime) override;


	bool GameStartMenu();
	

	void SetUpRecursiveHanoi(int disks);

	void DrawRecursiveHanoiGame(float deltaTime);
	

	void SetUpStackHanoi(int disks);

	void CheckMouseInput();
	

	void DrawStackHanoiGame();

	void SetUpStackAutoHanoiGame(int disks);

	void DrawStackAuto(float deltaTime);
	

	void DrawTowers(short background, short disksColor);
	
	void DrawScores();
	

	void ClearVectors();
	

	void DrawCredits();
	

	void GetUsersName(float deltaTime);
	

	void PointsSystem(int moves);
	

	void SortLists();
	
};

