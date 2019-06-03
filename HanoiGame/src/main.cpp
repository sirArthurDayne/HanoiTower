
/*
	REGLAS:
	1. Solo un disco puede ser movido por vez.
	2. El disco removido solo debe ser colocado en una torre
	3. Un disco grande no puede ser colocado encima de uno pequeno
	
	RULES:
	1. Only one disk can move at the time.
	2. The Disk removed has to be place in a tower.
	3. A big Disk cannot be placed on top a smaller disk.
	  -             |            |
	-----			|			 |
  ---------			|			 |
	  |				|			 |
	  |				|			 |

	TORRE[A]      TORRE[B]     TORRE[C]
	    0            1           2
 */


#include<iostream>
#include<string>
#include"olcConsoleGameEngine.h"

enum STATES {START, RECURSIVEGAME, STACKGAME, STACKAUTO, SCORES, CREDITS, EXIT};

struct move {
	move(int _from, int _to) {
		from = _from;
		to = _to;
	}
	int from, to;
};

struct vecContainer 
{
	vecContainer() {}
	std::vector<move> _moves;

	void addMove(move old) {
		_moves.push_back(move(old.from, old.to));
	}

	std::vector<move> getMoves() { return _moves; }
	int getSize() { return _moves.size(); }
};

vecContainer moveContainer;

void HanoiRecursion(int nDisks, int first, int middle, int last)
{
	//nDisks work has iterator
	if (nDisks > 0)
	{
		HanoiRecursion(nDisks - 1, first, last, middle);
		moveContainer.addMove(move(first, middle));
		HanoiRecursion(nDisks - 1, last, middle, first);
	}
}

std::vector<move> moveset;


class mainEngine : public olcConsoleGameEngine {
public:
	STATES state = START;
	std::vector<int> towers[3];
	int diskAmount;
	
	std::vector<std::wstring> players;
	
	int mode;//0: start 1: activa recursividad 2:stack 3: stackAuto 4:creditos 5:scoreboard 6:salir
	int mouseHolder;//the disk size pickup by the mouse
	float timeMarker;
	
	mainEngine()
	{
		m_sAppName = L"LOS JUEGOS DE TORRE DE HANOI";
	}

	bool OnUserCreate()
	{
		mode = 0;
		timeMarker = 0.0f;
		mouseHolder = 0;
		return true;
	}


	bool OnUserUpdate(float fElapsedTime)
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
		}
		else if (state == CREDITS)
		{
			DrawCredits();
		}
		else if (state == EXIT)
			return false;

		return true;
	}


	bool GameStartMenu()
	{
		//background
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_DARK_BLUE);

		//title 
		DrawString(ScreenWidth() / 2 - 20, 10, L"LOS JUEGOS DE HANOI", FG_WHITE);

		DrawString(10, 25, L"SELECCIONE LA CANTIDAD DE DISCOS", FG_WHITE);

		//disks selection
		if (diskAmount < 3 || diskAmount > 8)
			diskAmount = 3;

		if (m_keys[VK_UP].bReleased)
			diskAmount++;
		else if (m_keys[VK_DOWN].bReleased)
			diskAmount--;

		std::wstring diskNumber = std::to_wstring(diskAmount);
		DrawString(45, 25, diskNumber, FG_WHITE);

		DrawString(10, 30, L"PRESIONE 'R' PARA HANOI_RECURSIVO", FG_WHITE);//press R for recursiveHanoi
		DrawString(10, 40, L"PRESIONE 'S' PARA HANOI_STACK MANUAL", FG_WHITE);//press S for hanoiStack
		DrawString(10, 50, L"PRESIONE 'A' PARA HANOI_STACK AUTO", FG_WHITE);
		DrawString(10, 60, L"PRESIONE 'Q' PARA SCOREBOARD", FG_WHITE);//exit mode
		DrawString(10, 70, L"PRESIONE 'C' PARA CREDITOS", FG_WHITE);//creditos
		DrawString(10, 80, L"PRESIONE 'ESC' PARA SALIR", FG_WHITE);//exit mode

		//letter for activating hanoiRecursive
		if (m_keys['R'].bReleased)
		{
			SetUpRecursiveHanoi(diskAmount);
			mode = 1;
			return true;
		}

		//letter for activating hanoiStack manual
		if (m_keys['S'].bReleased)
		{
			SetUpStackHanoi(diskAmount);
			mode = 2;
			return true;
		}

		//letter for activating hanoiStack auto 
		if (m_keys['A'].bReleased)
		{
			SetUpStackAutoHanoiGame(diskAmount);
			mode = 3;
			return true;
		}

		//letter for credits
		if (m_keys['C'].bReleased)
		{
			mode = 4;
			return true;
		}

		//letter for SCORES
		if (m_keys['Q'].bReleased)
		{
			mode = 5;
			return true;
		}
		//letter for Exit
		if (m_keys[VK_ESCAPE].bReleased)
		{
			mode = 6;
			return true;
		}

		return false;
	}

	void SetUpRecursiveHanoi(int disks)
	{
		int towerA = 0;
		int towerB = 1;
		int towerC = 2;
		HanoiRecursion(disks, towerA, towerC, towerB);

		//carga moveset con todos los mov. generados en el algoritmo
		for (int i = 0; i < moveContainer.getSize(); i++)
		{
			moveset.push_back(move(moveContainer._moves.at(i).from, moveContainer._moves.at(i).to));
		}
		//carga la torre 0 con los discos
		for (int i = 0; i < disks; i++)
			towers[0].push_back(disks - i);
	}



	void DrawRecursiveHanoiGame(float deltaTime)
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


	void SetUpStackHanoi(int disks)
	{
		//insert disks on tower 0
		for (int i = 0; i < disks; i++)
			towers[0].push_back(disks - i);
	}


	void CheckMouseInput()
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

							std::wstring mouseContent = std::to_wstring(mouseHolder);
							std::wstring towerContent = std::to_wstring(i);

							DrawString(20, 25, L"MOVER DISCO", FG_WHITE);
							DrawString(20, 30, L"A TORRE", FG_WHITE);

							mouseHolder = 0;
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

							std::wstring mouseContent = std::to_wstring(mouseHolder);
							std::wstring towerContent = std::to_wstring(i);

							DrawString(15, 15, mouseContent, FG_WHITE);
							DrawString(15, 20, towerContent, FG_WHITE);
						}
					}
				}
			}
		}



	}

	void DrawStackHanoiGame()
	{
		//controls
		CheckMouseInput();
		
		//graphics
		DrawTowers(BG_BLACK, BG_DARK_YELLOW);

		//volver a menu
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

		//win the game
		if (towers[2].size() == diskAmount)
		{
			state = SCORES;
		}

	}

	void SetUpStackAutoHanoiGame(int disks)
	{
		HanoiRecursion(disks, 0, 2, 1);

		//carga moveset con todos los mov. generados en el algoritmo
		for (int i = 0; i < moveContainer.getSize(); i++)
		{
			moveset.push_back(move(moveContainer._moves.at(i).from, moveContainer._moves.at(i).to));
		}
		//carga la torre 0 con los discos
		for (int i = 0; i < disks; i++)
			towers[0].push_back(disks - i);
	}

	void DrawStackAuto(float deltaTime)
	{
		static float timer = 0;
		static int nmove = 0;
		timer += deltaTime;

		if (timer > 1.0)
		{
			timer -= 1.0;
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
		DrawTowers(BG_DARK_MAGENTA, BG_DARK_GREY);


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

	}

	void DrawTowers(short background, short disksColor)
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', background);
		//torres texto
		DrawString(18 + (0 * 40), 90 - 30, L"TORRE 0", FG_DARK_BLUE);
		DrawString(18 + (1 * 40), 90 - 30, L"TORRE 1", FG_DARK_GREEN);
		DrawString(18 + (2 * 40), 90 - 30, L"TORRE 2", FG_DARK_RED);

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
				}
			}
		}
	
	
	}

	void DrawScores()
	{
		Fill(0,0, ScreenWidth(), ScreenHeight(), ' ', BG_BLACK);


		DrawString(ScreenWidth() /2 - 10, ScreenHeight()/2 + 5, L"FELICIDADES HAS GANADO!!!", FG_DARK_YELLOW);
		DrawString(ScreenWidth() /2 - 10, ScreenHeight()/2 + 10, L"INSERTE SU NOMBRE: ", FG_DARK_YELLOW);
		DrawString(ScreenWidth() /2 - 10, ScreenHeight()/2 + 15, L"MENU, PRESIONA 'M'", FG_DARK_YELLOW);
		DrawString(ScreenWidth() /2 - 10, ScreenHeight()/2 + 20, L"SALIDA, PRESIONA 'ESC'", FG_DARK_YELLOW);

		//volver a menu
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

	
	void ClearVectors()
	{
		moveset.clear();
		for (int i = 0; i < 3; i++)
		{
			towers[i].clear();
		}
	}

	void DrawCredits()
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_DARK_BLUE);
		DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2 - 10, L"DESARROLLADO POR:", FG_WHITE);
		DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2, L"XAVIER LAMELA(SIR_ARTHUR_DAYNE)", FG_WHITE);
		DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2 + 10, L"CLYDE 'LA VOZ' HARBIN", FG_WHITE);
		DrawString(ScreenWidth() / 2 - 30, ScreenHeight() / 2 + 20, L"LUIS 'EL DEMENTOR' CHAVEZ", FG_WHITE);
		DrawString(ScreenWidth() / 2 - 30, ScreenHeight()/2 + 30, L"SPECIAL THANKS TO ZLEAPINGBEAR, WITHOUT YOU THIS PROYECT WONT BE COMPLETED", FG_WHITE);

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

};



int main()
{	
		mainEngine game;
		const int w = 120;
		const int h = 100;

		const int pixelSize = 10;
		if (game.ConstructConsole(w, h, pixelSize, pixelSize))
			game.Start();
		else 
			std::cout << "ERROR DE CARGA\n";

		return 0;
	
}