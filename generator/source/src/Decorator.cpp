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

	clearDecorations();
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
				terrainIdMatrix[i][j] = waterId;//, screen->setSolid(i, j, 1);
			else if (type == 3)
				terrainIdMatrix[i][j] = pathId, screen->setSolid(i, j, 0);
			else if (type == 4)
				terrainIdMatrix[i][j] = solidId, screen->setSolid(i, j, 1);
			else
				terrainIdMatrix[i][j] = solidId, screen->setSolid(i, j, 1);
		}
	}

	// FALTA COMPROBAR QUE LAS ENTIDADES QUEPAN Y TOENTITIES!!!!!!!!!!!!!!!!*************************
/*
	// Colocamos decoraciones
	Decoration* decoPath = autoTiler->getDecoration(Decoration::solid, Decoration::small, pathId); 
	Decoration* decoFloor = autoTiler->getDecoration(Decoration::solid, Decoration::small, floorId); 
	Decoration* decoMedium = autoTiler->getDecoration(Decoration::solid, Decoration::medium, floorId);
	Decoration* decoBig = autoTiler->getDecoration(Decoration::solid, Decoration::big, pathId);

	// Buscamos una posici�n libre
	vector<int>* posUsed = new vector<int>; // Vector de posiciones utilizadas
	int pos;

	// Deco1
	pos = screen->getFreePos(posUsed);
	if (pos != -1){
		// Ponemos alguna decoracion en la posici�n pos
		posUsed->push_back(pos);
		// Inicializamos las decoraciones
		decoPath->init(pos % SCREEN_WIDTH*16, pos / SCREEN_WIDTH*16);
	}

	// Deco2
	pos = screen->getFreePos(posUsed);
	if (pos != -1){
		// Ponemos alguna decoracion en la posici�n pos
		posUsed->push_back(pos);
		// Inicializamos las decoraciones
		decoFloor->init(pos % SCREEN_WIDTH*16, pos / SCREEN_WIDTH*16);
	}

	// Deco3
	pos = screen->getFreePos(posUsed);
	if (pos != -1){
		// Ponemos alguna decoracion en la posici�n pos
		posUsed->push_back(pos);
		// Inicializamos las decoraciones
		decoMedium->init(pos % SCREEN_WIDTH*16, pos / SCREEN_WIDTH*16);
	}

	// Deco4
	pos = screen->getFreePos(posUsed);
	if (pos != -1){
		// Ponemos alguna decoracion en la posici�n pos
		posUsed->push_back(pos);
		// Inicializamos las decoraciones
		decoBig->init(pos % SCREEN_WIDTH*16, pos / SCREEN_WIDTH*16);
	}

	// Las guardamos en la lista de decoraciones
	decorationList.push_back(decoPath);
	decorationList.push_back(decoFloor);
	decorationList.push_back(decoMedium);
	decorationList.push_back(decoBig);

	// Recorremos la lista de decoraciones convierti�ndolas en entidades (guard�ndolas en la screen)
	list<Decoration*>::iterator it;
	std::vector<Entity*> ents;
	for (it = decorationList.begin(); it != decorationList.end(); it++)
		if (*it != NULL){
			ents = (*it)->toEntities();
			for (int i = 0; i < ents.size(); i++)
				// A�adimos las entidades a la screen
				screen->addEntity(ents[i]);
		}

	// Borramos el vector de posiciones
	delete posUsed;
	posUsed = NULL;
	*/
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

void Decorator::clearDecorations()
{
	// borramos las decoraciones que no hayan sido borradas
	list<Decoration*>::iterator it;
	for (it = decorationList.begin(); it != decorationList.end(); it++){
		if (*it != NULL) delete (*it);
	}

	decorationList.clear();
}

short Decorator::gimmeTile() {
	// To be implemented
	return 0;
}

short Decorator::gimmeFloorButton() {
	return 0;
}

bool Decorator::checkDecoCollision(Decoration* d)
{/*
 list<Decoration*>::iterator it;
 for (it = decorationList.begin(); it != decorationList.end(); it++)
	if (*it != d){ // si no es la propia decoraci�n (que podr�a estar dentro de la lista)
		if ((*it)->x == d->x || (*it)->y == d->y)
			return false; // si est�n en la misma posici�n no se pueden colocar

   // comprobamos colisi�n entre los cuadrados de las decoraciones
   // no me da tiempo a hacerlo
  }*/
	return true;
}