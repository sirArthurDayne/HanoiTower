#include"olcConsoleGameEngine.h"
/*
	REGLAS:
	1. Solo un disco puede ser movido por vez.
	2. El disco removido solo debe ser colocado en una torre
	3. Un disco grande no puede ser colocado encima de uno pequeno
	
	  -             |            |
	-----			|			 |
  ---------			|			 |
	  |				|			 |
	  |				|			 |

	TORRE[A]      TORRE[B]     TORRE[C]
	    1            2            3
 */

void HanoiRecursion(int nDiscos, int first, int middle, int last)
{
	if (nDiscos > 0)
	{
		HanoiRecursion(nDiscos -1, first, last, middle);
		std::cout << "Mover Disco-->" << nDiscos << " desde---> " << first << " HASTA " << middle << ".\n";
		HanoiRecursion(nDiscos -1, last, middle, first);
	}
}

int main()
{
	int towerA = 1;
	int towerB = 2;
	int towerC = 3;
	int discosCantidad;

	std::cout << "Ingrese el Numero de discos--->";
	std::cin >> discosCantidad;
	std::cout << "-------------------------";
	std::cout << "\nLos movimientos a realizar son estos:\n";
	HanoiRecursion(discosCantidad, towerA, towerC, towerB);	


	std::cin.get();
	std::cin.get();
	return 0;
}