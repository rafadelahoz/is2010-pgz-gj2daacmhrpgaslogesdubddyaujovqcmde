#include "Game.h"

Game::Game(){}

void Game::genGame(DBManager* myDB){
	/* ---- Decidator obtiene los datos para los generadores ---- */
	// la GUI guardar� el archivo que posteriormente leer� decidator para obtener la informaci�n
	decidator = new Decidator(myDB, "./file.txt");
	int wSize = decidator->getWorldSize();
	int numDungeons = decidator->getNumDungeons();
	int numZones = decidator->getNumZones();
	int diff = decidator->getDifficulty();
	int ratioDungeon = decidator->getRatio();
	int numSafeZones = decidator->getNumSafeZones();
	int numEnemies = decidator->getNumEnemies();

	zones = new vector<GenZone*>();
	ow = new Overworld(wSize, diff, numZones, numDungeons, numSafeZones);
	GenDungeon* genDungeon = new GenDungeon();
	int numDungeon = 1; 
	int idTool = 1;   //params para la dungeon
	vector<SafeZoneInfo>* safeZones = NULL;//new vector<SafeZoneInfo>();
	string zoneTheme;
	for (int zoneNumber = 1; zoneNumber <= numZones; zoneNumber++)
	{
		switch(zoneNumber){ //esto lo har�a decidator pero no hay tiempo de pensar ahora o.O!
		case 1:
			zoneTheme = "Forest";
			break;
		case 2:
			zoneTheme = "Desert";
			break;
		case 3:
			zoneTheme = "Swamp";  //pantano
			break;
		default:
			zoneTheme = "Field";  
		}
		GenZone* myGenZone = new GenWormZone("theme-default", zoneTheme, zoneNumber, NULL, ow, numEnemies, genDungeon, /*numDungeon*/zoneNumber, idTool, ratioDungeon, safeZones, myDB);
		zones->push_back(myGenZone);
	}
	
	// Decidator obtiene de la base de dator el generador de mundo a utilizar
	/*switch (decidator->getWorldGen()){   Lo comento de momento porque la consulta no devuelve na y estalla esto x.x
	case 1:
		genOw = new GenVoroWorld(ow, zones, myDB);	
		break;
	}*/
	genOw = new GenVoroWorld(ow, zones, myDB);	

	world = new World(diff, genOw, myDB);

	world->buildOverworld();
	ow->save(); //ahora aqu� se hace el guardado
	for (int i = 0; i < genDungeon->getNumDungeons(); i++) //guardamos todas las dungeons
		genDungeon->getDungeon(i)->save();

	// Decidator guarda la informaci�n que necesita el int�rprete (como n�mero de piezas de coraz�n, etc...)
	decidator->save();

	delete genDungeon; 
	genDungeon = NULL;
}

Game::~Game(){

	vector<GenZone*>::iterator it;
    for(it = zones->begin(); it != zones->end(); it++)
        if ((*it) != NULL)
        {
			delete (*it);
			(*it) = NULL;
        }
	delete zones;
	zones = NULL;
	
	delete ow;			// SOLO LO BORRA ESTE PORQUE ES EL CLIENTE DICTADOR
	ow = NULL;

	delete genOw;		// As� nos aseguramos que llama al delete del GenVoronoiOW
	genOw = NULL;

	delete world; 
	world = NULL;

	delete decidator;
	decidator = NULL;
}
