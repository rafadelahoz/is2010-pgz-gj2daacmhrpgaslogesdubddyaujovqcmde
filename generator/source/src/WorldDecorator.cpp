#include "WorldDecorator.h"

WorldDecorator::WorldDecorator(DBManager* db) : Decorator(db){}

WorldDecorator::~WorldDecorator(){}

void WorldDecorator::init(string zone, short tileSetId)
{
	Decorator::init(zone, tileSetId);

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
		if (info.waterId != -1)
			info.floorId = autoTiler->getVariation(info.waterId, Terrain::floor);
		else
			info.floorId = autoTiler->getTerrainId(Terrain::floor);

		info.solidId = autoTiler->getTerrainId(Terrain::solid);
		info.pathId = autoTiler->getVariation(info.floorId, Terrain::path);
		
		changedZone = false;
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
			else if (type == 5)	// entrada a mazmorra (ponemos suelo, ya se pondr� la decoraci�n
				terrainIdMatrix[i][j] = info.floorId, screen->setSolid(i, j, 1);
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

	// pedimos l la decoraci�n
	decoFloor = autoTiler->getDecoration(type, size, -1, (Decoration::DecorationNear) near); 
	}
	else
		decoFloor = autoTiler->getDecoration(idDeco);

	if(decoFloor != NULL)
	{
		decoFloor->init(pos % SCREEN_WIDTH, pos / SCREEN_WIDTH);
		// La guardamos en la lista de decoraciones (si no colisiona con ninguna)
		if (checkDecoCollision(decoFloor) && isInBounds(decoFloor, s) && checkSolidCollision(decoFloor, s)
			&& checkBlockingPath(decoFloor, s) && checkEntitiesCollision(decoFloor, s))
		{
			decorationList.push_back(decoFloor);
			return decoFloor->getDecorationData().idDeco;
		}	
	}

	return -1;
}

void WorldDecorator::decorateDunEntrance(Screen* screen, int col, int row)
{
	// quitamos los s�lidos que ha puesto el generador (donde vamos a poner la decoraci�n)
	screen->setSolid(col-1, row, 0);
	screen->setSolid(col-1, row-1, 0);
	screen->setSolid(col, row-1, 0);
	screen->setSolid(col+1, row-1, 0);
	screen->setSolid(col+1, row, 0);

	// cogemos la decoraci�n de la mazmorra final
	Decoration* decoEntrance = autoTiler->getDecoration(Decoration::DecorationType::dungeonEntrance, Decoration::DecorationSize::sNone, info.floorId);

	if(decoEntrance == NULL)
		return; // caca, no hay ninguno

	// colocamos la decoraci�n
	decoEntrance->init(col - (decoEntrance->getDecorationData().width / 2), row - (decoEntrance->getDecorationData().height - 1));

	// si no colisiona, la guardamos en la lista de decoraciones
	if (checkDecoCollision(decoEntrance) && isInBounds(decoEntrance, screen) && checkSolidCollision(decoEntrance, screen))
		decorationList.push_back(decoEntrance);
	else
	{
		// cogemos la entrada m�s b�sica que cabe fijo
		decoEntrance = ((WorldAutoTiler*)autoTiler)->getDecoDunEntrance(info.floorId);
		// colocamos la decoraci�n
		decoEntrance->init(col - (decoEntrance->getDecorationData().width / 2), row - (decoEntrance->getDecorationData().height - 1));
		if (decoEntrance == NULL)
			return; // caca otra vez
		// si no colisiona la ponemos
		else if (checkDecoCollision(decoEntrance) && isInBounds(decoEntrance, screen) && checkSolidCollision(decoEntrance, screen))
			decorationList.push_back(decoEntrance);
	}

	// Recorremos la lista de decoraciones convierti�ndolas en entidades (guard�ndolas en la screen)
	list<Decoration*>::iterator it;
	for (it = decorationList.begin(); it != decorationList.end(); it++)
		if (*it != NULL)
			screen->addEntity((*it)->toEntities());

	// borramos la lista de decoraciones
	clearDecorations();
}
