#include <iostream>
#include"olcConsoleGameEngine.h"
#include<string>

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
	    1            2            3
 */

struct move {
	move(int from, int to) {
		this->from = from;
		this->to = to;

	}
	int from, to;
};

void HanoiRecursion(int nDisks, int first, int middle, int last, std::vector<move>& _moves)
{
	//nDisks work has iterator
	if (nDisks > 0)
	{
		HanoiRecursion(nDisks - 1, first, last, middle, _moves);
		_moves.push_back(move(first, middle));
		HanoiRecursion(nDisks - 1, last, middle, first, _moves);
	}
}



class HanoiGames : public olcConsoleGameEngine {
public:
	std::vector<move> moveset;
	std::vector<int> towers[3];
	bool OnUserCreate()
	{

		int towerA = 0;
		int towerB = 1;
		int towerC = 2;
		int diskAmount = 3;
		HanoiRecursion(diskAmount, towerA, towerC, towerB, moveset);
		for (int i = 0; i < diskAmount; i++) towers[0].push_back(diskAmount - i);
		return true;
	}


	bool OnUserUpdate(float fElapsedTime)
	{
		static float timer = 0;
		static int nmove = 0;
		timer += fElapsedTime;
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
		Fill(0, 0, 160, 100, ' ', BG_DARK_BLUE);
		for (int i = 0; i < 3; i++) {
			if (towers[i].size() > 0) {
				for (int j = 0; j < towers[i].size(); j++) {
					int _size = towers[i].at(j);

					Fill(20 + i * 40 - (_size + 4), 90 - (j * 3), (20 + i * 40 - (_size + 4)) + (_size + 4) * 2 + 1, (90 - (j * 3)) + 3, ' ', BG_DARK_RED);
				}
			}
		}

		return true;
	}



};



int main()
{

	HanoiGames game;

	game.ConstructConsole(160, 100, 8, 8);

	game.Start();



	std::cin.get();
	std::cin.get();
	return 0;
}