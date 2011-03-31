#include "controller.h"

Controller::Controller(Game* g)
{
	// Almacena los par�metros necesarios
	game = g;
	width = game->getGameConfig()->gameWidth;
	height = game->getGameConfig()->gameHeight;

	// Crea las im�genes que usar� para el cambio de posici�n del mapa
	currentRoom = new Image(width, height, game->getGfxEngine(), false, true);
	nextRoom = new Image(width, height, game->getGfxEngine(), false, true);

	// Crea Data
	data = new DataPersistence();
	gamePlayState = NULL;
	dbi = new DataBaseInterface();
	state = NORMAL;
}
	
Controller::~Controller()
{
}
	


bool Controller::initData(std::string path)
{
	GameData* gdata = data->getGameData();
	GameStatus* gstatus = gdata->getGameStatus();


	// FROM GAME
	numPlayers = 1;

	// MAPDATA

	int numMaps, mapId, w, h, numPuzzles, numDoors, numMinibosses;
	char type;
	int** layout;

	// FROM DB
	numMaps = 1;

	for (int i = 0; i < numMaps; i++)
	{
		mapId = i;
		// FROM FILE
		type = '0';
		w = 2;
		h = 2;
	
		layout = (int**) malloc(sizeof(int*)*w);
		for (int i = 0; i < w; i++)
			layout[i] = (int*) malloc(sizeof(int)*h);

		layout[0][0] = 0;
		layout[0][1] = 1;
		layout[1][0] = 1;
		layout[1][1] = 1;

		numPuzzles = 0;
		numDoors = 0;
		numMinibosses = 0;

		data->addMapData(mapId, type, w, h, (const int**) layout, numPuzzles, numDoors, numMinibosses);
	}

	// GDATA

	int neededHeartPieces;

	neededHeartPieces = 4;
	gdata->init(neededHeartPieces);


	// GSTATUS

	int numKeyItems, maxLife, actualMoney, numPidgeons, numHeartPieces, barterProgress, gameProgress;
	MapLocation actualScreen;
	std::map<int,ToolInfo> tools;
	std::pair<int,int> lastPos;

	if (path != "")
	{
		// FROM FILE
		numKeyItems;
		maxLife;
		actualMoney;
		numPlayers;
		numPidgeons;
		numHeartPieces;
		barterProgress;
		gameProgress;
		actualScreen;
		tools;
		lastPos;
	}
	else
	{
		// FROM DB
		maxLife = 3;			//DataBaseInterface->initialMaxLife();
		tools;					//DataBaseInterface->initialTools();
		actualScreen.id = 0;	//DataBaseInterface->initialMap();

		// FROM MAPDATA

		actualScreen.positionX = 0;
		actualScreen.positionY = 0;
		actualScreen.screenX = 80;
		actualScreen.screenY = 50;

		// POSSIBLY FROM DB
		
		actualMoney = 0; //DataBaseInterface->initialMoney();
		numKeyItems = 0; //DataBaseInterface->initialKeyItems();

		// NOT FROM DB
		numPidgeons = 0;
		numHeartPieces = 0;
		barterProgress = 0;
		gameProgress = 0;
		lastPos.first = actualScreen.screenX;
		lastPos.second = actualScreen.screenX;
	}

	gstatus->init(	numKeyItems, 
					maxLife,
					tools, 
					actualMoney,
					actualScreen, 
					lastPos, 
					numPlayers,
					numPidgeons,
					numHeartPieces, 
					barterProgress, 
					gameProgress
				);


	// MAPSTATUS

	std::map<int, bool> collectables, doors, puzzles, minibosses;
	
	// ONLY IF DUNGEON
	bool bossDefeated, gotBossKey, gotCompass, gotMap, gotPowerUp;

	MapData* mapData;
	MapStatus* mapStatus;

	if (path != "")
	{
		for (int i = 0; i < numMaps; i++)
		{
			// FROM FILE
			collectables;
			doors;
			puzzles;
			minibosses;

			mapData = data->getMapData(i);
			mapStatus = mapData->getMapStatus();

			mapStatus->setCollectables(collectables);
			mapStatus->setDoors(doors);
			mapStatus->setPuzzles(puzzles);
			mapStatus->setMinibosses(minibosses);


			if (mapData->getType() == 1)
			{
				// FROM FILE
				bossDefeated;
				gotBossKey;
				gotCompass;
				gotMap;
				gotPowerUp;

				((DungeonMapStatus*) mapStatus)->setBossDefeated(bossDefeated);
				((DungeonMapStatus*) mapStatus)->setBossKeyGot(gotBossKey);
				((DungeonMapStatus*) mapStatus)->setCompassGot(gotCompass);
				((DungeonMapStatus*) mapStatus)->setMapGot(gotMap);
				((DungeonMapStatus*) mapStatus)->setPowerUpGot(gotPowerUp);

			}
		}
	}
	else
	{
		for (int i = 0; i < numMaps; i++)
		{
			// FROM DB & FILE
			collectables;
			doors;
			puzzles;
			minibosses;

			mapData = data->getMapData(i);
			mapStatus = mapData->getMapStatus();

			mapStatus->setCollectables(collectables);
			mapStatus->setDoors(doors);
			mapStatus->setPuzzles(puzzles);
			mapStatus->setMinibosses(minibosses);

			if (mapData->getType() == 1)
			{
				bossDefeated = false;;
				gotBossKey = false;;
				gotCompass = false;
				gotMap = false;
				gotPowerUp = false;

				((DungeonMapStatus*) mapStatus)->setBossDefeated(bossDefeated);
				((DungeonMapStatus*) mapStatus)->setBossKeyGot(gotBossKey);
				((DungeonMapStatus*) mapStatus)->setCompassGot(gotCompass);
				((DungeonMapStatus*) mapStatus)->setMapGot(gotMap);
				((DungeonMapStatus*) mapStatus)->setPowerUpGot(gotPowerUp);

			}
		}
	}

	return true;
}

bool Controller::shortInitData(std::string path){

	GameStatus* gstatus = data->getGameData()->getGameStatus();

	int numKeyItems, maxLife, actualMoney, numPidgeons, numHeartPieces, barterProgress, gameProgress;
	MapLocation actualScreen;
	std::map<int,ToolInfo> tools;
	std::pair<int,int> lastPos;

	if (path != "")
	{
		// FROM FILE
		numKeyItems;
		maxLife;
		actualMoney;
		numPlayers;
		numPidgeons;
		numHeartPieces;
		barterProgress;
		gameProgress;
		actualScreen;
		tools;
		lastPos;
	}
	else
	{
		// FROM DB
		maxLife = 3;			//DataBaseInterface->initialMaxLife();
		tools;					//DataBaseInterface->initialTools();
		actualScreen.id = 0;	//DataBaseInterface->initialMap();

		// FROM MAPDATA

		actualScreen;

		// POSSIBLY FROM DB
		
		actualMoney = 0; //DataBaseInterface->initialMoney();
		numKeyItems = 0; //DataBaseInterface->initialKeyItems();

		// NOT FROM DB
		numPidgeons = 0;
		numHeartPieces = 0;
		barterProgress = 0;
		gameProgress = 0;
		lastPos.first = actualScreen.screenX;
		lastPos.second = actualScreen.screenX;
	}

	gstatus->init(	numKeyItems, 
					maxLife,
					tools, 
					actualMoney,
					actualScreen, 
					lastPos, 
					numPlayers,
					numPidgeons,
					numHeartPieces, 
					barterProgress, 
					gameProgress
				);

	return true;
}

bool Controller::initGamePlayState(GamePlayState* gpst)
{

	if (gamePlayState != NULL)
		delete gamePlayState;

	gamePlayState = gpst;

	hudController = new HUDController(game, gamePlayState);
	toolController = new ToolController();
	eventController = new EventController(game, gamePlayState, this);
	
	gamePlayState->_add(eventController);


/* ---------------------------------------------------------------------
1.	Localiza el mapa actual y la pantalla v�a data
--------------------------------------------------------------------- */

	MapLocation location = data->getGameData()->getGameStatus()->getCurrentMapLocation();

/* ---------------------------------------------------------------------
2.	Carga la pantalla actual en su totalidad, y por cada entidad que
	cargue, la env�a a GameState al add sobrecargado. Al final
	se llama al init de ScreenMap con todos los datos cargados.
--------------------------------------------------------------------- */

	load_screen(location);

/* ---------------------------------------------------------------------
3.	Crea los players en la posici�n configurada de la pantalla del
	mapa actual.
--------------------------------------------------------------------- */

	DataBaseInterface::HeroData heroData;
	for (int i = 0; i < numPlayers; i++)
	{
		heroData = dbi->getHeroData();
		players[i] = new Player(location.screenX, location.screenY, game, gamePlayState);
		players[i]->init(heroData.gfxPath, 4, 44, heroData.hpMax, heroData.mpMax, this);
		gamePlayState->_add(players[i]);
	}

/* ---------------------------------------------------------------------
4.	Crea el hud adecuado a la cantidad de players.
--------------------------------------------------------------------- */

	// for each player
	// hud controller->addhud(player[i])

/* ---------------------------------------------------------------------
7.	Playea todos los elementos que ha creado de la nueva pantalla.
--------------------------------------------------------------------- */

	return true;
}

bool Controller::load_screen(MapLocation m)
{
	// WHAT TO DO WITH OLD SCREENMAP

	if (screenMap != NULL)
		; // a la cola
	
	FILE* file = NULL;
    file = fopen("./map", "r");



	int** solids;
	int columns;
	int rows;
	int tilew;
	int tileh;

	fscanf(file, "%d", &columns);
    fscanf(file, "%d", &rows);
	fscanf(file, "%d", &tilew);
    fscanf(file, "%d", &tileh);

	int** tiles;
	tiles = (int**) malloc(sizeof(int*)*columns);
	for (int i = 0; i < columns; i++)
		tiles[i] = (int*) malloc(sizeof(int)*rows);

	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
		{
            fscanf(file, "%d", &tiles[i][j]);
		}

	solids = (int**) malloc(sizeof(int*)*columns);
	for (int i = 0; i < columns; i++)
		solids[i] = (int*) malloc(sizeof(int)*rows);

	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
		{
            fscanf(file, "%d", &solids[i][j]);
		}

    fclose(file);

	screenMap = new ScreenMap(tilew*columns, tileh*rows, tilew, tileh, 0, 0, game->getGfxEngine());
	screenMap->setSolids(0, 0, solids, columns, rows);
	screenMap->setTiles(tiles, columns, rows);
	screenMap->setTileset("./gfx/tset.png");

	screenMap->getMapImage();
	gamePlayState->addMap(screenMap);
	return true;
}

bool Controller::move_screen(Dir dir){

/* ---------------------------------------------------------------------
1. Preguntar a Data por la pantalla destino. Recibe lista de propiedades
	1.1. Preguntar si la pantalla destino existe
--------------------------------------------------------------------- */
	
	// Obtenemos la pantalla destino del cambio
	int up; int left;
	up = left = 0;
	switch (dir) 
	{
		case UP:	up = -1; break;
		case DOWN:	up = 1; break;
		case LEFT:	left = -1; break;
		case RIGHT:	left = 1; break;
		default:	break;
	}

	/* Obtener el mapLocation de Data, rollo:

	MapLocation m;
	 
	m.mapX = m.mapX + left;
	m.mapY = m.mapY + up;       
	*/
 
	// Preguntamos si la pantalla existe
	if (true /*data->hasScreen(m)*/)
	{
			
/* ---------------------------------------------------------------------
	1.2. Obtenemos las propiedades de la pantalla destino.
--------------------------------------------------------------------- */
	
			// Ya est�n en m, lala(8)
			   
/* ---------------------------------------------------------------------
2. Desvisivilizar player y hud
	2.1. Desvisibilizar player
--------------------------------------------------------------------- */

		for (int i = 0; i < numPlayers; i++)
			players[i]->setVisible(false);
				
/* ---------------------------------------------------------------------
	2.2. Desvisibilizar hud
--------------------------------------------------------------------- */

//		hud->setVisible(false);       

/* ---------------------------------------------------------------------
3. Hace foto y la guarda
--------------------------------------------------------------------- */

		// Limpiar currentRoom si es necesario
		game->getGfxEngine()->setRenderTarget(currentRoom);
		gamePlayState->onRender();
		game->getGfxEngine()->resetRenderTarget();
		   
/* ---------------------------------------------------------------------
4. Mapa, c�rgame los datos del nuevo mapa y borra los anteriores ( o buffer ). 
El nuevo mapa sustituir� al actual, contendr� a los players y el hud y adem�s
las entidades cargadas deber�n estar disabled (de eso me ocupo yo, Controller).
--------------------------------------------------------------------- */

		// think
				
/* ---------------------------------------------------------------------
5. Hazle foto al nuevo mapa.
--------------------------------------------------------------------- */

		// Limpiar nextRoom si es necesario
		game->getGfxEngine()->setRenderTarget(nextRoom);
		gamePlayState->onRender();
		game->getGfxEngine()->resetRenderTarget();

/* ---------------------------------------------------------------------
6. Junta fotos.
--------------------------------------------------------------------- */
		
		// No es necesario si pintamos las 2 directamente en la pantalla de juego
		// Nos ahorramos una pantalla auxiliar

/* ---------------------------------------------------------------------
7. Visibiliza player y hud
	7.1 Hace visible el player. Ser� necesario adem�s disablearlo para 
	que no pueda moverse durante la transici�n
--------------------------------------------------------------------- */
	
		for (int i = 0; i < numPlayers; i++)
		{
			players[i]->setVisible(false);
			players[i]->disable();
		}
	
/* ---------------------------------------------------------------------
	7.2. Hace visible el Hud y lo disablea por el mismo motivo.
--------------------------------------------------------------------- */
	
			/*hud->setVisible(true);
			hud->disable();*/

/* ---------------------------------------------------------------------
8. Actualiza los datos con la nueva pos del player en el mapa.
--------------------------------------------------------------------- */

			// Igual ha de hacerse antes de cargar el nuevo ScreenMap
				
/* ---------------------------------------------------------------------
9. Colocaci�n del player + preparaci�n para la transici�n
	9.1. Colocaci�n del player
--------------------------------------------------------------------- */
	
			// PONER OFFSET A LOS MAPAS

			// Stupid code test
			int x, y;
			switch (dir) 
			{
				case UP: 
					// Mantenemos la x y cambiamos la y
					y = height; 
					break;
				case DOWN:
					// Mantenemos la x y cambiamos la y
					y = 0; 
					break;
				case LEFT:
					// Mantenemos la y y cambiamos la x
					x = width;
					break;
				case RIGHT: 
					// Mantenemos la y y cambiamos la x
					x = 0;
					break;
			};

/* ---------------------------------------------------------------------
	9.2. Aplicaci�n de efectos y preparaci�n de la transici�n.
--------------------------------------------------------------------- */
	
		setState(TRANSITION);
		transitionEffect = SCROLL;
			
		// Direcci�n expl�cita de la transici�n
		int xdir, ydir;
		xdir = ydir = 0;
			
		speed = 4;
		// Origen (mx,my) y destino (tx, ty) de la transici�n
		int mx, my, tx, ty;
		mx = my = tx = ty = 0;
		switch (dir) 
		{			
			case UP: 
				ydir = 1;
				ty = height;
				break;
			case DOWN: 
				ydir = -1;
				ty = -height;
				break;
			case LEFT:  
				xdir = 1; 
				tx = width;
				break;
			case RIGHT:  
				xdir = -1; 
				tx = -width;
				break;
		}

/* ---------------------------------------------------------------------
10. Finalizaci�n
--------------------------------------------------------------------- */
		return true;
	}
	else
		return false;
}


bool Controller::change_map(MapLocation m, Player* p, TransitionEffect te, bool brute){

/* -----------------------
	STAAART!

	Si brute est� activado, ir a 1 directamente.

	A. Ir al map de portales activos y aumentar en 1 el contador de �ste (el id del portal puede ser interno), si no existe se pone a 1.
	B. Si el contador aumentado es mayor que la mitad de los players
		B1. Hacer la correspondiente animaci�n del player con el portal.
		B2. Desactivar al player.
		B3.ir a 1.
	C. Si el contador es menor, y es el primer portal que se toca (easy, si el mapa estaba vac�o al entrar al m�todo), poner un timer(0, n?)
		C1. Hacer la correspondiente animaci�n del player con el portal.
		C2. Desactivar al player.
		C3. return (false?)

	1. Preguntar a la DBI si existe el mapa destino. (b�sicamente pedirle el archivo correspondiente al id y si le da caca pues caca no vale)
		1y. Si existe, obtener v�a DBI el archivo del mapa (done en 1), cargarlo (loadmap) y meter la info Data (no inclu�do en loadmap, loadmap devuelve la info).
		1n. Si no existe, el portal no tiene efecto [return false]

	// A partir de aqu�, �dem al cambio de pantalla (�reusar c�digo?)

	2. Preguntar a Data si existe la pantalla dada (rezando que Data tenga esa funci�n).
		2y. Si no, elegir la pantalla de inicio del mapa (cargada en 1a).
		2n. Si existe, ir a 3.
	3. Hacer invisibles a los player y al hud
	4. Hacer foto
	5. Cargar la pantalla destino.
	6. Dar la info cargada a ScreenMap y GameState. Las entidades que se carguen deber�n disablearse.
	7. Foto
	8. Visibilizar players y hud; disablearlos para la transici�n.
	9. Preparar los efectos de la transici�n
	10. Fin! [return true]

	FINIISH!
----------------------- */

	return true;
}


Controller::State Controller::getState()
{
	return state;
}

void Controller::setState(State st)
{
	state = st;
}
		
int Controller::getNumPlayers()
{
	return numPlayers;
}

Controller::TransitionEffect Controller::getTransitionEffect()
{
	return transitionEffect;
}

ScreenMap* Controller::getScreenMap()
{
	return screenMap;
}
Player* Controller::getPlayer(int i)
{
	if ((i >= 0) && (i < numPlayers))
		return players[i];
	else
		return NULL;
}

HUDController* Controller::getHUDController()
{
	return hudController;
}

ToolController* Controller::getToolController()
{
	return toolController;
}

EventController* Controller::getEventController()
{
	return eventController;
}
		
bool Controller::addPlayer(/*params?*/)
{

	return true;
}

bool Controller::removePlayer(Player* p)
{
	return true;
}

bool Controller::removePlayer(int i)
{
	return true;
}