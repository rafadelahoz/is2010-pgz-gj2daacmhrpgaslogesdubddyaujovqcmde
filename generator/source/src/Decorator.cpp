#include "Decorator.h"

Decorator::Decorator(DBManager* db)
{
	this->db = db;
	zone = "";
	changedZone = true;
	autoTiler = NULL;
	terrainIdMatrix = NULL;
}

Decorator::~Decorator()
{
	if (autoTiler != NULL) delete autoTiler;

	clearDecorations();
}

void Decorator::init(string zone, short tileSetId)
{
	if (this->zone != zone)
		changedZone = true;

	this->zone = zone;
	idTileset = tileSetId;
}

void Decorator::terrainsToTiles(Screen* screen)
{
	// Ahora se guarda en screen
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			autoTiler->getTerrain(terrainIdMatrix[i][j])->toTiles(terrainIdMatrix, screen, SCREEN_WIDTH, SCREEN_HEIGHT, i, j);
		}
	}
}

void Decorator::clearDecorations()
{
	// borramos las decoraciones que no hayan sido borradas
	list<Decoration*>::iterator it;
	for (it = decorationList.begin(); it != decorationList.end(); it++){
		if (*it != NULL) delete (*it);
	}

	decorationList.clear();
}

void Decorator::clearTerrains()
{
	if (terrainIdMatrix == NULL) return;

	for (int i = 0; i < SCREEN_WIDTH; i++)
		if (terrainIdMatrix[i] != NULL) delete [] terrainIdMatrix[i];

	delete [] terrainIdMatrix;
}

short Decorator::gimmeTile() {
	// To be implemented
	return 0;
}

short Decorator::gimmeFloorButton() {
	return 0;
}

int Decorator::getFreeSpace(Screen* s)
{
	int freespace = 0;
	// recorremos la pantalla buscando huecos libres
	for (int i = 0; i < SCREEN_WIDTH; i++)
		for (int j = 0; j < SCREEN_HEIGHT; j++)
			if (s->getSolid(i, j) == 0) freespace += 1;

	// quitamos los espacios correspondientes a decoraciones
	std::list<Decoration*>::iterator it;
	for(it = decorationList.begin(); it != decorationList.end(); it++)
		freespace -= (*it)->getDecorationData().width * (*it)->getDecorationData().height;

	return freespace;	// devolvemos el n�mero de huecos libres
}

int Decorator::place_symmetrics(Screen* s, int terrainId)
{
	// Buscamos una posici�n libre
	vector<int>* posUsed = new vector<int>; // Vector de posiciones utilizadas
	int pos;

	// Decoraciones sim�tricas por columnas
	pos = s->getFreePos(posUsed);
	delete posUsed; posUsed = NULL;
	if (pos != -1){
		// PLANTILLA POR COLUMNAS *********************************************************************
		int x = pos % SCREEN_WIDTH;
		int y = pos / SCREEN_WIDTH;

		// buscamos la segunda columna libre
		int i = 2;
		while (s->getSolid((x + i) % SCREEN_WIDTH, y) != 0 && i < 9)
			i++;
		
		// si no hemos encontrado columna libre salimos
		if (i == 6)
			return getFreeSpace(s);

		// posici�n de la segunda columna de decoraciones
		int x2 = (x + i) % SCREEN_WIDTH;

		// solicitamos una decoraci�n peque�a
		Decoration* d = autoTiler->getDecoration(Decoration::symmetric, Decoration::small, terrainId);

		// colocamos la primera fila ------------------------------------------------------
		if (d != NULL)
			d->init(x, y);
		else
			return getFreeSpace(s);	// si no la podemos colocar cerramos

		Decoration* d2 = autoTiler->getDecoration(d->getDecorationData().idDeco);
		if (d2 != NULL)
			d2->init(x2, y);
		else
			return getFreeSpace(s);	// si no la podemos colocar cerramos

		// miramos si colisiona alguna decoraci�n, no las ponemos
		if (checkDecoCollision(d) && isInBounds(d, s) && checkSolidCollision(d, s) &&
			checkDecoCollision(d2) && isInBounds(d2, s) && checkSolidCollision(d2, s))
				decorationList.push_back(d), decorationList.push_back(d2);
		else
			return getFreeSpace(s);

		// colocamos la segunda fila -------------------------------------------------------
		Decoration* d3 = autoTiler->getDecoration(d->getDecorationData().idDeco);
		if (d3 != NULL)
			d3->init(x, y + d3->getDecorationData().height + 1);
		else
			return getFreeSpace(s);	// si no la podemos colocar cerramos

		Decoration* d4 = autoTiler->getDecoration(d->getDecorationData().idDeco);
		if (d4 != NULL)
			d4->init(x2, y + d4->getDecorationData().height + 1);
		else
			return getFreeSpace(s);	// si no la podemos colocar cerramos

		// miramos si colisiona alguna decoraci�n, no las ponemos
		if (checkDecoCollision(d3) && isInBounds(d3, s) && checkSolidCollision(d3, s) &&
			checkDecoCollision(d4) && isInBounds(d4, s) && checkSolidCollision(d4, s))
				decorationList.push_back(d3), decorationList.push_back(d4);
		else
			return getFreeSpace(s);

		// colocamos la tercera fila -------------------------------------------------------
		Decoration* d5 = autoTiler->getDecoration(d->getDecorationData().idDeco);
		if (d5 != NULL)
			d5->init(x, y + d5->getDecorationData().height + 1);
		else
			return getFreeSpace(s);	// si no la podemos colocar cerramos

		Decoration* d6 = autoTiler->getDecoration(d->getDecorationData().idDeco);
		if (d6 != NULL)
			d6->init(x2, y + d6->getDecorationData().height + 1);
		else
			return getFreeSpace(s);	// si no la podemos colocar cerramos

		// miramos si colisiona alguna decoraci�n, no las ponemos
		if (checkDecoCollision(d5) && isInBounds(d5, s) && checkSolidCollision(d5, s) &&
			checkDecoCollision(d6) && isInBounds(d6, s) && checkSolidCollision(d6, s))
				decorationList.push_back(d5), decorationList.push_back(d6);
		else
			return getFreeSpace(s);

	}
	return getFreeSpace(s);
}

int Decorator::getDecoSolidBase(Decoration* d)
{
	Decoration::DecorationData data = d->getDecorationData();
	for (int i = 0; i < data.width; i++)
		for (int j = 0; j < data.height; j++)
			if (data.tileTypes.at(j*data.width + i) == 1) // base s�lida encontrada
				return i;

	// si no encontramos base s�lida
	return -1;

}

bool Decorator::isInBounds(Decoration* d, Screen* s)
{
	// comprobamos que no se salga del cuadrado que forma la pantalla - 1
	if (d->x + d->getDecorationData().width > (SCREEN_WIDTH - 1) || d->y + d->getDecorationData().height > (SCREEN_HEIGHT - 1) || d->x < 1 || d->y < 1)
		return false;
	else
		return true;
}

bool Decorator::checkDecoCollision(Decoration* d)
{
	// variables auxiliares para trabajar m�s c�modamente
	int itx, ity, itw, ith;
	Decoration::DecorationData data;
	data = d->getDecorationData();
	int dw = data.width;	// ancho de la decoraci�n con la que se comprueba
	int dh = data.height;	// alto de la decoraci�n con la que se comprueba
	int dx = d->x;	
	int dy = d->y;

	// recorremos la lista de decoraciones para ver si colisiona con alguna
	list<Decoration*>::iterator it;
	for (it = decorationList.begin(); it != decorationList.end(); it++)
		if (*it != d) // si no es la propia decoraci�n (que podr�a estar dentro de la lista)
		{
			// guardamos los valores de la decoraci�n de la lista para trabajar m�s c�modamente
			data = (*it)->getDecorationData();
			itx = (*it)->x;
			ity = (*it)->y;
			itw = data.width;
			ith = data.height;
			
			// comprobamos colisi�n entre los cuadrados de las decoraciones
			if (dx + dw <= itx || dy + dh <= ity)
				continue;	// no colisionan y seguimos evaluando con otras decoraciones

			if (dx >= itx + itw || dy >= ity + ith)
				continue;	// no colisionan y seguimos evaluando con otras decoraciones

			// ha habido colisi�n (miramos si colisiona con la fila de la base)
			int row = getDecoSolidBase(d);
			for (int j = 0; j < ith; j++)
				if (ity + j == dy + row)
					return false;
		}
	// hemos recorrido toda la lista sin ninguna colisi�n
	return true;
}

bool Decorator::checkSolidCollision(Decoration* d, Screen* s){
	
	Decoration::DecorationData data = d->getDecorationData();
	int w = data.width;
	int h = data.height;

	// Recocrremos la decoraci�n
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			// Si en la posici�n de la decoraci�n hay algo que no sea libre (y es un tile de base s�lido) devolvemos false
			if (s->getSolid(d->x + i, d->y + j) != 0 && data.tileTypes.at(j*w + i) != 0)
				return false;
	return true;
}

// vale s�lo para decoraciones con una fila de base
bool Decorator::checkBlockingPath(Decoration* d, Screen* s)
{
	Decoration::DecorationData data = d->getDecorationData();
	int w = data.width;
	int h = data.height;
	int x = d->x;
	int y = d->y;

	int solid = -1; // primer s�lido que hemos encontrado
	bool firstSolid = false;
	bool firstGap = false;
	short screenSolid;

	// miramos que no bloquea caminos VERTICALES ----------------------------------------------------------
	for (int j = 0; j < h; j++)
	{
		// buscamos en la fila de la decoraci�n por la que vamos si existe alg�n tile s�lido
		for (int i = 0; i < w; i++)
			if (data.tileTypes.at(j*w + i) == 1)	// si es tile s�lido
			{
				solid = j;
				break;
			}
		
		if (solid != -1) // si hemos encontrado s�lidos en esta fila
		{
			firstSolid = false;
			firstGap = false;

			// comprobamos que no bloqueen caminos verticales
			for (int n = -1; n < 2; n++) // miramos en la fila de arriba, en la misma fila, y en la de abajo
				for (int k = x + solid - 1; k < x + w + 1; k++)
				{
					screenSolid = s->getSolid(k, j + n);
					firstSolid = firstSolid || (screenSolid != 0 && screenSolid != 3);	// hemos encontrado al menos un s�lido
					firstGap = firstSolid && (firstGap || screenSolid == 0 || screenSolid == 3);	// hemos encontrado al menos un hueco
					if (firstSolid && firstGap && (screenSolid != 0 && screenSolid != 3))	// si adem�s hemos vuelto a encontrar un s�lido
						return false;	// estamos bloqueando un camino
				}
			solid = -1;
		}
	}

	// miramos que no bloquea caminos HORIZONTALES ----------------------------------------------------------
	for (int i = 0; i < w; i++)
	{
		// buscamos en la columna de la decoraci�n por la que vamos si existe alg�n tile s�lido
		for (int j = 0; j < h; j++)
			if (data.tileTypes.at(j*w + i) == 1) // si es tile s�lido
			{
				solid = i;
				break;
			}
		
		if (solid != -1) // si hemos encontrado s�lidos en esta columna
		{
			firstSolid = false;
			firstGap = false;

			// comprobamos que no bloqueen caminos horizontales
			for (int n = -1; n < 2; n++) // miramos en la columna de la izqda, en la misma columna, y en la columna de la derecha
				for (int k = y + solid - 1; k < y + h + 1; k++)
				{
					screenSolid = s->getSolid(i + n, k);
					firstSolid = firstSolid || (screenSolid != 0 && screenSolid != 3);	// hemos encontrado al menos un s�lido
					firstGap = firstSolid && (firstGap || screenSolid == 0 || screenSolid == 3);	// hemos encontrado al menos un hueco
					if (firstSolid && firstGap && (screenSolid != 0 && screenSolid != 3))	// si adem�s hemos vuelto a encontrar un s�lido
						return false;	// estamos bloqueando un camino
				}
			solid = -1;
		}
	}
	// si todo ha salido bien
	return true;
}

bool Decorator::checkEntitiesCollision(Decoration* d, Screen* s){

	Decoration::DecorationData data = d->getDecorationData();
	int w = data.width;
	int h = data.height;

	// Recocrremos la decoraci�n
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			// Si en la posici�n de la decoraci�n (parte s�lida) hay algo que no sea libre devolvemos false
			if (s->isThereAnyEntityAt(s->getEntities(), (d->y + j) * SCREEN_WIDTH + d->x + i) && data.tileTypes.at(j*w + i) == 1)
				return false;
	return true;
}