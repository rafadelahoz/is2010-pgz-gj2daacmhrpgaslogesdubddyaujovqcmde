#pragma once

#ifndef _AUTOTERRAIN_H_
#define _AUTOTERRAIN_H_

#include "Terrain.h"

class AutoTerrain : public Terrain{
	private:

		/* Vector con las distintas posiciones del terreno (esquina superior izquieda, centro...):
		0  1  2
		3  4  5
		6  7  8
		9  10 11*/

		// Devuelve el identificador del tile en la posici�n indicada dentro del autotile
		int getTileIdAt(int row, int col);
	
	public:
		// Constructora
		AutoTerrain();
		// Destructora
		virtual ~AutoTerrain();

		// Hablar este m�todo
		void toTiles(int** source, int** screen, int w, int h, int i, int j);
};

#endif