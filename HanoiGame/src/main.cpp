
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
#include"HanoiGame.h"




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