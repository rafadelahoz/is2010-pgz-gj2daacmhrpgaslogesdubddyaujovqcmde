#include "GenVoroWorld.h"

GenVoroWorld::GenVoroWorld(Overworld* overworld, vector<GenZone*>* genZones, DBManager* mydb) : GenOverworld(overworld, genZones, mydb)
{
	blockadeVerts = new vector<set<GPoint> >();
	mainRoadVerts = new vector<GPoint>();
	interestingPoints = new vector<GPoint>();
}

GenVoroWorld::~GenVoroWorld()
{
	delete blockadeVerts;
	blockadeVerts = NULL;
	
	delete mainRoadVerts;
	mainRoadVerts = NULL;

	delete interestingPoints;
	interestingPoints = NULL;
}

void GenVoroWorld::genFrontiers(){
	//cout << "Ejecutando funcion <GenOverworld::genFrontiers()>" << endl;
	float x1,y1,x2,y2;
	GLine l;
	GenVoronoi vdg;

	ptList = genPoints(overworld->getNumZones(), overworld->getWorldSizeH(), overworld->getWorldSizeW(), 
				overworld->getNumZones());

	//voronoi
	float* xPts = getPoints(ptList, 0);
	float* yPts = getPoints(ptList, 1);
	vdg.generateVoronoi(xPts, yPts, ptList.size(), 
		0, overworld->getWorldSizeW(), 0, overworld->getWorldSizeH(), 1, false);
	delete xPts;
	delete yPts;
	vdg.resetIterator();
	while(vdg.getNext(x1,y1,x2,y2)){
		l.a.x = x1;
		l.a.y = y1;
		l.b.x = x2;
		l.b.y = y2;
		// A�adimos la frontera 
		voronoiPoly.addLine(l);
	}
	// Los bordes tambien son fronteras 
	for (int  i = 0; i< overworld->getWorldSizeW(); i++){
		overworld->getMapTile(i, 0)->setZoneNumber(0);
		overworld->getMapTile(i, overworld->getWorldSizeH()-1)->setZoneNumber(0);
	}
	for (int  i = 0; i< overworld->getWorldSizeH(); i++){
		overworld->getMapTile(0, i)->setZoneNumber(0);
		overworld->getMapTile(overworld->getWorldSizeW()-1, i)->setZoneNumber(0);
	}
}

void GenVoroWorld::genShape(){
	//cout << "Ejecutando funcion <GenOverworld::genShape()>" << endl;
	Horses h;
	h.placeHorses(voronoiPoly, 50);
	h.run();
	vector<GPoint> bresenPoints;
	for(int i=0; i<h.ropes.size(); i++){
		// !!!!!!!!!!! Aqu� a veces se sale de rango de la matriz. Revisar y quitar comments !!!!!!!!!!!!!! FIXME
		//cout<< "Tama�o con caballos: "<<ropes.size()<< endl;
		bresenPoints = getMatrixLine(h.ropes[i].a.x, h.ropes[i].a.y, h.ropes[i].b.x, h.ropes[i].b.y);
		for (int j=0; j < bresenPoints.size(); j++)
			overworld->getMapTile(bresenPoints[j].x, bresenPoints[j].y)->setZoneNumber(0);
	}
}

void GenVoroWorld::assignTilesAndScreens(){
	//cout << "Ejecutando funcion <GenOverworld::assignTilesScreens()>" << endl;
	for ( unsigned int i = 0; i<overworld->getNumZones(); i++)
		floodFillScanlineStack(ptList[i].x, ptList[i].y, i+1);

	int screensPerCol = overworld->getWorldSizeH() / SCREEN_HEIGHT;
	int screensPerRow = overworld->getWorldSizeW() / SCREEN_WIDTH;

	int screenNumber = 0;
    int iniTileRow;
    int iniTile;
	int zoneNum;

    for (int row = 0; row < screensPerCol; row++){
        iniTileRow = SCREEN_WIDTH*SCREEN_HEIGHT*screenNumber;
        for (int col = 0; col < screensPerRow; col++){
            iniTile = col*SCREEN_WIDTH + iniTileRow;
			OwScreen* screen = makeNewScreen(iniTile,screenNumber);  //creamos el screen
			zoneNum = screen->getZoneNum();  //n�mero de zona a la que pertenece la screen
			overworld->addScreen(screen);  //a�adimos la referencia a mundo
			genZones->at(zoneNum-1)->addScreen(screen);  //a�adimos la referencia al generador de Zona
			screenNumber++;
        }
    }

	int erased = 0;
	//Borrado de zonas vacias
	for (int i = 0; i < genZones->size(); i++){
		if ( genZones->at(i)->getNumScreens() == 0 ){
			cout << "ZONA" << genZones->at(i)->getZoneNumber() << " VAC�A!!!" << endl;
			genZones->erase(genZones->begin() + i - erased);
			erased++;
		}		
	}
	//cout << "------> DONE! <-------" << endl;
}

OwScreen* GenVoroWorld::makeNewScreen(int iniT, int screenNumber){
	int screensPerRow = overworld->getWorldSizeW() / SCREEN_WIDTH;
	int iniTile;
	MapTile* t;
	int zoneNum; //n�mero de zona
	int* candidates = new int[overworld->getNumZones()];
	for (int i=0; i<overworld->getNumZones(); i++) {
		candidates[i] = 0;
	}

	vector<MapTile*>* screenMatrix = new vector<MapTile*>();  //es una matriz que tiene punteros a tiles.

	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		iniTile = iniT + SCREEN_WIDTH*screensPerRow*i;
		for (int j = 0; j < SCREEN_WIDTH; j++){
			t = overworld->mapTileMatrix->at(iniTile + j); // pillamos el mapTile que toque. Cogemos el tile.
			zoneNum = checkTileinZone(t); // este nos dice en que zona esta el mapTile(tile).
			if (zoneNum > 0){
				t->setZoneNumber(zoneNum); // se lo ponemos al mapTile
				candidates[zoneNum-1]++; // incrementamos el numero de tiles de una zona dentro de una pantalla
			}
			screenMatrix->push_back(overworld->mapTileMatrix->at(iniTile + j));  //vamos a�adiendo tiles a la pantalla.
		}
	}

	int maxNumber = 0;
	zoneNum = 0;
	for ( int i = 0; i<overworld->getNumZones(); i++){
		if (candidates[i] > maxNumber){  //La zona i es la que m�s candidatos tiene?
			maxNumber = candidates[i];
			zoneNum = i;	//en zoneNum se guarda el n�mero de zona a la que pertenece.
		}
	}
	zoneNum ++; //sumamos uno porque las zonas empiezan por el n�mero 1 y no por 0

	delete candidates;
	candidates = NULL;

	short posX = screenNumber % screensPerRow;
	short posY = screenNumber / screensPerRow;
	
	//Si, mega-llamada porque necesita muchas cosas para poder hacer el guardado. El primer argumento '0' es el mapNumber. Que pertenece al OW inicial.
	return new OwScreen(0, screenNumber, screenMatrix, zoneNum, posX, posY, genZones->at(zoneNum-1)->getNumEnemies(), genZones->at(zoneNum-1)->getZone(), genZones->at(zoneNum-1)->getTheme(), myDB);
}

//Devuelve el n�mero de la zona en el q est� el tile
int GenVoroWorld::checkTileinZone(MapTile* mTile){
	if(mTile->getZoneNumber() == -1) return 1;
	return mTile->getZoneNumber();
	//return 0;
}

void GenVoroWorld::genGeoDetail(){
	//cout << "Ejecutando funcion <GenOverworld::genGeoDetail()>" << endl;

	for (unsigned int i = 0; i<genZones->size(); i++){
		GenZone* genZone = genZones->at(i);
		genZone->genGeoDetail( overworld->getWorldSizeW() / SCREEN_WIDTH);
	}
	filterTiles();
	//cout << "------> DONE! <-------" << endl;
}

//Se encarga de que no aparezcan tiles sueltos rodeados de s�lidos y adem�s repasa que las fronteras est�n bien.
void GenVoroWorld::filterTiles()
{
	short up, down, left, right;
	for (int i = 0; i < overworld->getWorldSizeW(); i++)
	{
		for (int j = 0; j < overworld->getWorldSizeH(); j++)
		{
			if (j == 0 || i == 0 || j == (overworld->getWorldSizeH()-1) || i == (overworld->getWorldSizeW()-1)) //Los bordes del mapa a solidos
				overworld->getMapTile(i,j)->setSolid(1);
			else
			{
				up = overworld->getMapTile(i,j-1)->getSolid();
				down = overworld->getMapTile(i,j+1)->getSolid();
				right = overworld->getMapTile(i+1,j)->getSolid();
				left = overworld->getMapTile(i-1,j)->getSolid();
				if( up == 1 && down == 1 && right == 1 && left == 1)
					overworld->getMapTile(i,j)->setSolid(1);
			}
		}
	}

	OwScreen* screen;  //pantalla actual
	OwScreen* rightScreen;  //pantalla a la derecha de la actual
	OwScreen* downScreen; //pantalla debajo de la actual

	for (int k = 0; k < overworld->screenList->size()-1; k++) //-1 porque la �ltima pantalla no tiene sentido mirarla.
	{
		if ( k < ((overworld->getWorldSizeW()/SCREEN_WIDTH) * ((overworld->getWorldSizeH()/SCREEN_HEIGHT)-1))) //NO Estamos en la �ltima fila. Se puede revisar la de debajo
		{
			screen = overworld->screenList->at(k);
			downScreen = overworld->screenList->at(k+(overworld->getWorldSizeW()/SCREEN_WIDTH)); //cogemos la pantalla de justo debajo de nosotros
			for (int i = 0; i < SCREEN_WIDTH; i++) //Vamos a arreglar la frontera de abajo o.O!
			{
				if( (screen->getSolid(i,SCREEN_HEIGHT-1) == 1) || (downScreen->getSolid(i,0) == 1)) //si alguno de los dos son solidos...
				{
					screen->setSolid(i,SCREEN_HEIGHT-1, 1);
					downScreen->setSolid(i,0, 1);
				}
			}
		}
		if(!( k+1 % (overworld->getWorldSizeW()/SCREEN_WIDTH) == 0))// (k+1 % screensPerRow) Si no estamos en la columna de m�s a la derecha. Podemos comprobar frontera derecha ^^
		{
			screen = overworld->screenList->at(k);
			rightScreen = overworld->screenList->at(k+1);
			for (int i = 0; i < SCREEN_HEIGHT; i++) //Vamos a arreglar la frontera de la derecha o.O!
			{
				if( (screen->getSolid(SCREEN_WIDTH-1,i) == 1) || (rightScreen->getSolid(0,i) == 1)) //si alguno de los dos son solidos...
				{
					screen->setSolid(SCREEN_WIDTH-1,i, 1);
					rightScreen->setSolid(0,i, 1);
				}
			}
		}
	} //for de las fronteras entre pantallas

	MapTile* a, * b;	// Tiles de esta forma en las	a b
	MapTile* c, * d;	// esquinas de las pantallas	c d
	for (int i = 1; i < (overworld->getWorldSizeW()/SCREEN_WIDTH); i++)  // 1..ScreensPerRow-1
		for(int j = 1; j < (overworld->getWorldSizeH()/SCREEN_HEIGHT); j++)  // 1..ScreensPerColum-1
		{
			a = overworld->getMapTile((i*SCREEN_WIDTH)-1, (j*SCREEN_HEIGHT)-1);
			b = overworld->getMapTile((i*SCREEN_WIDTH), (j*SCREEN_HEIGHT)-1);
			c = overworld->getMapTile((i*SCREEN_WIDTH)-1, (j*SCREEN_HEIGHT));
			d = overworld->getMapTile((i*SCREEN_WIDTH), (j*SCREEN_HEIGHT));
			
			if(a->getSolid() == 1 || b->getSolid() == 1 || c->getSolid() == 1 || d->getSolid() == 1)
			{
				a->setSolid(1);
				b->setSolid(1);
				c->setSolid(1);
				d->setSolid(1);
			}
		}
}

void GenVoroWorld::genDecoration(DBManager* myDB)
{
	//cout << "Ejecutando funcion <GenOverworld::genDecoration()>" << endl;

	for (int i = 0; i<genZones->size(); i++){
		genZones->at(i)->genDetail();
	}
	//cout << "------> DONE! <-------" << endl;
}

void GenVoroWorld::placeDungeons(){
	//cout << "Ejecutando funcion <GenOverworld::placeDungeons()>" << endl;
	for (unsigned int i = 0; i< genZones->size();i++){
		
		GenZone* z = genZones->at(i);
		z->placeDungeon();
	}
}

void GenVoroWorld::placeSafeZones(){
	//cout << "Ejecutando funcion <GenOverworld::placeSafeZones()>" << endl;
}

void GenVoroWorld::genMainRoad(){
	
	int tilesPerRow = overworld->getWorldSizeW();

	vector<int>* choosed = new vector<int>();
	
	int actZoneIni = 0;
	int actZoneEnd = 0;
	GenZone* zIni = NULL;
	GenZone* zEnd = NULL;
	int iniTile = 0;
	int endTile = 0;

	for (int i = 0; i< genZones->size() - 1; i++){
		
		zIni = genZones->at(actZoneIni);
		iniTile = zIni->getDungEntranceTile();

		cout << "Zona inicial:" << zIni->getZoneNumber() << endl;
		
		choosed->push_back(actZoneIni);
		
		actZoneEnd = findNearestZone(actZoneIni, zIni, choosed);
		
		if (actZoneEnd != -1 ){
			zEnd = genZones->at(actZoneEnd);
			endTile = zEnd->getDungEntranceTile();

					int iniTileRow = iniTile / tilesPerRow;
					int endTileRow = endTile / tilesPerRow;
					int tile = iniTile;
					int row = iniTileRow;
					int lastRowTurned = row;

					MapTile* actTile;

					if( iniTile - endTile > 0 ){ //La otra entrada est� hacia arriba
						//Hacemos camino hacia arriba hasta llegar a la misma fila
						while ( row > endTileRow){
							actTile = overworld->mapTileMatrix->at(tile);
							if (actTile->getTileId() != 0)
								actTile->setTileId(666);
							if( (rand() % 3 == 0 ) && ( lastRowTurned - row > 3 ) && (row - endTileRow > 8)){ //Hacemos giro por ah�
								GPoint p;
								p.x = tile % overworld->getWorldSizeW();
								p.y = tile / overworld->getWorldSizeW();
								mainRoadVerts->push_back(p);
								drawLateralTurn(tile, row, true, endTileRow);
								lastRowTurned = row;
							}
							else{
								tile -= tilesPerRow;
								row--;
							}
						}
						row = endTileRow;
						tile = row*overworld->getWorldSizeW() + tile%overworld->getWorldSizeW();
					}
					else{ //La otra entrada est� por debajo
						//Hacemos camino hacia abajo hasta llegar a la misma fila
						while ( row < endTileRow){
							actTile = overworld->mapTileMatrix->at(tile);
							if (actTile->getTileId() != 0)
								actTile->setTileId(666);
							if( (rand() % 3 == 0 ) && (row - lastRowTurned > 3) && (endTileRow - row > 8) ){ //Hacemos giro por ah�
								drawLateralTurn(tile, row, false,  endTileRow);
								lastRowTurned = row;
							}
							else{
								tile += tilesPerRow;
								row++;
							}
						}
						row = endTileRow;
						tile = row*overworld->getWorldSizeW() + tile%overworld->getWorldSizeW();
					}

					int col = tile % tilesPerRow;
					int endCol = endTile % tilesPerRow;
					int lastColTurned = col;

					if ( col - endCol > 0 ){ //La otra entrada est� hacia la izquierda
						//Hacemos camino hacia la izquierda hasta llegar al mismo tile
						while (col > endCol){
							actTile = overworld->mapTileMatrix->at(tile);
							if ( actTile->getTileId() != 0 )
								actTile->setTileId(666);
							if ( (rand() % 3 == 0) && (lastColTurned  - col > 3) && (col - endCol > 8)){
								drawVerticalTurn(tile, col, false, endCol);
								lastColTurned = col;
							}
							else{
								tile --;
								col--;
							}
						}
					}
					else{ //La otra entrada est� hacia la derecha
						//Hacemos camino hacia la derecha hasta llegar al mismo tile
						while ( col < endCol){
							actTile = overworld->mapTileMatrix->at(tile);
							if ( actTile->getTileId() != 0 )
								actTile->setTileId(666);
							if ( (rand() % 3 == 0) && (col - lastColTurned > 3) && (endCol - col > 8)){
								drawVerticalTurn(tile, col, true, endCol);
								lastColTurned = col;
							}
							else{
								tile++;
								col++;
							}
						}
					}
					actZoneIni = actZoneEnd;
					actZoneEnd = -1;

		}	
	}

	delete choosed; choosed = NULL;
}

void GenVoroWorld::drawVerticalTurn(int& tile, int& col, bool right, int maxCol){
	string direction = ( rand()%2==0? "up":"down");

	int iniRow = tile/overworld->getWorldSizeW();
	int endRow = -1;
	int colsCovered = 0;
	int rowsCovered = 0;
	int iniCol = col;
	int endCol = -1;
	bool doubleLoop = (rand()%3 > 0? true:false);

	int totalRows;

	if (direction == "down"){
		endRow = iniRow + (rand()%5 + 4 );
		totalRows = endRow - iniRow;
	}
	else{
		endRow = iniRow - (rand()%5 + 4 );
		totalRows = iniRow - endRow;
	}

	for (int i = 0; i<totalRows; i++){
		if ( ! isFrontierNear(tile, 3) && 
			 ! isRoadNear(tile, 2 )){
			if (overworld->mapTileMatrix->at(tile)->getTileId() != 0)
				overworld->mapTileMatrix->at(tile)->setTileId(666);
			rowsCovered++;

			direction=="down"? tile += overworld->getWorldSizeW() : tile -= overworld->getWorldSizeW();
		}
	}

	if ( right ){ //Tenemos que ir a la derecha
		endCol = iniCol + (rand()%6 + 4 );
		for (int i = 0; i< endCol - iniCol; i++){
			if ( col + 2 < maxCol &&
				 overworld->mapTileMatrix->at(tile)->getTileId() != 0 &&
				!isFrontierNear(tile, 3) &&
				!isRoadNear(tile,2)){
					overworld->mapTileMatrix->at(tile)->setTileId(666);
					col++;
					tile++;
					if (rand()%4==0 && doubleLoop && i>1 && i< endCol-iniCol-2){
						if ( col + 6 < maxCol)
							drawVerticalTurn(tile,col,right,maxCol);

						doubleLoop = false;
					}
			}
		}
	}
	else{ //Tenemos que ir a la izquierda
		endCol = iniCol - (rand()%6 + 4 );
		for (int i = 0; i<iniCol-endCol; i++){
			if ( col - 2 > maxCol &&
				 overworld->mapTileMatrix->at(tile)->getTileId() != 0 &&
				!isFrontierNear(tile, 3) &&
				!isRoadNear(tile,2)){
					overworld->mapTileMatrix->at(tile)->setTileId(666);
					col--;
					tile--;
					if (rand()%4==0 && doubleLoop && i>1 && i< endCol-iniCol-2){
						if ( col - 6 > maxCol)
							drawVerticalTurn(tile,col,right,maxCol);

						doubleLoop = false;
					}

			}

		}
	}

	for (int i = 0 ; i<rowsCovered; i++){
		if (overworld->mapTileMatrix->at(tile)->getTileId() != 0)
				overworld->mapTileMatrix->at(tile)->setTileId(666);
		
		direction=="down"? tile -= overworld->getWorldSizeW() : tile += overworld->getWorldSizeW();
	}
}

void GenVoroWorld::drawLateralTurn(int& tile, int& row, bool up, int maxEndRow){
	
	string direction = ( (rand() % 2) == 0) ? "right" : "left"; 

	int iniCol = tile % overworld->getWorldSizeW();
	int endCol = -1;
	int colsCovered = 0;
	int rowsCovered = 0;
	int iniRow = row;
	int endRow = -1;
	bool doubleLoop = (((rand()%3) > 0)? true:false);
	
		
		int totalCols;

		if (direction == "right"){
			endCol = iniCol  + (rand()%5 + 4 );
			totalCols = endCol - iniCol;
		}
		else{
			endCol = iniCol  - (rand()%5 + 4 );
			totalCols = iniCol - endCol;
		}

		for (int i = 0; i<totalCols; i++){
			if ( ((tile + 3) / overworld->getWorldSizeW() == iniRow || 
				  (tile - 3) / overworld->getWorldSizeW() == iniRow   ) && 
				 !isFrontierNear(tile, 3) &&
				 !isRoadNear(tile,2)){
				if (overworld->mapTileMatrix->at(tile)->getTileId() != 0)
					overworld->mapTileMatrix->at(tile)->setTileId(666);
				colsCovered++;
				
				(direction=="right"? tile++ : tile--);
			}
		}

		if ( up ){ //Tenemos que subir filas
			endRow = iniRow - (rand()%6+4);
			for (int i = 0; i < (iniRow-endRow); i++){
				if (row - 2 > maxEndRow &&
					overworld->mapTileMatrix->at(tile)->getTileId() != 0 &&
					!isFrontierNear(tile, 3) &&
					!isRoadNear(tile,2)){
						overworld->mapTileMatrix->at(tile)->setTileId(666);
						row--;
						tile -= overworld->getWorldSizeW();
						if (doubleLoop && i>1 && i<iniRow-endRow-2 && rand()%4 == 0){
							if (row - 6 > maxEndRow)
								 drawLateralTurn(tile,row,up,maxEndRow);
							
							doubleLoop = false;
						}	
				}
			}
		}
		else{ //Tenemos que bajar filas
			endRow = iniRow + (rand()%6+4);
			for (int i = 0; i< (endRow - iniRow); i++){
				if (row + 2 < maxEndRow &&
					overworld->mapTileMatrix->at(tile)->getTileId() != 0 &&
					!isFrontierNear(tile, 3) &&
				    !isRoadNear(tile,2)){
						overworld->mapTileMatrix->at(tile)->setTileId(666);
						row++;
						tile += overworld->getWorldSizeW();
						if(doubleLoop && i>1 && i<endRow-iniRow-2&& rand()%4 == 0){
							if (row + 6 < maxEndRow)
								drawLateralTurn(tile,row,up,maxEndRow);
							
							doubleLoop = false;
						}
				}
			}
		}

		for (int i = 0; i<colsCovered; i++){ //Volvemos hacia la izquierda tantos como hemso ido a la drecha
			if (overworld->mapTileMatrix->at(tile)->getTileId() != 0)
					overworld->mapTileMatrix->at(tile)->setTileId(666);
			(direction=="right"? tile-- : tile++);
		}
}

int GenVoroWorld::findNearestZone(int actZone, GenZone* zIni, vector<int>* choosed){
	
	int tilesPerRow = overworld->getWorldSizeW();

	bool alreadyChoosed = false;
	int minDistance = 2147483647;
	int minDistanceZone = -1;
	int iniTile = zIni->getDungEntranceTile();
	int endTile = -1;

	GenZone* zEnd;
	for (int i = 0; i < genZones->size(); i++){
		if ( !contains(i,choosed) ){
			zEnd = genZones->at(i);
			endTile = zEnd->getDungEntranceTile();
			int tilesHeight = abs( (iniTile / tilesPerRow) - (endTile / tilesPerRow) );
			int tilesWidth = abs( (iniTile % tilesPerRow) - (endTile % tilesPerRow) );
			int absDistance = tilesHeight + tilesWidth;
			if (absDistance < minDistance){
				minDistance = absDistance;
				minDistanceZone = i;
			}
		}
	}
	return minDistanceZone;
}

bool GenVoroWorld::isRoadNear(int iniT, int range){
	int iniTile = iniT - range - (range*overworld->getWorldSizeW());

	int roadsFound = 0;
	int tile;
	for (int i = 0; i < range*2+1; i++){
		tile = iniTile + i*overworld->getWorldSizeW();
		for (int j = 0; j < range*2+1; j++){
			if ( roadsFound <= range+2 &&  overworld->mapTileMatrix->at(tile)->getTileId() == 666)
				roadsFound++;
			tile++;
		}	
	}

	if (roadsFound > (range+2))
		return true;
	else
		return false;
}

bool GenVoroWorld::isRoadInDirection(int iniT, int range, int direction){
	

	int tile = iniT;
	bool roadFound = false;
	for (int i = 0; i<range; i++){
		if (!roadFound){
			switch (direction){
				case 1: // Derecha
					tile++;
					break;
				case 2: // Abajo
					tile += overworld->getWorldSizeW();
					break;
				case 3: // Izquierda
					tile--;
					break;
				case 4: // Arriba
					tile -= overworld->getWorldSizeW();
					break;
				default :
					break;
			}
			if (overworld->mapTileMatrix->at(tile)->getTileId() == 666 )
				roadFound = true;
		}
	}
	return roadFound;
}

bool GenVoroWorld::isFrontierNear(int iniT, int range){
	int iniTile = iniT - range - (range*overworld->getWorldSizeW());
	if (iniTile < 0) 
		return true;

	bool frontierFound = false;
	int tile = 0;
	for (int i = 0; i < (range*2+1); i++){
		tile = iniTile + i*overworld->getWorldSizeW();
		for (int j = 0; j < (range*2+1); j++){
			if ( !frontierFound) 
				if (tile >= overworld->mapTileMatrix->size() || overworld->mapTileMatrix->at(tile)->getZoneNumber() == 0 )
					frontierFound = true;
			tile++;
		}	
	}

	if (frontierFound)
		return true;
	else
		return false;
}


bool GenVoroWorld::contains(int elem, vector<int>* collect){
	for (int i = 0; i < collect->size(); i++)
		if (collect->at(i) == elem)
			return true;

	return false;
}

void GenVoroWorld::genRoadRamifications(){
	// Debug -------------
	fstream file;

	file.open("OW.dat", ios::out |ios::binary | ios::trunc);
	
	//N�mero tiles
	int aux = overworld->mapTileMatrix->size();
	file.write((char *)& aux,sizeof(int));
	//World width
	aux = overworld->getWorldSizeW();
	file.write((char *)& aux,sizeof(int));
	//Tile info
	for (int i=0; i<overworld->mapTileMatrix->size(); i++){
		aux = overworld->mapTileMatrix->at(i)->getTileId();
		file.write((char *)&aux,sizeof(int));
	}
	file.close();
}

void GenVoroWorld::genBlockades(){
	//cout << "Ejecutando funcion <GenOverworld::genBlockades()>" << endl;
}

//Generar un screen para cada Zona
void GenVoroWorld::genScreens(){
	for (unsigned int i = 0; i < genZones->size(); i++){
		GenZone* gen = genZones->at(i);
		gen->genScreens();
	}

    //DE CO�A
	cout<< overworld->mapTileMatrix->size() << " tiles de mapa" << endl;
}

void GenVoroWorld::floodFillScanlineStack(int x, int y, int zoneNum)
{
    if(overworld->getMapTile(x,y)->getZoneNumber() != -1) return;
      
    int y1,h,w;
    
	h = overworld->getWorldSizeH();
	w = overworld->getWorldSizeW();
    //draw current scanline from start position to the top
    y1 = y;
    while(y1 < h && overworld->getMapTile(x,y1)->getZoneNumber() == -1)
    {
        overworld->getMapTile(x,y1)->setZoneNumber(zoneNum);
        y1++;
    }    
    
    //draw current scanline from start position to the bottom
    y1 = y - 1;
    while(y1 >= 0 && overworld->getMapTile(x,y1)->getZoneNumber() == -1)
    {
        overworld->getMapTile(x,y1)->setZoneNumber(zoneNum);
        y1--;
    }
    
    //test for new scanlines to the left
    y1 = y;
    while(y1 < h && overworld->getMapTile(x,y1)->getZoneNumber() == zoneNum)
    {
        if(x > 0 && overworld->getMapTile(x - 1,y1)->getZoneNumber() == -1) 
        {
            floodFillScanlineStack(x - 1, y1, zoneNum);
        } 
        y1++;
    }
    y1 = y - 1;
    while(y1 >= 0 && overworld->getMapTile(x,y1)->getZoneNumber() == zoneNum)
    {
        if(x > 0 && overworld->getMapTile(x - 1,y1)->getZoneNumber() == -1) 
        {
            floodFillScanlineStack(x - 1, y1, zoneNum);
        }
        y1--;
    } 
    
    //test for new scanlines to the right 
    y1 = y;
    while(y1 < h && overworld->getMapTile(x,y1)->getZoneNumber() == zoneNum)
    {
        if(x < w - 1 && overworld->getMapTile(x + 1,y1)->getZoneNumber() == -1) 
        {           
            floodFillScanlineStack(x + 1, y1, zoneNum);
        } 
        y1++;
    }
    y1 = y - 1;
    while(y1 >= 0 && overworld->getMapTile(x,y1)->getZoneNumber() == zoneNum)
    {
        if(x < w - 1 && overworld->getMapTile(x + 1,y1)->getZoneNumber() == -1) 
        {
            floodFillScanlineStack(x + 1, y1, zoneNum);
        }
        y1--;
    }
}

