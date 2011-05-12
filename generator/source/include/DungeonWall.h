#pragma once

#ifndef _DUNGEONWALL_H_
#define _DUNGEONWALL_H_

#include <vector>
#include "AutoTerrain.h"

class DungeonWall : public AutoTerrain{
	private:

		void setTile(Screen* s, int col, int row, int tile);

	public:

		// Constructora
		DungeonWall();
		// Destructora
		virtual ~DungeonWall();

		// Inicializaci�n de atributos
		void init(int wallId, int tileId, vector<int> variations, int chipsetWidth);

		// Hablar este m�todo
		void toTiles(int** source, Screen* screen, int w, int h, int i, int j);
};

#endif