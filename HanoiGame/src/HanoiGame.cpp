#include"HanoiGame.h"

struct move {
	move(int _from, int _to) {
		from = _from;
		to = _to;
	}
	int from, to;
};
std::vector<move> moveset;

void HanoiRecursion(int nDisks, int first, int end, int middle)
{

	if (nDisks > 0)
	{
		HanoiRecursion(nDisks - 1, first, middle, end);

		moveset.push_back(move(first, end));

		HanoiRecursion(nDisks - 1, middle, end, first);
	}
}

mainEngine::mainEngine()
{
	m_sAppName = L"LOS JUEGOS DE TORRE DE HANOI / HANOI TOWER";
}

bool mainEngine::OnUserCreate()
{
	mode = 0;
	mouseHolder = 0;
	gettingName = false;
	movesTotal = 0;
	points = MAXPOINTS;
	return true;
}

bool mainEngine::OnUserUpdate(float fElapsedTime)
{
	if (state == START)
	{
		if (GameStartMenu())
		{
			if (mode == 1)
				state = RECURSIVEGAME;
			else if (mode == 2)
				state = STACKGAME;
			else if (mode == 3)
				state = STACKAUTO;
			else if (mode == 4)
				state = CREDITS;
			else if (mode == 5)
				state = SCORES;
			else if (mode == 6)
				state = EXIT;
		}
	}
	else if (state == RECURSIVEGAME)
	{
		DrawRecursiveHanoiGame(fElapsedTime);
	}
	else if (state == STACKGAME)
	{
		DrawStackHanoiGame();
	}
	else if (state == STACKAUTO)
	{
		DrawStackAuto(fElapsedTime);
	}
	else if (state == SCORES)
	{
		DrawScores();
		if (gettingName)//TODO 
		{
			GetUsersName(fElapsedTime);
		}
		else
		{
			int i = 1;
			for (auto name : players)
			{
				DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2 + (i * 5), name, FG_DARK_YELLOW);
				i++;
			}
			int j = 1;
			for (auto p : pointsplayers)
			{
				std::wstring point = std::to_wstring(p);
				DrawString(ScreenWidth() / 2, ScreenHeight() / 2 + (j * 5), point, FG_DARK_YELLOW);
				j++;
			}
		}
	}
	else if (state == CREDITS)
	{
		DrawCredits();
	}
	else if (state == EXIT)
		return false;

	return true;
}

bool mainEngine::GameStartMenu()
{
	//fondo
	Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_DARK_CYAN);


	DrawString(ScreenWidth() / 2 - 20, 10, L"LOS JUEGOS DE HANOI / THE TOWER OF HANOI", FG_WHITE);

	DrawString(10, 25, L"SELECT DISK AMOUNT:", FG_WHITE);
	DrawString(10, 30, L"SELECCIONE LA CANTIDAD DE DISCOS:", FG_WHITE);

	if (diskAmount < 3 || diskAmount > 8)
		diskAmount = 3;

	if (m_keys[VK_UP].bReleased)
		diskAmount++;
	else if (m_keys[VK_DOWN].bReleased)
		diskAmount--;

	std::wstring diskNumber = std::to_wstring(diskAmount);
	DrawString(45, 25, diskNumber, FG_WHITE);

	DrawString(10, 30, L"PRESS 'R' FOR HANOI_RECURSIVO", FG_WHITE);
	DrawString(10, 40, L"PRESS 'S' FOR HANOI_STACK MANUAL", FG_WHITE);
	DrawString(10, 50, L"PRESS 'A' FOR HANOI_STACK AUTO", FG_WHITE);
	DrawString(10, 60, L"PRESS 'Q' FOR SCOREBOARD/TABLA", FG_WHITE);
	DrawString(10, 70, L"PRESS 'C' FOR CREDITS/CREDITOS", FG_WHITE);
	DrawString(10, 80, L"PRESS 'ESC' FOR EXIT/SALIR", FG_WHITE);

	if (m_keys['R'].bReleased)
	{
		SetUpRecursiveHanoi(diskAmount);
		mode = 1;
		return true;
	}

	if (m_keys['S'].bReleased)
	{
		SetUpStackHanoi(diskAmount);
		mode = 2;
		return true;
	}

	if (m_keys['A'].bReleased)
	{
		SetUpStackAutoHanoiGame(diskAmount);
		mode = 3;
		return true;
	}

	if (m_keys['C'].bReleased)
	{
		mode = 4;
		return true;
	}

	if (m_keys['Q'].bReleased)
	{
		mode = 5;
		return true;
	}
	if (m_keys[VK_ESCAPE].bReleased)
	{
		mode = 6;
		return true;
	}

	return false;
}

void mainEngine::SetUpRecursiveHanoi(int disks)
{
	int towerA = 0;
	int towerB = 1;
	int towerC = 2;

	HanoiRecursion(disks, towerA, towerC, towerB);


	for (int i = 0; i < disks; i++)
		towers[0].push_back(disks - i);
}

void mainEngine::DrawRecursiveHanoiGame(float deltaTime)
{
	static float timer = 0;
	static int nmove = 0;
	timer += deltaTime;

	if (timer > 0.5)
	{
		timer -= 0.5;
		if (nmove < moveset.size())
		{
			towers[moveset[nmove].to].push_back(towers[moveset[nmove].from].back());
			towers[moveset[nmove].from].pop_back();
			towers[moveset[nmove].from].shrink_to_fit();
			towers[moveset[nmove].to].shrink_to_fit();


			nmove++;
		}
	}



	//Graphics	
	DrawTowers(FG_BLACK, BG_DARK_MAGENTA);


	//menu
	if (m_keys['M'].bReleased)
	{
		ClearVectors();
		state = START;
	}

	//end app
	else if (m_keys[VK_ESCAPE].bReleased)
	{
		ClearVectors();
		state = EXIT;
	}

	else if (m_keys['C'].bReleased)
	{
		ClearVectors();
		state = CREDITS;
	}
}

void mainEngine::SetUpStackHanoi(int disks)
{
	//insert disks on tower 0
	for (int i = 0; i < disks; i++)
		towers[0].push_back(disks - i);
}

void mainEngine::CheckMouseInput()
{
	if (GetMouse(0).bReleased)
	{
		int mouseX = GetMouseX();
		int mouseY = GetMouseY();

		if (mouseHolder != 0)
		{
			for (int i = 0; i < 3; i++)
			{
				if ((mouseX < i * 40 + 20) && (mouseX > i * 40 - 20) && (mouseY < 90 && mouseY >  65))//this means we have hit tower[i], or a box around tower[i] at least
				{
					if (towers[i].empty() || towers[i].back() > mouseHolder)
					{
						towers[i].push_back(mouseHolder);

						mouseHolder = 0;

						//agregamos un movimiento y verificamos el puntaje
						movesTotal++;
						PointsSystem(movesTotal);
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if ((mouseX < i * 40 + 20) && (mouseX > i * 40 - 20) && (mouseY < 90 && mouseY >  65))
				{
					if (towers[i].empty() == false)
					{
						mouseHolder = towers[i].back();
						towers[i].pop_back();
					}
				}
			}
		}
	}



}

void mainEngine::DrawStackHanoiGame()
{
	//controles
	CheckMouseInput();

	//graficas
	DrawTowers(BG_BLACK, BG_DARK_YELLOW);

	//volver a menu
	if (m_keys['M'].bReleased)
	{
		ClearVectors();
		points = MAXPOINTS;
		movesTotal = 0;
		state = START;
	}

	//matar app
	else if (m_keys[VK_ESCAPE].bReleased)
	{
		ClearVectors();
		state = EXIT;
	}

	//TU GANAS
	if (towers[2].size() == diskAmount)
	{
		state = SCORES;
	}

}

void mainEngine::SetUpStackAutoHanoiGame(int disks)
{
	HanoiRecursion(disks, 0, 2, 1);

	//carga la torre 0 con los discos
	for (int i = 0; i < disks; i++)
		towers[0].push_back(disks - i);
}

void mainEngine::DrawStackAuto(float deltaTime)
{
	static float timer = 0;
	static int nmove = 0;
	timer += deltaTime;

	if (timer > 0.6)
	{
		timer -= 0.6;
		if (nmove < moveset.size())
		{
			towers[moveset[nmove].to].push_back(towers[moveset[nmove].from].back());
			towers[moveset[nmove].from].pop_back();

			towers[moveset[nmove].from].shrink_to_fit();
			towers[moveset[nmove].to].shrink_to_fit();

			nmove++;
		}
	}

	//Graficas	
	DrawTowers(BG_BLACK, BG_DARK_GREY);


	//menu
	if (m_keys['M'].bReleased)
	{
		ClearVectors();
		state = START;
	}

	//final de app
	else if (m_keys[VK_ESCAPE].bReleased)
	{
		ClearVectors();
		state = EXIT;
	}

}

void mainEngine::DrawTowers(short background, short disksColor)
{
	Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', background);
	//torres texto
	DrawString(18 + (0 * 40), 90 - 30, L"TORRE/TOWER 0", FG_DARK_BLUE);
	DrawString(18 + (1 * 40), 90 - 30, L"TORRE/TOWER 1", FG_DARK_GREEN);
	DrawString(18 + (2 * 40), 90 - 30, L"TORRE/TOWER 2", FG_DARK_RED);

	//dibujar torres en pantalla
	for (int i = 0; i < 3; i++)
	{
		int xTowerCoord = 20 + (i * 40);
		int yTowerCoord = 90 - 25;
		short color;
		if (i == 0) color = BG_DARK_BLUE;
		if (i == 1) color = BG_DARK_GREEN;
		if (i == 2) color = BG_DARK_RED;

		Fill(xTowerCoord, yTowerCoord, (xTowerCoord - 2) + 3, yTowerCoord + 25, ' ', color);
	}


	for (int i = 0; i < 3; i++)
	{
		//si aun hay movimientos en las torres
		if (towers[i].size() > 0)
		{
			for (int j = 0; j < towers[i].size(); j++)
			{
				//dibujar los discos
				int _size = towers[i].at(j);//torre i disco j
				int xdiskCoord = 20 + i * 40 - (_size + 4);
				int ydiskCoord = 90 - (j * 3);
				Fill(xdiskCoord, ydiskCoord, xdiskCoord + (_size + 4) * 2 + 1, ydiskCoord + 3, ' ', disksColor);

				//dibujar titulos y detalles
				if (state == RECURSIVEGAME)
				{
					DrawString(ScreenWidth() / 2 - 20, 5, L"HANOI RECURSIVO", FG_DARK_GREEN);
					DrawString(20, 10, L"TOTAL OF MOVES: ", FG_WHITE);
					DrawString(20, 14, L"TOTAL DE MOVIMIENTOS: ", FG_WHITE);
					DrawString(45, 10, std::to_wstring(moveset.size()), FG_WHITE);
				}
				else if (state == STACKAUTO)
				{
					DrawString(ScreenWidth() / 2 - 20, 5, L"HANOI STACKAUTO", FG_DARK_BLUE);
					DrawString(20, 10, L"TOTAL OF MOVES: ", FG_WHITE);
					DrawString(20, 14, L"TOTAL DE MOVIMIENTOS: ", FG_WHITE);
					DrawString(45, 10, std::to_wstring(moveset.size()), FG_WHITE);
				}
				else if (state == STACKGAME)
				{
					DrawString(ScreenWidth() / 2 - 20, 5, L"HANOI JUGABLE/HANOI PLAYABLE", FG_DARK_YELLOW);
					DrawString(25, 10, L"MOVIMIENTOS:", FG_DARK_YELLOW);
					DrawString(25, 14, L"MOVES:", FG_DARK_YELLOW);
					DrawString(40, 10, std::to_wstring(movesTotal), FG_DARK_YELLOW);
					DrawString(25, 20, L"PUNTOS:", FG_DARK_YELLOW);
					DrawString(25, 24, L"POINTS:", FG_DARK_YELLOW);
					DrawString(35, 20, std::to_wstring(points), FG_DARK_YELLOW);
				}
			}
		}
	}


}

void mainEngine::DrawScores()
{
	Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_BLACK);

	DrawString(ScreenWidth() / 2 - 15, ScreenHeight() / 2 - 30, L"CONGRATS, YOU WIN!!!", FG_DARK_YELLOW);

	DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2 - 5, L"INSERT YOUR NAME Y DATE OF THE SYSTEM: PRESS 'N' ", FG_DARK_YELLOW);
	DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2, L"SCOREBOARD", FG_DARK_YELLOW);

	DrawString(ScreenWidth() / 2 + 20, ScreenHeight() / 2 + 30, L"MENU, PRESS 'M'", FG_DARK_YELLOW);
	DrawString(ScreenWidth() / 2 + 20, ScreenHeight() / 2 + 35, L"EXIT, PRESS 'ESC'", FG_DARK_YELLOW);

	//name input
	if (gettingName == false && m_keys['N'].bReleased)
	{
		gettingName = true;

	}

	//volver a menu
	if (gettingName == false && m_keys['M'].bReleased)
	{
		ClearVectors();
		points = MAXPOINTS;
		movesTotal = 0;
		state = START;
	}
	//end app
	else if (gettingName == false && m_keys[VK_ESCAPE].bReleased)
	{
		ClearVectors();
		state = EXIT;
	}
}

void mainEngine::ClearVectors()
{
	moveset.shrink_to_fit();
	moveset.clear();
	for (int i = 0; i < 3; i++)
	{
		towers[i].shrink_to_fit();
		towers[i].clear();
	}

}

void mainEngine::DrawCredits()
{
	Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_DARK_BLUE);
	DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2 - 10, L"DEVELOP BY:", FG_WHITE);
	DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2, L"XAVIER LAMELA(SIR_ARTHUR_DAYNE)", FG_WHITE);
	DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2 + 15, L"SPECIAL THANKS TO ZLEAPINGBEAR, WITHOUT YOU THIS PROYECT WONT BE COMPLETED", FG_WHITE);


	if (m_keys['M'].bReleased)
	{
		ClearVectors();
		state = START;
	}
	//end app
	else if (m_keys[VK_ESCAPE].bReleased)
	{
		ClearVectors();
		state = EXIT;
	}

}

void mainEngine::GetUsersName(float deltaTime)
{
	DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2 - 10, L"**INSERTANDO**...", FG_DARK_YELLOW);
	static std::wstring name = L"";
	static float keytimer = 0;

	keytimer += deltaTime;

	if (keytimer > 0.1)
	{
		for (int i = 0; i < 128; i++)
		{
			if (m_keys[i].bReleased)
			{
				name.push_back(i);
				keytimer = 0;
			}
		}

	}

	if (m_keys[VK_RETURN].bReleased)//out of input SUB-state
	{
		players.push_back(name);
		pointsplayers.push_back(points);

		//sort list
		if (players.size() > 2 && pointsplayers.size() > 2)
		{
			SortLists();
		}
		name = L"";
		gettingName = false;
	}

}

void mainEngine::PointsSystem(int moves)
{
	if (points > 0)
	{
		if (moves % 8 == 0 && diskAmount == 3)
		{
			points -= 1000;
		}
		if (moves % 15 == 0 & diskAmount == 4)
		{
			points -= 500;
		}
		if (moves % 20 == 0 && diskAmount == 5)
		{
			points -= 400;
		}
		if (moves % 22 == 0 && diskAmount == 6)
		{
			points -= 300;
		}
		if (moves % 26 == 0 && diskAmount == 7)
		{
			points -= 200;
		}
		if (moves % 30 == 0 && diskAmount == 8)
		{
			points -= 100;
		}
	}
	else points = 0;
}

void mainEngine::SortLists()
{
	for (int i = 0; i < pointsplayers.size() - 1; i++)
	{
		for (int j = i + 1; j < pointsplayers.size(); j++)
		{
			if (pointsplayers.at(j) > pointsplayers.at(i))
			{
				int pTemp = pointsplayers.at(i);
				std::wstring nTemp = players.at(i);

				pointsplayers.at(i) = pointsplayers.at(j);
				players.at(i) = players.at(j);

				pointsplayers.at(j) = pTemp;
				players.at(j) = nTemp;
			}
		}
	}
}