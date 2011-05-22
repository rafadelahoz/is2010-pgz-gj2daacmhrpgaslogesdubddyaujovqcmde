#include "WorldDecorator.h"

WorldDecorator::WorldDecorator(DBManager* db) : Decorator(db){}

WorldDecorator::~WorldDecorator(){}

void WorldDecorator::init(string zone, string theme, short tileSetId)
{
	Decorator::init(zone, theme, tileSetId);

	info.floorId = -1;
	info.pathId = -1;
	info.solidId = -1;
	info.waterId = -1;

	if (autoTiler != NULL) delete autoTiler; // si hab�a un autotiler anterior lo borramos
	autoTiler = new WorldAutoTiler(db->getPath("TileSet", tileSetId));	 // creamos el nuevo
}

void WorldDecorator::decorate(Screen* screen)
{
	// Magic goes here!
	// recorremos poniendo 0 en no solid, 1 en solid
	// Y currando a 16x16!

	screen->setIdTileset(idTileset);	// le decimos a la screen cu�l es su tileset

	// Cambiamos o mantenemos los terrenos con los que estamos decorando
	if (changedZone) // tenemos que cambiar de terrenos
	{
		info.waterId = autoTiler->getTerrainId(Terrain::water);
		info.floorId = autoTiler->getVariation(info.waterId, Terrain::walk);
		info.solidId = autoTiler->getTerrainId(Terrain::solid);
		info.pathId = autoTiler->getVariation(info.floorId, Terrain::walk);
	}

//-------------------------------------------------- ALGORITMO DE COLOCAR DECORACIONES -----------------------------------------------------------	
	//Colocamos decoraciones
	int screenSize = SCREEN_WIDTH * SCREEN_HEIGHT;
	
	int nDecos = 0;							// n� decoraciones a colocar
	vector<int>* posUsed = new vector<int>; // Vector de posiciones utilizadas

	// ***** DECORACIONES GRANDES ******

	//Calculamos el n�mero de decoraciones grandes a colocar
	if (getFreeSpace(screen) > screenSize / 4)
		nDecos = 1;

	// Colocamos las decoraciones grandes
	place_decos(screen, nDecos, Decoration::DecorationSize::big, posUsed);

	// ***** DECORACIONES MEDIANAS ******

	//Calculamos n� de decoraciones medianas a colocar
	if(getFreeSpace(screen) > screenSize / 16)
		nDecos = 2;
	else
		nDecos = 0;

	// Colocamos las decoraciones medianas
	place_decos(screen, nDecos, Decoration::DecorationSize::medium, posUsed);

	// ***** DECORACIONES PEQUE�AS ******

	//Calculamos n� de decoracioens peque�as a colocar
	if(getFreeSpace(screen) > screenSize/32)
		nDecos = ((rand() % 7) + 1);
	else
		nDecos = 0;

	//Colocamos las decoraciones peque�as
	place_decos(screen, nDecos, Decoration::DecorationSize::small, posUsed);

	// borramos el vector de posiciones auxiliares
	delete posUsed; posUsed = NULL;

	// Recorremos la lista de decoraciones convierti�ndolas en entidades (guard�ndolas en la screen)
	list<Decoration*>::iterator it;
	for (it = decorationList.begin(); it != decorationList.end(); it++)
		if (*it != NULL)
			screen->addEntity((*it)->toEntities());

	// borramos la lista de decoraciones
	clearDecorations();
//--------------------------------------------HASTA AQUI DECORACIONES -----------------------------------------------------------------------
	
	// Terrenos

	// los colocamos en la screen
	terrainIdMatrix = (int**) malloc(sizeof(int*)*SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		terrainIdMatrix[i] = (int*) malloc(sizeof(int)*SCREEN_HEIGHT);
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			short type = screen->getSolid(i, j);
			if (type == 0)
				terrainIdMatrix[i][j] = info.floorId;
			else if (type == 1)
				terrainIdMatrix[i][j] = info.solidId;
			else if (type == 2)
				terrainIdMatrix[i][j] = info.waterId;//, screen->setSolid(i, j, 1);
			else if (type == 3)
				terrainIdMatrix[i][j] = info.pathId, screen->setSolid(i, j, 0);
			else if (type == 4)
				terrainIdMatrix[i][j] = info.solidId, screen->setSolid(i, j, 1);
			else
				terrainIdMatrix[i][j] = info.solidId, screen->setSolid(i, j, 1);
		}
	}

	// los pintamos en la screen
	terrainsToTiles(screen);

	// limpiamos la matriz de terrenos
	clearTerrains();
};

void WorldDecorator::place_decos(Screen* screen, int nDecos, Decoration::DecorationSize size, std::vector<int>* posUsed)
{
	int attempts = 0;		// n� de intentos al colocar una decoraci�n
	int placedDecos = 0;	// n� decoraciones colocadas
	int idDeco = -1;		// id de la decoraci�n que estamos poniendo
	int pos;				// posici�n auxiliar donde se va a colocar la decoraci�n

	while (placedDecos < nDecos && attempts < N_ATTEMPTS)
	{
		// elegimos una posici�n al azar de la pantalla
		pos = screen->getFreePos(posUsed);

		// intentamos colocar una decoraci�n ah�
		idDeco = place_deco(screen, size, Decoration::DecorationType::tNone, idDeco, pos);

		if (idDeco != -1) // si lo hemos conseguido
		{
			placedDecos++;				// aumentamos el n�mero de decoraciones colocadas
			posUsed->push_back(pos);	// marcamos como usada esa posici�n
		}
		else	// si no lo hemos conseguido aumentamos el n�mero de intentos
			attempts++;
	}
}

int WorldDecorator::place_deco(Screen* s, Decoration::DecorationSize size, Decoration::DecorationType type, int idDeco,int pos)
{
	int x = pos % SCREEN_WIDTH;
	int y = pos / SCREEN_WIDTH;
	short tileType = -1;
	int distMin = 100;
	int distAux;
	int near = Decoration::DecorationNear::nNone;
	Decoration* decoFloor;

	if(idDeco == -1)
	{

	//Calculo que elementos rodean la posici�n donde queremos colocar la decoraci�n
	for(int i = x; i < x + 3; i++)
		for(int j = y; j < y + 3; j++)
		{
			if(!(i > SCREEN_WIDTH || j > SCREEN_HEIGHT))
			{
				//Calculo la distancia a la que me encuentro de la decoraci�n
				distAux = max(abs(x - i),abs(y - j));

				short tileType = s->getSolid(i, j);

				if (tileType == 1)
				{
					if (distAux < distMin)
					{
						near = 3;
						distMin = distAux;
					}
				}
				else if (tileType == 2)
				{
					if (distAux < distMin)
					{
						near = 2;
						distMin = distAux;
					}
				}
				else if (tileType == 3)
				{
					if (distAux < distMin)
					{
						near = 1;
						distMin = distAux;
					}
				}
			}
	}//for

	// colocamos la decoraci�n
	decoFloor = autoTiler->getDecoration(type, size, -1, (Decoration::DecorationNear) near); 
	}
	else
		decoFloor = autoTiler->getDecoration(idDeco);

	if(decoFloor != NULL)
	{
		decoFloor->init(pos % SCREEN_WIDTH, pos / SCREEN_WIDTH);
		// La guardamos en la lista de decoraciones (si no colisiona con ninguna)
		if (checkDecoCollision(decoFloor) && isInBounds(decoFloor, s) && checkSolidCollision(decoFloor, s))
		{
			decorationList.push_back(decoFloor);
			return decoFloor->getDecorationData().idDeco;
		}	
	}

	return -1;
}

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