
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


#include <iostream>
#include<string>
#include"olcConsoleGameEngine.h"

enum STATES {START, RECURSIVEGAME, STACKGAME, SCORES, EXIT};

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



class mainEngine : public olcConsoleGameEngine {
public:
	STATES state = START;
	std::vector<move> moveset;
	std::vector<int> towers[3];
	int diskAmount;
	std::wstring playerName;
	bool game1;//1: activate recursive, 0: activate stack

	mainEngine()
	{
		m_sAppName = L"JUEGO DE TORRE DE HANOI";
	}
	
	bool OnUserCreate()
	{
		game1 = true;
		return true;
	}


	bool OnUserUpdate(float fElapsedTime)
	{
		if (state == START)
		{
			if (GameStartMenu())
			{
				if (game1)
					state = RECURSIVEGAME;
				else
					state = STACKGAME;
			}
		}
		if (state == RECURSIVEGAME)
		{
			if (DrawRecursiveHanoiGame(fElapsedTime))
			{
				state = START;
			}
		}
		if (state == STACKGAME)
		{
			
		}
		 

		if (state == EXIT)
			return false;
		
		return true;
	}


	bool GameStartMenu()
	{
		//background
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_DARK_BLUE);
		
		//title 
		DrawString(ScreenWidth()/2 - 20, 10, L"LOS JUEGOS DE HANOI", FG_WHITE);

		DrawString(10, 25, L"SELECCIONE LA CANTIDAD DE DISCOS", FG_WHITE);
		
		//disks selection
		if (diskAmount < 3 || diskAmount > 8)
			diskAmount = 3;

		if (m_keys[VK_UP].bReleased)
			diskAmount++;
		else if (m_keys[VK_DOWN].bReleased)
			diskAmount--;

		std::wstring diskNumber= std::to_wstring(diskAmount);
		DrawString(45, 25, diskNumber, FG_WHITE);
		
		DrawString(10, 30, L"PRESIONE 'R' PARA ACTIVAR HANOIRECURSIVO", FG_WHITE);//press R for recursiveHanoi
		
		DrawString(10, 40, L"PRESIONE 'S' PARA ACTIVAR HANOISTACK", FG_WHITE);//press S for hanoiStack
		DrawString(10, 50, L"PRESIONE 'ESC' PARA SALIR", FG_WHITE);//exit mode
		
		//letter for activating hanoiRecursive
		if (m_keys['R'].bReleased)
		{
			SetUpRecursiveHanoi(diskAmount);
			return true;
		}

		//letter for activating hanoiStack
		if (m_keys['S'].bReleased)
		{
			SetUpStackHanoi(diskAmount);
			game1 = false;
			return true;
		}
		//letter for SCORES
		
		//letter for Exit
		if (m_keys[VK_ESCAPE].bReleased)
		{
			state = EXIT;
			//return true;
		}

		return false;
	}

	void SetUpRecursiveHanoi(int disks)
	{
		int towerA = 0;
		int towerB = 1;
		int towerC = 2;
		HanoiRecursion(disks, towerA, towerC, towerB);


		for (int i = 0; i < moveContainer.getSize(); i++)
		{
			moveset.push_back(move(moveContainer._moves.at(i).from, moveContainer._moves.at(i).to));
		}

		for (int i = 0; i < diskAmount; i++)
			towers[0].push_back(diskAmount - i);
	}



	bool DrawRecursiveHanoiGame(float deltaTime)
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
		//fondo
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_BLACK);
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
			
			Fill(xTowerCoord, yTowerCoord, (xTowerCoord -2) + 3, yTowerCoord + 25, ' ', color);
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
					Fill(xdiskCoord, ydiskCoord, xdiskCoord + (_size + 4) * 2 + 1, ydiskCoord + 3, ' ', BG_DARK_CYAN);
				}
			}
		}

		//volver a menu
		if (m_keys['M'].bReleased)
		{
			ClearVectors();
			return true;
		}

		//end app
		else if (m_keys[VK_ESCAPE].bReleased)
		{
			state = EXIT;
			return true;
		}
		 	
		return false;

		
	}


	void SetUpStackHanoi(int disks)
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_DARK_GREEN);
		
	}



	void ClearVectors()
	{
		moveset.clear();
		for (int i = 0; i < 3; i++)
		{
			towers[i].clear();
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
		
		return 0;
	
}