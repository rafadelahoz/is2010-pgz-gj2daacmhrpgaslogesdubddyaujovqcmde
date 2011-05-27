#include "DunDecorator.h"

DunDecorator::DunDecorator(DBManager* db) : Decorator(db){}

DunDecorator::~DunDecorator(){}

void DunDecorator::init(string zone, short tileSetId){
	Decorator::init(zone, tileSetId);

	info.terrainId = -1;
	info.statueId = -1;
	info.torchId = -1;

	if (autoTiler != NULL) delete autoTiler; // si hab�a un autotiler anterior lo borramos
	autoTiler = new DungeonAutoTiler(db->getPath("TileSet", tileSetId));	 // creamos el nuevo
}

void DunDecorator::decorate(Screen* screen){

	screen->setIdTileset(idTileset);

// Terrenos y muros
	place_terrains(screen);

// DECORACIONES ---------------------------------------------------------------------------------------------------------------

	// colocamos objetos en las paredes (antorchas o lo que toque)
	place_torchs(screen);

	// colocamos elementos pisables en cualquier sitio de la mazmorra
	place_walkables(screen);

	// colocamos las estatuas por la pantalla si es posible
	place_statues(screen);

	// Recorremos la lista de decoraciones convierti�ndolas en entidades (guard�ndolas en la screen)
	list<Decoration*>::iterator it;
	for (it = decorationList.begin(); it != decorationList.end(); it++)
		if (*it != NULL)
			screen->addEntity((*it)->toEntities());

	// Borramos la lista de decoraciones
	clearDecorations();

// Fin decoraciones ---------------------------------------------------------------------------------------------
}

void DunDecorator::place_terrains(Screen* screen){
	// TERRENOS ----------------------------------------------------------------------------------------------------------

	int terrainVar; // variaci�n del terreno principal que puede aparecer en ciertas pantallas de la mazmorra

	// Cambiamos o mantenemos los terrenos con los que estamos decorando
	if (changedZone) // tenemos que cambiar de terrenos
	{
		info.wallId = ((DungeonAutoTiler*) autoTiler)->getTerrainId(Terrain::TerrainType::dungeonWall);
		info.terrainId = autoTiler->getTerrainId(Terrain::TerrainType::floor);
		changedZone = false;

	}
	else // cogemos una variaci�n para el suelo
	{
		terrainVar = autoTiler->getVariation(info.terrainId, Terrain::TerrainType::floor);
		if (terrainVar != -1)	// si hay variaciones, intentamos variar el terreno principal
		{
			// damos un 33% de posibilidades de que se ponga la variaci�n
			int r = rand() % 3;
			if (r == 0)
				info.terrainId = terrainVar;
		}
	}

	// colocaci�n de los terrenos en la mazmorra
	terrainIdMatrix = (int**) malloc(sizeof(int*)*SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		terrainIdMatrix[i] = (int*) malloc(sizeof(int)*SCREEN_HEIGHT);
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			short type = screen->getSolid(i, j);
			if (type == 1)
				terrainIdMatrix[i][j] = info.wallId;
			else if (type != 25) // si no es un tile de entrada a mazmorra
				terrainIdMatrix[i][j] = info.terrainId;
		}
	}

	// Ahora se guarda en screen (los terrenos)
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			((DungeonAutoTiler*) autoTiler)->getTerrain(terrainIdMatrix[i][j])->toTiles(terrainIdMatrix, screen, SCREEN_WIDTH, SCREEN_HEIGHT, i, j);
		}
	}

	// colocamos suelo debajo de las entidades (muros) para cosas de perspectiva del player
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			if (screen->isThereAnyEntityAt(screen->getEntities(), j*SCREEN_WIDTH + i)){
				screen->setSolid(i, j, 0);
				((DungeonAutoTiler*) autoTiler)->getTerrain(info.terrainId)->toTiles(terrainIdMatrix, screen, SCREEN_WIDTH, SCREEN_HEIGHT, i, j);
			}
		}
	}

	// limpiamos la matriz de terrenos
	clearTerrains();
}

void DunDecorator::place_walkables(Screen* screen)
{
	int pos;
	// intentamos poner hasta NWALKABLES decoraciones
	for (int i = 0; i < NWALKABLES; i++)
	{
		// elegimos una posici�n al azar
		pos = rand() % (SCREEN_WIDTH * SCREEN_HEIGHT);
		// pedimos una decoraci�n
		Decoration* d = autoTiler->getDecoration(Decoration::DecorationType::walkable, Decoration::DecorationSize::small,
			info.terrainId);
		// Si no existen decoraciones walkables-> salimos!!
		if (d == NULL)
			return;
		// Inicializamos la decoraci�n
		d->init(pos % SCREEN_WIDTH, pos / SCREEN_HEIGHT);
		// La a�adimos a la lista de decoraciones
		if (checkDecoCollision(d) && isInBounds(d, screen) && checkSolidCollision(d, screen) && checkWallCollision(d, screen))
			decorationList.push_back(d);
	}
}

void DunDecorator::place_statues(Screen* screen){

	int idStatue1, idStatue2;

	Decoration* decoStatueAux = autoTiler->getDecoration(Decoration::dungeonStatue, Decoration::small, info.terrainId);
	// si no existen estatuas...
	if (decoStatueAux == NULL)
		return;

	idStatue1 = decoStatueAux->getDecorationData().idDeco;
	delete decoStatueAux; decoStatueAux = NULL;

	decoStatueAux = autoTiler->getDecoration(Decoration::dungeonStatue, Decoration::small, info.terrainId);
	idStatue2 = decoStatueAux->getDecorationData().idDeco;
	delete decoStatueAux; decoStatueAux = NULL;

	// colocamos estatuas (o lo que toque) en las posiciones indicadas por el generador
	for (int i = 0; i < SCREEN_WIDTH; i++)	
		for (int j = 0; j < SCREEN_HEIGHT; j++){
			// Si leemos un s�lido (ponemos estatua)
			if (screen->getSolid(i, j) == 2){	// Identificar que hay un solido (estatua)
				// cogemos una estatua diferente dependiendo de la simetr�a
				Decoration* decoStatue;
				if (i <= SCREEN_WIDTH / 2)
					decoStatue = autoTiler->getDecoration(idStatue1);
				else
					decoStatue = autoTiler->getDecoration(idStatue2);

				// Si no existen decoraciones walkables-> salimos!!
				if (decoStatue == NULL)
					return;
				// Inicializamos la decoraci�n
				decoStatue->init(i, j - (decoStatue->getDecorationData().height - 1));
				// Cambiamos el s�lido para que sea pasable por detr�s
				screen->setSolid(i, j, 0);
				// La a�adimos a la lista de decoraciones
				if (checkDecoCollision(decoStatue) && isInBounds(decoStatue, screen) && checkSolidCollision(decoStatue, screen))
					decorationList.push_back(decoStatue);
			}
		}
}

bool DunDecorator::checkWallCollision(Decoration* d, Screen* screen)
{
	int w =  d->getDecorationData().width;
	int h = d->getDecorationData().height;
	int x = d->x;
	int y = d->y;

	// miramos si chocamos con alguna entidad
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			if (screen->isThereAnyEntityAt(screen->getEntities(), (y + j) * SCREEN_WIDTH + x + i))
				return false;

	// no chocamos con ninguna entidad
	return true;
}

void DunDecorator::place_torchs(Screen* screen)
{
	int row;
	int col;

	// intentamos poner antorchas ARRIBA -----------------------------------------------

	// ponemos la de la izq.
	row = 1;
	col = SCREEN_WIDTH / 4;
	place_upperTorchs(screen, col, row);

	// ponemos la antorcha de la der.
	row = 1;
	col =  3 * (SCREEN_WIDTH / 4);
	place_upperTorchs(screen, col, row);

	// intentamos poner antorchas A LA IZQUIERDA ---------------------------------------------

	// ponemos la de la arriba
	row = SCREEN_HEIGHT / 4;
	col = 1;
	place_leftTorchs(screen, col, row);

	// ponemos la antorcha de la abajo
	row = 3 * (SCREEN_HEIGHT / 4);
	col = 1;
	place_leftTorchs(screen, col, row);

	// intentamos poner antorchas A LA DERECHA -----------------------------------------------

	// ponemos la de la arriba
	row = SCREEN_HEIGHT / 4;
	col = SCREEN_WIDTH - 2;
	place_rightTorchs(screen, col, row);

	// ponemos la antorcha de la abajo
	row = 3 * (SCREEN_HEIGHT / 4);
	col = SCREEN_WIDTH - 2;
	place_rightTorchs(screen, col, row);
}

bool DunDecorator::place_upperTorchs(Screen* screen, int col, int row)
{
	bool placed = false;
	// intentamos ponerla en las dos primeras filas
	while (!placed && row < 3)
	{
		if (!(placed = place_upperTorch(screen, col, row)))
			// si no lo conseguimos probamos una posici�n m�s a la izq.
			if (!(placed = place_upperTorch(screen, col -1, row)))
				// si no lo conseguimos probamos una m�s a la der.
				placed = place_upperTorch(screen, col +1, row);
		row++;
	}
	return placed;
}

bool DunDecorator::place_upperTorch(Screen* screen, int col, int row)
{
	if (screen->getSolid(col, row) == 1 && screen->getSolid(col, row + 1) != 1 
		&& screen->getSolid(col - 1, row) == 1 && screen->getSolid(col + 1, row) == 1)
	{
		Decoration* decoTorch = ((DungeonAutoTiler*) autoTiler)->getDungeonDeco(DunDecorationPos::top, info.wallId, Decoration::DecorationType::hangable);
		if (decoTorch != NULL)
		{
			// Inicializamos la decoraci�n
			decoTorch->init(col, row);
			// La a�adimos a la lista de decoraciones
			decorationList.push_back(decoTorch);
			return true;
		}
	}

	// si algo ha salido mal
	return false;
}

bool DunDecorator::place_leftTorchs(Screen* screen, int col, int row)
{
	bool placed = false;
	// intentamos ponerla en las dos primeras columnas
	while (!placed && col < 3)
	{
		if (!(placed = place_siderTorch(screen, col, row, DunDecorationPos::left)))
			// si no lo conseguimos probamos una posici�n m�s a la izq.
			if (!(placed = place_siderTorch(screen, col, row -1, DunDecorationPos::left)))
				// si no lo conseguimos probamos una m�s a la der.
				placed = place_siderTorch(screen, col, row +1, DunDecorationPos::left);
		col++;
	}
	return placed;
}

bool DunDecorator::place_rightTorchs(Screen* screen, int col, int row)
{
	bool placed = false;
	while (!placed && col > SCREEN_WIDTH - 4)
	{
		if (!(placed = place_siderTorch(screen, col, row, DunDecorationPos::right)))
			// si no lo conseguimos probamos una posici�n m�s a la izq.
			if (!(placed = place_siderTorch(screen, col, row -1, DunDecorationPos::right)))
				// si no lo conseguimos probamos una m�s a la der.
				placed = place_siderTorch(screen, col, row +1, DunDecorationPos::right);
		col--;
	}
	return placed;
}

bool DunDecorator::place_siderTorch(Screen* screen, int col, int row, DunDecorationPos pos)
{
	// comprobamos que tanto el tile de arriba como el de abajo sean paredes
	if (screen->getSolid(col, row) == 1	&& screen->getSolid(col, row - 1) == 1 && screen->getSolid(col, row + 1) == 1)
		// En funci�n de si es izq. o der. comprobamos que estamos en una pared y no dentro del muro
		if ((pos == DunDecorationPos::left && screen->getSolid(col + 1, row) != 1 && (!screen->isThereAnyEntityAt(screen->getEntities(), row * SCREEN_WIDTH + col+1)))
			 || (pos == DunDecorationPos::right && screen->getSolid(col - 1, row) != 1 && (!screen->isThereAnyEntityAt(screen->getEntities(), row * SCREEN_WIDTH + col-1))))
		{
			Decoration* decoTorch = ((DungeonAutoTiler*) autoTiler)->getDungeonDeco(pos, info.wallId, Decoration::DecorationType::hangable);
			if (decoTorch != NULL)
			{
				// Inicializamos la decoraci�n
				decoTorch->init(col, row);
				// La a�adimos a la lista de decoraciones
				decorationList.push_back(decoTorch);
				return true;
			}
		}

	// si algo ha salido mal
	return false;
}

void DunDecorator::decorateDunEntrance(Screen* screen, int col, int row)
{
	DunDecorationPos pos;
	int colFloor = col;		// columna donde ir� la decoraci�n de suelo de entrada
	int rowFloor = row;		// fila donde ir� la decoraci�n de suelo de entrada
	
	// miramos cu�l es la posici�n en la que hay que poner la entrada
	if (col == 0)
	{
		row--;
		pos = DunDecorationPos::left;
	}
	else if (col == SCREEN_WIDTH - 2)
	{
		row--;
		pos = DunDecorationPos::right;
	}
	else if (row == 0)
	{
		col--;
		pos = DunDecorationPos::top;
	}
	else if (row == SCREEN_HEIGHT - 2)
	{
		col--;
		pos = DunDecorationPos::bottom;
	}
	else // algo ha fallado
		return;

	// Cogemos una decoraci�n de entrada en funci�n de la posici�n que nos pasen
	Decoration* decoEntrance = ((DungeonAutoTiler*) autoTiler)->getDungeonDeco(pos, info.wallId, Decoration::DecorationType::dungeonEntrance);

	// Cogemos una decoraci�n de suelo entrada en funci�n de la posici�n que nos pasen
	Decoration* decoFloor = ((DungeonAutoTiler*) autoTiler)->getDungeonDeco(pos, info.wallId, Decoration::DecorationType::dunFloorEntrance);

	if (decoEntrance == NULL || decoFloor == NULL)
		return;

	// colocamos las decoraciones
	decoEntrance->init(col, row);
	decoFloor->init(colFloor, rowFloor);

	// las metemos en la lista (no comprobamos que choque...)
	decorationList.push_back(decoFloor);
	decorationList.push_back(decoEntrance);
}

void DunDecorator::decorateLS(Screen* screen){

// Terrenos y muros
	place_terrains(screen);

// Decoraciones 

	// Estatua Izquierda
	Decoration* decoLeft = autoTiler->getDecoration(11);
	// si no existen estatuas...
	if (decoLeft == NULL)
		return;
	// Inicializamos la decoraci�n
	decoLeft->init(3, 4);
	// Cambiamos el s�lido para que sea pasable por detr�s
	//screen->setSolid(3, 4, 0);
	// La a�adimos a la lista de decoraciones
	if (checkDecoCollision(decoLeft) && isInBounds(decoLeft, screen) && checkSolidCollision(decoLeft, screen))
		decorationList.push_back(decoLeft);


	// Estatua derecha
	Decoration* decoRight = autoTiler->getDecoration(12);
	// si no existen estatuas...
	if (decoRight == NULL)
		return;
	// Inicializamos la decoraci�n
	decoRight->init(10, 4);
	// Cambiamos el s�lido para que sea pasable por detr�s
	//screen->setSolid(10, 4, 0);
	// La a�adimos a la lista de decoraciones
	if (checkDecoCollision(decoRight) && isInBounds(decoRight, screen) && checkSolidCollision(decoRight, screen))
		decorationList.push_back(decoRight);


	// Metemos el port�n
	Decoration* decoDoor = autoTiler->getDecoration(10);
	// si no existen estatuas...
	if (decoDoor == NULL)
		return;
	// Inicializamos la decoraci�n
	decoDoor->init(6, 0);
	// Cambiamos el s�lido para que sea pasable por detr�s
	//screen->setSolid(6, 0, 0);
	// La a�adimos a la lista de decoraciones
	if (checkDecoCollision(decoDoor) && isInBounds(decoDoor, screen) && checkSolidCollision(decoDoor, screen))
		decorationList.push_back(decoDoor);
	
	
	// Recorremos la lista de decoraciones convierti�ndolas en entidades (guard�ndolas en la screen)
	list<Decoration*>::iterator it;
	for (it = decorationList.begin(); it != decorationList.end(); it++)
		if (*it != NULL)
			screen->addEntity((*it)->toEntities());

	// Borramos la lista de decoraciones
	clearDecorations();
}

void DunDecorator::decorateFS(Screen* screen){/*
	screen->setIdTileset(idTileset);

	// Terrenos y muros
		place_terrains(screen);

	// Decoraciones 

	// Estatua Arriba-Izquierda
	Decoration* decoLeft = autoTiler->getDecoration(24);
	// si no existen estatuas...
	if (decoLeft == NULL)
		return;
	// Inicializamos la decoraci�n
	decoLeft->init(3, 4);
	// Cambiamos el s�lido para que sea pasable por detr�s
	//screen->setSolid(3, 4, 0);
	// La a�adimos a la lista de decoraciones
	if (checkDecoCollision(decoLeft) && isInBounds(decoLeft, screen) && checkSolidCollision(decoLeft, screen))
		decorationList.push_back(decoLeft);*/






	// Recorremos la lista de decoraciones convierti�ndolas en entidades (guard�ndolas en la screen)
	/*list<Decoration*>::iterator it;
	for (it = decorationList.begin(); it != decorationList.end(); it++)
		if (*it != NULL)
			screen->addEntity((*it)->toEntities());

	// Borramos la lista de decoraciones
	clearDecorations();*/
}