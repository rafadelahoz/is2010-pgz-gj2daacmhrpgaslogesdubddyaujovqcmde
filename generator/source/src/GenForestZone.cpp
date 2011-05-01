#include "GenForestZone.h"

// Constructora.
GenForestZone::GenForestZone(string theme, string zone, int zoneNumber, GPolygon* zoneShape, Overworld* ow, short numEnemies,
						 GenDungeon* genDungeon, short numDungeon, short idTool, short ratioDungeon, vector<SafeZoneInfo>* safeZones, Decorator* decorator, DBManager* myDB)
			: GenZone(theme, zone, zoneNumber, zoneShape, ow, numEnemies, genDungeon, numDungeon, idTool, ratioDungeon, safeZones, decorator, myDB){
	seeds = new vector<int>();
}

// Destructora.
GenForestZone::~GenForestZone()
{ 
	seeds->clear();
	delete seeds;
	seeds = NULL;
}


void GenForestZone::genScreens(){
   	for (unsigned int i=0; i< screenList->size(); i++){
		OwScreen* screen = screenList->at(i);
		screen->placeDetails();
		screen->placeEnemies();
	}
}


void GenForestZone::placeDungeon()
{	
	//int screensPerRow = overworld->getWorldSizeW();
	//bool goodScreen = false;
	//int screenN, screenNFirst;
	//screenNFirst = screenList->at(rand()%screenList->size())->getScreenNumber();
	//screenN = screenNFirst;
	//while(!goodScreen)
	//{
	//	goodScreen = true;
	//	if(((screenN+1)%screensPerRow) != 0)
	//		goodScreen = overworld->screenList->at(screenN+1)->getZoneNum() == overworld->screenList->at(screenN)->getZoneNum();
	//	if(goodScreen && (screenN+screensPerRow) < (screensPerRow*overworld->getWorldSizeH()))
	//		goodScreen = overworld->screenList->at(screenN+overworld->getWorldSizeW())->getZoneNum() == overworld->screenList->at(screenN)->getZoneNum();
	//	if(goodScreen && ((screenN-1)%screensPerRow) != (screensPerRow-1) && (screenN-1) >= 0)
	//		goodScreen = overworld->screenList->at(screenN-1)->getZoneNum() == overworld->screenList->at(screenN)->getZoneNum();
	//	if(goodScreen && (screenN-screensPerRow) >= 0)
	//		goodScreen = overworld->screenList->at(screenN-screensPerRow)->getZoneNum() == overworld->screenList->at(screenN)->getZoneNum();

	//	screenN = (screenN + 1)%(screenList->size());
	//	if(!goodScreen)
	//		goodScreen = screenN == screenNFirst;
	//}

	////coordenadas de la screenN dentro del mundo.
	//int screenX = screenN % overworld->getWorldSizeW();
	//int screenY = screenN / overworld->getWorldSizeW();

	//// coordenada X e Y del tile donde se encuentra la mazmorra
	//int screenTileY = (rand()%(SCREEN_HEIGHT-8))+4; //ponemos 4 tiles para que no salga en el borde de la pantalla
	//int screenTileX = (rand()%(SCREEN_WIDTH-8))+4;
	//
	//int tileY = (screenY * SCREEN_HEIGHT) + screenTileY; //ponemos 4 tiles para que no salga en el borde de la pantalla
	//int tileX = (screenX * SCREEN_WIDTH) + screenTileX;
	//
	//// el tile dentro del mapa de tiles grande.
	//int tile = (tileY * overworld->getTileWorldSizeW()) + tileX;

	//overworld->mapTileMatrix->at(tile)->setSolid(0);
	//dungEntranceTile = tile;
	//// POSICI�N DE LA MAZMORRA HERE!!
	//DungeonPos dp;
	//dp.screenX = screenN%screensPerRow;
	//dp.screenY = screenN/screensPerRow;
	//dp.tileX = screenTileX;
	//dp.tileY = screenTileY;
	//Dungeon* newDungeon = genDungeon->createDungeon(zone, theme, gameDifficulty, numDungeon, ratioDungeon, idTool, 2/*keyObj*/, dp/*Posici�n de la mazmorra*/, myDB);
	//int dunScreenX = newDungeon->getIniDScreenX();
	//int dunScreenY = newDungeon->getIniDScreenY();
	//int dunTileX = newDungeon->getIniDTileX();
	//int dunTileY = newDungeon->getIniDTileY();
	//EntityTeleporter* e = new EntityTeleporter(TELEPORTATOR, tileX, tileY, -1/*idCollectable*/, -1/*linkedTo*/, numDungeon/*idMap*/, dunScreenX, dunScreenY, dunTileX, dunTileY);
	//overworld->screenList->at(screenN)->getEntities()->push_back(e);
	////crear espacio alrededor del teleporter
	//overworld->screenList->at(screenN)->setSolid(screenTileX+1,screenTileY,0);
	//overworld->screenList->at(screenN)->setSolid(screenTileX+1,screenTileY-1,0);  //esquina sup-der
	//overworld->screenList->at(screenN)->setSolid(screenTileX,screenTileY+1,0);
	//overworld->screenList->at(screenN)->setSolid(screenTileX+1,screenTileY+1,0);  //esquina inf-der
	//overworld->screenList->at(screenN)->setSolid(screenTileX-1,screenTileY,0);
	//overworld->screenList->at(screenN)->setSolid(screenTileX-1,screenTileY+1,0);  //esquina inf-izq
	//overworld->screenList->at(screenN)->setSolid(screenTileX,screenTileY-1,0);
	//overworld->screenList->at(screenN)->setSolid(screenTileX-1,screenTileY-1,0);  //esquina sup-izq

	int screenNumber = 0;

	int screensPerRow = overworld->getWorldSizeW() / SCREEN_WIDTH;
	int tilesPerRow = screensPerRow*SCREEN_WIDTH;
	// Pantalla de comienzo del gusano
	// por ahora se elige una al azar y creo que se va a quedar as�
	int tile;
	if ( screenList->size() != 0 ){

		int iniTile = getTileOfScreen(screenNumber);
		tile = iniTile;
		bool placed = false;
		short range = 20;
		short tries = 0;
		
		while (!placed){
			if (tile < overworld->mapTileMatrix->size() &&
				overworld->mapTileMatrix->at(tile)->getZoneNumber() == this->zoneNumber && 
				overworld->mapTileMatrix->at(tile)->getSolid() > 0 ){
				if ( !isFrontierNear(tile, range) ){
					placed = true;
					overworld->mapTileMatrix->at(tile)->setTileId(0);
					dungEntranceTile = tile;
				}
				else{
					iniTile = getTileOfScreen(screenNumber);
					tile = iniTile;
				}
			}
			else{
				if (tile+1 < overworld->mapTileMatrix->size() &&
					overworld->mapTileMatrix->at(tile + 1)->getZoneNumber() == this->zoneNumber)
					tile++;
				else{
					iniTile = getTileOfScreen(screenNumber);
					tile = iniTile;
				}
			}
			tries++;

			if (tries == 10 || tries == 20 || tries == 30)
				range -=5;
			else if (tries == 40)
				range = 2;
		}
	}


	//coordenadas de la screenN dentro del mundo.
	int screenX = screenNumber % overworld->getWorldSizeW();
	int screenY = screenNumber / overworld->getWorldSizeW();

	int tileX = dungEntranceTile % SCREEN_WIDTH;
	int tileY = dungEntranceTile / SCREEN_HEIGHT;
	
	
	// el tile dentro del mapa de tiles grande.
	//int tile = (tileY * overworld->getTileWorldSizeW()) + tileX;

	DungeonPos dp;
	dp.screenX = screenNumber%screensPerRow;
	dp.screenY = screenNumber/screensPerRow;
	dp.tileX = tileX;
	dp.tileY = tileY;
	genDungeon->createDungeon(zone, theme, gameDifficulty, numDungeon, ratioDungeon, idTool, 2/*keyObj*/, dp/*Posici�n de la mazmorra*/, myDB);

	Dungeon* newDungeon = genDungeon->createDungeon(zone, theme, gameDifficulty, numDungeon, ratioDungeon, idTool, 2/*keyObj*/, dp/*Posici�n de la mazmorra*/, myDB);
	int dunScreenX = newDungeon->getIniDScreenX();
	int dunScreenY = newDungeon->getIniDScreenY();
	int dunTileX = newDungeon->getIniDTileX();
	int dunTileY = newDungeon->getIniDTileY();
	EntityTeleporter* e = new EntityTeleporter(TELEPORTATOR, tileX, tileY, -1/*idCollectable*/, -1/*linkedTo*/, numDungeon/*idMap*/, dunScreenX, dunScreenY, dunTileX, dunTileY);
	overworld->screenList->at(screenNumber)->getEntities()->push_back(e);

	//crear espacio alrededor del teleporter
	/*overworld->screenList->at(screenN)->setSolid(screenTileX+1,screenTileY,0);
	overworld->screenList->at(screenN)->setSolid(screenTileX+1,screenTileY-1,0);  //esquina sup-der
	overworld->screenList->at(screenN)->setSolid(screenTileX,screenTileY+1,0);
	overworld->screenList->at(screenN)->setSolid(screenTileX+1,screenTileY+1,0);  //esquina inf-der
	overworld->screenList->at(screenN)->setSolid(screenTileX-1,screenTileY,0);
	overworld->screenList->at(screenN)->setSolid(screenTileX-1,screenTileY+1,0);  //esquina inf-izq
	overworld->screenList->at(screenN)->setSolid(screenTileX,screenTileY-1,0);
	overworld->screenList->at(screenN)->setSolid(screenTileX-1,screenTileY-1,0);  //esquina sup-izq*/
}

// Por decidir, de primeras coloca la entrada a una zona segura.
void GenForestZone::placeSafeZone(int idZone,GPoint* pos)
{
	//cout << "Ejecutando funcion <>Zone::placeSafeZone()>" << endl;
}

//Vamos a crear bosques
void GenForestZone::genGeoDetail()
{
	int numScreens = screenList->size();
	sowSeeds(numScreens-numScreens/5); //Plantamos semillas
	int numSolids = floor((numScreens*SCREEN_HEIGHT*SCREEN_WIDTH)*0.60);
	waterSeeds(numSolids);  //regamos las semillas para que se reproduzcan
}

//plantamos semillas de las cuales aparecer� un grupo de �rboles.
bool GenForestZone::sowSeeds(int numSeeds)
{
	int sowed = 0;  //semillas plantadas
	int posSeed;  //donde se va a encontrar nuestra semilla dentro de la matriz de pantalla
	int screenN; //actual screen number
	int screenX, screenY;  //coordenadas X Y de la screen Actual
	int tileX, tileY;  //coordenadas X Y sobre el mapa mundi del tile izq-arriba de la matriz de la pantalla
	int tileInic; // tile inicial de la matriz de la screen sobre el mapa mundi

	//plantamos una semilla por pantalla
	int i = 0;
	while (i < screenList->size() && sowed < numSeeds)
	{
		screenN = screenList->at(rand()%screenList->size())->getScreenNumber();  //cogemos una screen

		posSeed = ((rand()%SCREEN_HEIGHT) * overworld->getTileWorldSizeW()) + rand()%SCREEN_WIDTH; //donde se va a ubicar la semilla (offSet)

		//coordenadas de la screenN dentro del mundo.
		screenX = screenN % overworld->getWorldSizeW();
		screenY = screenN / overworld->getWorldSizeW();

		// coordenada X e Y del tile inicial de pantalla
		tileY = screenY * SCREEN_HEIGHT;
		tileX = screenX * SCREEN_WIDTH;
	
		// el tile dentro del mapa de tiles grande.
		tileInic = (tileY * overworld->getTileWorldSizeW()) + tileX;

		seeds->push_back(tileInic + posSeed);  //guardamos la posici�n de la semilla dentro del mapa mundi
		overworld->getMapTile(tileInic+posSeed)->setTileId(222); //para debugueo <-------------------------------- Quitar
		sowed++;
		i++;
	}

	//plantamos semillas en las esquinas de las pantalla (que queda m�s cool ^^)
	while( sowed < numSeeds)
	{
		if ((sowed % 4) == 0)
			posSeed = 0;
		else
			posSeed = (SCREEN_WIDTH-1);

		screenN = screenList->at(rand() % screenList->size())->getScreenNumber();
		//coordenadas de la screenN dentro del mundo.
		screenX = screenN % overworld->getWorldSizeW();
		screenY = screenN / overworld->getWorldSizeW();

		// coordenada X e Y del tile inicial de pantalla
		tileY = screenY * SCREEN_HEIGHT;
		tileX = screenX * SCREEN_WIDTH;
	
		// el tile dentro del mapa de tiles grande.
		tileInic = (tileY * overworld->getTileWorldSizeW()) + tileX;
		seeds->push_back(tileInic + posSeed);
		sowed++;
	}

	return true;
}

bool GenForestZone::waterSeeds(int numSolids)
{
	int growFactor;
	int actualSeed = 0;
	int actualSolids = 0;
	while(actualSeed < seeds->size() && actualSolids < numSolids)
	{
		growFactor = rand()%51 + 40;
		actualSolids = actualSolids + growSeed(seeds->at(actualSeed), growFactor);
		actualSeed++;
	}
	return true;
}

int GenForestZone::growSeed(int posSeed, int growFactor)
{
	int tilesPerRow = overworld->getTileWorldSizeW(); //nos ahorramos muchos getters ^^
	int solidsMade = 0;

	if((posSeed+1)%tilesPerRow != 0) //se puede a la derecha? no nos salimos de la columna?
	{
		if(overworld->getMapTile(posSeed+1)->getZoneNumber() == zoneNumber)  // y estamos en la misma zona?
		{
			if(overworld->getMapTile(posSeed+1)->getSolid() == 0)  //era ya un s�lido?
			{
				solidsMade++;
				overworld->getMapTile(posSeed+1)->setSolid(1);
				if(rand()%101 < growFactor)//la semilla consigue crecer?? (a mayor factor de crecimiento m�s posibilidades)
					seeds->push_back(posSeed+1); //a�adimos la nueva semilla
			}
			//else ya hay un arbol en la posici�n.
		}
		//else no ha conseguido sembrarse la semilla.
	}

	if((posSeed+tilesPerRow) < (tilesPerRow * overworld->getTileWorldSizeH())) //se puede hacia abajo? no nos salimos de la matriz?
	{
		if(overworld->getMapTile(posSeed+tilesPerRow)->getZoneNumber() == zoneNumber)
		{
			if(overworld->getMapTile(posSeed+tilesPerRow)->getSolid() == 0)  //era ya un s�lido?
			{
				solidsMade++;
				overworld->getMapTile(posSeed+tilesPerRow)->setSolid(1);
				if(rand()%101 < growFactor)//la semilla consigue crecer?? (a mayor factor de crecimiento m�s posibilidades)
					seeds->push_back(posSeed+tilesPerRow); //a�adimos la nueva semilla
			}
			//else ya hay un arbol en la posici�n.
		}
		//else no ha conseguido sembrarse la semilla.
	}

	if((posSeed-1) > 0 && ((posSeed-1)%tilesPerRow) != (tilesPerRow-1)) //no nos vamos a otra columna(atravesamos la matriz)
	{
		if(overworld->getMapTile(posSeed-1)->getZoneNumber() == zoneNumber)
		{
			if(overworld->getMapTile(posSeed-1)->getSolid() == 0)  //era ya un s�lido?
			{
				solidsMade++;
				overworld->getMapTile(posSeed-1)->setSolid(1);
				if(rand()%101 < growFactor)//la semilla consigue crecer?? (a mayor factor de crecimiento m�s posibilidades)
					seeds->push_back(posSeed-1); //a�adimos la nueva semilla
			}
			//else ya hay un arbol en la posici�n.
		}
		//else no ha conseguido sembrarse la semilla.
	}

	if((posSeed-tilesPerRow) > 0) //se puede hacia arriba? no nos salimos de la matriz?
	{
		if(overworld->getMapTile(posSeed-tilesPerRow)->getZoneNumber() == zoneNumber)
		{
			if(overworld->getMapTile(posSeed-tilesPerRow)->getSolid() == 0)  //era ya un s�lido?
			{
				solidsMade++;
				overworld->getMapTile(posSeed-tilesPerRow)->setSolid(1);
				if(rand()%101 < growFactor) //la semilla consigue crecer?? (a mayor factor de crecimiento m�s posibilidades)
					seeds->push_back(posSeed-tilesPerRow); //a�adimos la nueva semilla
			}
			//else ya hay un arbol en la posici�n.
		}
		//else no ha conseguido sembrarse la semilla.
	}

	return solidsMade;

}

void GenForestZone::genDetail()
{
	decorator->init("mipene", "roger", "world.png");
	for(vector<OwScreen*>::iterator it = screenList->begin(); it != screenList->end(); it++)
		(*it)->generate(), decorator->decorate(*(it));
}