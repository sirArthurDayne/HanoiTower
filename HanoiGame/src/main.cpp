
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

	bool OnUserCreate()
	{

		//int towerA = 0;
		//int towerB = 1;
		//int towerC = 2;
		//diskAmount = 3;
		//HanoiRecursion(diskAmount, towerA, towerC, towerB);

		


		return true;
	}


	bool OnUserUpdate(float fElapsedTime)
	{
		if (state == START)
		{
			if (GameStartMenu())
			{
				state = RECURSIVEGAME;
			}
		}
		if (state == RECURSIVEGAME)
		{
			if (DrawRecursiveHanoiGame(fElapsedTime))
			{
				state = EXIT;
			}
		}
		

		if (state == EXIT)
			return false;
		
		return true;
	}


	bool GameStartMenu()
	{
		//background
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_DARK_BLUE);
		
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
		
		//letter for SCORES
		
		//letter for Exit
		if (m_keys[VK_ESCAPE].bReleased)
		{
			state = EXIT;
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
		if (timer > 0.5) {
			timer -= 0.5;
			if (nmove < moveset.size()) {
				towers[moveset[nmove].to].push_back(towers[moveset[nmove].from].back());
				towers[moveset[nmove].from].pop_back();
				towers[moveset[nmove].from].shrink_to_fit();
				towers[moveset[nmove].to].shrink_to_fit();

				nmove++;
			}
		}
		//fondo
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ', BG_BLACK);
		
		//dibujar torres
		Fill(20 + (0 * 40), 90 - 25, (18 + (0 * 40)) + 3, 90, ' ', BG_DARK_BLUE);
		Fill(20 + (1 * 40), 90 - 25, (18 + (1 * 40)) + 3, 90, ' ', BG_DARK_GREEN);
		Fill(20 + (2 * 40), 90 - 25, (18 + (2 * 40)) + 3, 90, ' ', BG_DARK_RED);
		//torres texto
		DrawString(18 + (0 * 40), 90 - 30, L"TORRE 0", FG_DARK_BLUE);
		DrawString(18 + (1 * 40), 90 - 30, L"TORRE 1", FG_DARK_GREEN);
		DrawString(18 + (2 * 40), 90 - 30, L"TORRE 2", FG_DARK_RED);

		for (int i = 0; i < 3; i++) {

			if (towers[i].size() > 0) {
				for (int j = 0; j < towers[i].size(); j++) {
					
					//drawing the disks
					int _size = towers[i].at(j);
					int xdiskCoord = 20 + i * 40 - (_size + 4);
					int ydiskCoord = 90 - (j * 3);
					Fill(xdiskCoord, ydiskCoord, xdiskCoord + (_size + 4) * 2 + 1, ydiskCoord + 3, ' ', BG_DARK_CYAN);
				}
			}
		}

			
		return false;

		
	}

};



int main()
{

	mainEngine game;

	game.ConstructConsole(160, 100, 8, 8);

	game.Start();



	std::cin.get();
	std::cin.get();
	return 0;
}