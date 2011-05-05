#include "Decorator.h"

Decorator::Decorator()
{
	theme = "";
	zone = "";
	autoTiler = NULL;
	terrainIdMatrix = NULL;
}

Decorator::~Decorator()
{
	if (autoTiler != NULL) delete autoTiler;
}

void Decorator::init(string zone, string theme, string tileSetPath)
{
	this->zone = zone;
	this->theme = theme;

	if (autoTiler != NULL) delete autoTiler; // si hab�a un autotiler anterior lo borramos
	autoTiler = new AutoTiler(tileSetPath);	 // creamos el nuevo
}

void Decorator::decorate(Screen* screen)
{
	// Magic goes here!
	// recorremos poniendo 0 en no solid, 1 en solid
	// Y currando a 16x16!


	// Colocamos terrenos
	int waterId = autoTiler->getTerrainId(Terrain::water);
	int floorId = autoTiler->getVariation(waterId, Terrain::walk);
	int solidId = autoTiler->getTerrainId(Terrain::solid);
	int pathId = autoTiler->getVariation(floorId, Terrain::walk);

	terrainIdMatrix = (int**) malloc(sizeof(int*)*SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		terrainIdMatrix[i] = (int*) malloc(sizeof(int)*SCREEN_HEIGHT);
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			short type = screen->getSolid(i, j);
			if (type == 0)
				terrainIdMatrix[i][j] = floorId;
			else if (type == 1)
				terrainIdMatrix[i][j] = solidId;
			else if (type == 2)
				terrainIdMatrix[i][j] = waterId, screen->setSolid(i, j, 1);
			else if (type == 3)
				terrainIdMatrix[i][j] = pathId, screen->setSolid(i, j, 0);
			else if (type == 4)
				terrainIdMatrix[i][j] = solidId, screen->setSolid(i, j, 1);
			else
				terrainIdMatrix[i][j] = solidId, screen->setSolid(i, j, 1);
		}
	}

/*	// Colocamos decoraciones
	Decoration* decoPath = autoTiler->getDecoration(Decoration::solid, Decoration::small, pathId); 
	Decoration* decoFloor = autoTiler->getDecoration(Decoration::solid, Decoration::small, floorId); 
	Decoration* decoMedium = autoTiler->getDecoration(Decoration::solid, Decoration::medium, floorId);
	Decoration* decoBig = autoTiler->getDecoration(Decoration::solid, Decoration::big, pathId);

	vector<int>* posUsed = new vector<int>; // Vector de posiciones utilizadas
	int pos = screen->getFreePos(posUsed);
	// Ponemos alguna decoracion en la posici�n pos
	posUsed->push_back(pos);

	// Borramos el vector de posiciones
	delete posUsed;
	posUsed = NULL;*/

	// Ahora se guarda en screen
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			autoTiler->getTerrain(terrainIdMatrix[i][j])->toTiles(terrainIdMatrix, screen, SCREEN_WIDTH, SCREEN_HEIGHT, i, j);
		}
	}

	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		delete (terrainIdMatrix[i]);
	}
	delete (terrainIdMatrix);

};

short Decorator::gimmeTile() {
	// To be implemented
	return 0;
}