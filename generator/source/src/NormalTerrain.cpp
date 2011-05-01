#include "NormalTerrain.h"

// Constructora
NormalTerrain::NormalTerrain() : Terrain()
{
}

// Destructora
NormalTerrain::~NormalTerrain()
{
}

// i columnas, j filas
void NormalTerrain::toTiles(int** source, Screen* screen, int w, int h, int i, int j)
{
	int col = 2*i;
	int row = 2*j;

	if (i < w && i >= 0 && j < h && j >= 0 && screen != NULL){
		// simplemente pone su tile gr�fico en la posici�n indicada de la pantalla
		screen->setTile(col, row, tile*2);
		screen->setTile(col+1, row, tile*2 + 1);
		screen->setTile(col, row+1, tile*2 + chipsetW/8);
		screen->setTile(col+1, row+1, tile*2 + chipsetW/8 + 1);
	}
}

void NormalTerrain::toSolids(int** screen, int w, int h, int i, int j)
{
	// ??
}