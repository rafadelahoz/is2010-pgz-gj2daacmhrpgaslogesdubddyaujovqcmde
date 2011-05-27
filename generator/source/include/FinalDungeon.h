#pragma once

#ifndef _FINALDUNGEON_H_
#define _FINALDUNGEON_H_

#define FINAL_SCREEN 0
#define LOCK_SCREEN 1

#include "Dungeon.h"
#include "DunScreen.h"
#include "Screen.h"
#include "DBManager.h"

/*
		       Lock Screen									  Final Screen

		1 1 1 1 1 1 L L L L 1 1 1 1 1 1				1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1	
		1 1	1 1 1 1 L L L L 1 1 1 1 1 1				1 1	1 1 1 1 1 1 1 1 1 1 1 1 1 1
		1 1					        1 1				1 1   2 2 2    FE   2 2 2   1 1
		1 1    			            1 1				1 1   2 2 2         2 2 2   1 1	
		1 1   2 2 2		    2 2 2   1 1				1 1   2 2 2 2	  2	2 2 2   1 1
		1 1	  2 2 2         2 2 2   1 1				1 1	  2 2 2 2     2 2 2 2   1 1
		1 1   2 2 2         2 2 2   1 1				1 1         2     2         1 1
		1 1                         1 1				1 1   2                 2   1 1
		1 1                         1 1				1 1       2        2        1 1
		1 1                         1 1				1 1   2                 2   1 1
		1 1                         1 1		        1 1       2        2        1 1
		1 1                         1 1				1 1   2                 2   1 1
		1 1 1 1 1 1         1 1 1 1 1 1				1 1 1 1 1 1 1     1 1 1 1 1 1 1
		1 1 1 1 1 1         1 1 1 1 1 1				1 1 1 1 1 1 1     1 1 1 1 1 1 1
		
*/
class FinalDungeon: public Dungeon {

	private:

		void generateLockScreen();

		void generateFinalScreen(); 

		void allocateSolids(DunScreen* ds, int type);

		// puertas y paredes copia de dunScreen // por no hacer p�blico el que ya tiene
		void placeWalls(DunScreen* ds);

	public:
		// Debe encargarse de realizar las operaciones con la BDI
		FinalDungeon(string zone, string theme, short gameDiff, short dungNumber, short ratio, short tool, short keyObj, DungeonPos dungeonPos, DBManager* db);

		// Destructora
		~FinalDungeon();

		// Desencadena la generaci�n de la mazmorra 
		void generate();
};

#endif