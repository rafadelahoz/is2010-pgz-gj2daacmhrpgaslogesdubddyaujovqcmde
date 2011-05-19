#include "WorldDecorator.h"

WorldDecorator::WorldDecorator(DBManager* db) : Decorator(db){}

WorldDecorator::~WorldDecorator(){}

void WorldDecorator::init(string zone, string theme, short tileSetId)
{
	Decorator::init(zone, theme, tileSetId);

	if (autoTiler != NULL) delete autoTiler; // si hab�a un autotiler anterior lo borramos
	autoTiler = new WorldAutoTiler(db->getPath("TileSet", tileSetId));	 // creamos el nuevo
}

void WorldDecorator::decorate(Screen* screen)
{
	// Magic goes here!
	// recorremos poniendo 0 en no solid, 1 en solid
	// Y currando a 16x16!

	screen->setIdTileset(idTileset);	// le decimos a la screen cu�l es su tileset

//	if (changedZone) // tenemos que cambiar de terrenos
//	{
		// aqu� habr� getTerrains
//	}

	// Colocamos terrenos
	int waterId = autoTiler->getTerrainId(Terrain::water);
	int floorId = autoTiler->getVariation(waterId, Terrain::walk);
	int solidId = autoTiler->getTerrainId(Terrain::solid);
	int pathId = autoTiler->getVariation(floorId, Terrain::walk);
	
	//Colocamos decoraciones
	int free_space = getFreeSpace(screen);
	int screenSize = SCREEN_WIDTH * SCREEN_HEIGHT;
	
	bool placed;
	int nBigDeco = 0;	//n� decoraciones a colocar
	int auxBigDeco = 0;	//n� decoraciones colocadas

	//Calculamos el n�mero de decoraciones grandes a colocar
	if (free_space > screenSize / 2)
		nBigDeco = 1;

	vector<int>* posUsed = new vector<int>; // Vector de posiciones utilizadas
	int pos;
	int auxPos;

	while (auxBigDeco < nBigDeco)
	{
		pos = screen->getFreePos(posUsed);
		auxPos = pos;

		while(!placed)
		{
		//placed = place_bigDeco(screen, pos);
		auxPos = (auxPos + 1) % (SCREEN_WIDTH*SCREEN_HEIGHT - 1);

		if(auxPos == pos)
			placed = true;
		}
		auxBigDeco++;
		posUsed->push_back(pos);
	}

	delete posUsed; posUsed = NULL;

	// Recorremos la lista de decoraciones convierti�ndolas en entidades (guard�ndolas en la screen)
	list<Decoration*>::iterator it;
	for (it = decorationList.begin(); it != decorationList.end(); it++)
		if (*it != NULL)
			screen->addEntity((*it)->toEntities());

	clearDecorations();

//	if (free_space > screenSize / 4)
//		free_space = place_templates();

	// funci�n de calcular mierda de johan


/*
	// Colocamos terrenos
	int waterId = autoTiler->getTerrainId(Terrain::water);
	int floorId = autoTiler->getVariation(waterId, Terrain::walk);
	int solidId = autoTiler->getTerrainId(Terrain::solid);
	int pathId = autoTiler->getVariation(floorId, Terrain::walk);

	if (OwScreen* ows = dynamic_cast<OwScreen*> (screen)){
		// Colocamos decoraciones
		Decoration* decoPath = autoTiler->getDecoration(Decoration::solid, Decoration::small, pathId); 
		Decoration* decoFloor = autoTiler->getDecoration(Decoration::solid, Decoration::small, floorId); 
		Decoration* decoMedium = autoTiler->getDecoration(Decoration::solid, Decoration::medium, floorId);
		Decoration* decoBig = autoTiler->getDecoration(Decoration::solid, Decoration::big, pathId);

		// Buscamos una posici�n libre
		vector<int>* posUsed = new vector<int>; // Vector de posiciones utilizadas
		int pos;

		// Deco1
		pos = ows->getFreePos(posUsed);
		if (pos != -1){
			// Ponemos alguna decoracion en la posici�n pos
			posUsed->push_back(pos);
			// Inicializamos la decoraci�n
			decoPath->init(pos % SCREEN_WIDTH, pos / SCREEN_WIDTH);
			// La guardamos en la lista de decoraciones (si no colisiona con ninguna)
			if (checkDecoCollision(decoPath) && isInBounds(decoPath, screen) && checkSolidCollision(decoPath, screen))
				decorationList.push_back(decoPath);
		}

		// Deco2
		pos = ows->getFreePos(posUsed);
		if (pos != -1){
			// Ponemos alguna decoracion en la posici�n pos
			posUsed->push_back(pos);
			// Inicializamos la decoraci�n
			decoFloor->init(pos % SCREEN_WIDTH, pos / SCREEN_WIDTH);
			// La guardamos en la lista de decoraciones (si no colisiona con ninguna)
			if (checkDecoCollision(decoFloor) && isInBounds(decoFloor, screen) && checkSolidCollision(decoFloor, screen))
				decorationList.push_back(decoFloor);
		}

		// Deco3
		pos = ows->getFreePos(posUsed);
		if (pos != -1){
			// Ponemos alguna decoracion en la posici�n pos
			posUsed->push_back(pos);
			// Inicializamos la decoraci�n
			decoMedium->init(pos % SCREEN_WIDTH, pos / SCREEN_WIDTH);
			// La guardamos en la lista de decoraciones (si no colisiona con ninguna)
			if (checkDecoCollision(decoMedium) && isInBounds(decoMedium, screen) && checkSolidCollision(decoMedium, screen))
				decorationList.push_back(decoMedium);
		}

		// Deco4
		pos = ows->getFreePos(posUsed);
		if (pos != -1){
			// Ponemos alguna decoracion en la posici�n pos
			posUsed->push_back(pos);
			// Inicializamos la decoraci�n
			decoBig->init(pos % SCREEN_WIDTH, pos / SCREEN_WIDTH);
			// La guardamos en la lista de decoraciones (si no colisiona con ninguna)
			if (checkDecoCollision(decoBig) && isInBounds(decoBig, screen) && checkSolidCollision(decoBig, screen))
				decorationList.push_back(decoBig);
		}

		// Recorremos la lista de decoraciones convierti�ndolas en entidades (guard�ndolas en la screen)
		list<Decoration*>::iterator it;
		for (it = decorationList.begin(); it != decorationList.end(); it++)
			if (*it != NULL)
				ows->addEntity((*it)->toEntities());

		// Borramos el vector de posiciones
		delete posUsed;
		posUsed = NULL;

		// Borramos la lista de decoraciones
		clearDecorations();
	}	

	// Terrenos -----------------------------------------------------------------------------------

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
				terrainIdMatrix[i][j] = waterId;//, screen->setSolid(i, j, 1);
			else if (type == 3)
				terrainIdMatrix[i][j] = pathId, screen->setSolid(i, j, 0);
			else if (type == 4)
				terrainIdMatrix[i][j] = solidId, screen->setSolid(i, j, 1);
			else
				terrainIdMatrix[i][j] = solidId, screen->setSolid(i, j, 1);
		}
	}
*/
	// Terrenos

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
				terrainIdMatrix[i][j] = waterId;//, screen->setSolid(i, j, 1);
			else if (type == 3)
				terrainIdMatrix[i][j] = pathId, screen->setSolid(i, j, 0);
			else if (type == 4)
				terrainIdMatrix[i][j] = solidId, screen->setSolid(i, j, 1);
			else
				terrainIdMatrix[i][j] = solidId, screen->setSolid(i, j, 1);
		}
	}

	terrainsToTiles(screen);

	// limpiamos la matriz de terrenos
	clearTerrains();
};

bool WorldDecorator::place_bigDeco(Screen* s, int pos)
{
	int x = pos % SCREEN_WIDTH;
	int y = pos / SCREEN_WIDTH;
	short tileType = -1;
	int distMin = 100;
	int distAux;
	int type;

	//Calculo que elementos rodean la posici�n donde queremos colocar la decoraci�n
	for(int i = x; i < x + 3; i++)
		for(int j = y; j < j + 3; j++)
		{
			if(i > SCREEN_WIDTH)
				i = SCREEN_WIDTH;
			if(j > SCREEN_HEIGHT)
				j = SCREEN_HEIGHT;

			//Calculo la distancia a la que me encuentro de la decoraci�n
			distAux = min(abs(x - i),abs(y - j));

			short tileType = s->getSolid(i, j);

			if (tileType == 1)
			{
				if (distAux < distMin)
					type = 3;
			}
			else if (tileType == 2)
			{
				if (distAux < distMin)
					type = 2;
			}
			else if (tileType == 3)
			{
				if (distAux < distMin)
					type = 1;
			}
		}

	// colocamos la decoraci�n
		Decoration* decoFloor = autoTiler->getDecoration(Decoration::solid, Decoration::small, -1, (Decoration::DecorationNear) type); 
	decoFloor->init(pos % SCREEN_WIDTH, pos / SCREEN_WIDTH);
		// La guardamos en la lista de decoraciones (si no colisiona con ninguna)
		if (checkDecoCollision(decoFloor) && isInBounds(decoFloor, s) && checkSolidCollision(decoFloor, s))
			decorationList.push_back(decoFloor);
	
	
	return true;
}