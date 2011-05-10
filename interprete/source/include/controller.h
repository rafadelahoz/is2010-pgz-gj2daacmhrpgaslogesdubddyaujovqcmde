#pragma once

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "HelperTypes.h"

#include "HUDController.h"
#include "ToolController.h"
#include "MessageController.h"
#include "EventController.h"

#include "GamePlayState.h"
#include "ScreenMap.h"
#include "DataPersistence.h"
#include "DataBaseInterface.h"

#include "DungeonMapStatus.h"
#include <map>
#include <deque>
#include "GamePuzzle.h"

#include "Enemy.h"

class GamePlayState;
class ScreenMap;
class ScreenMapConstructor;
class ToolController;
class MessageController;
class EventController;
class GamePuzzle;
class Enemy;

class Controller
{

	friend class HUDController;
	friend class ToolController;
	friend class EventController;
	friend class MessageController;

	public:

		enum State {NORMAL, TRANSITION};

		Game* game;
		InputConfig mainInputConfig;

		Controller(Game* g);
		~Controller();

		bool loadInputConfig(InputConfig& ic, std::string path);
		// Carga el saveslot de ruta path en Data, o la inicia con la BD si path es nulo
		bool initData(std::string path/*, players info*/);
		// Carga r�pida de valores b�sicos del saveslot de ruta path
		bool shortInitData(std::string path);
		// Inicia la informaci�n necesaria para comenzar la ejecuci�n del juego.
		bool initGamePlayState(GamePlayState* gpst);

		// Cambio al mapa m con transici�n te solicitado por P, de forma bruta (no espera al resto de compa�eros) o no
		bool teleportTo(MapLocation m, Player* p, TransitionEffect te, bool brute = false);
		// Desplaza el mapa a la pantalla contigua en direcci�n dir
		bool moveScreen(Direction dir);

		// Cambia efectivamente el mapa, llamado por los anteriores
		bool changeLocation(MapLocation target);

		State getState();
		void setState(State st);

		int getNumPlayers();
		TransitionEffect getTransitionEffect();
		ScreenMap* getScreenMap();
		Player* getPlayer(int i);

		DataBaseInterface* getDataBaseInterface();
		HUDController* getHUDController();
		ToolController* getToolController();
		MessageController* getMessageController();
		EventController* getEventController();

		bool addPlayer(/*params?*/);
		bool removePlayer(Player* p);
		bool removePlayer(int i);

		DataPersistence* getData();

		void save();

	private:

		struct PortInfo{
			MapLocation mapLocation;
			Player* p;
			TransitionEffect te;
		};

		bool readMainInfo(int & numMaps, int & numKeyItems, int & initLife, int & initMoney, vector<int> * tools, int & numPigeons);

		deque<ScreenMapConstructor*>* screenMapList;

		int width;
		int height;

		Player* players[4];
		int numPlayers;

		HUDController* hudController;
		ToolController* toolController;
		MessageController* messageController;
		EventController* eventController;

		DataBaseInterface* dbi;

		DataPersistence* data;
		ScreenMap* screenMap;
		GamePlayState* gamePlayState;

		// Estado del controlador
		State state;


		/* --------------
			   LOAD
		 -------------- */

		// loadMap(MapLocation int, (bloque de info a devolver ));
		bool loadScreen(MapLocation location);

		bool readEntities(FILE* file, map<int, Entity*>* screenEntities, map<int, GamePuzzle*>* screenPuzzles);
		bool readEnemies(FILE* file, vector<Enemy*>* screenEnemies, map<int, Entity*>* screenEntities);

		//! Construye el nombre de archivo a cargar a partir de una localizaci�n
		std::string getMapScreenFileName(MapLocation map);

		/* --------------
			TRANSITION
		 -------------- */
		// Efecto de la transici�n
		TransitionEffect transitionEffect;
		// Im�genes para la transici�n de mapas
		Image* currentRoom;
		Image* nextRoom;
		// Velocidad de la transici�n
		int speed;
		// Coordenadas origen de la transici�n
		int mx, my;
		// Coordenadas destino de la transici�n
		int tx, ty;
		// Direcci�n de la transici�n
		int xdir, ydir;

		// Listado din�mico de los portales y la cantidad de players en cada uno <idport, cont>
		map<PortInfo, int>* active_teleports;

		void endTransition();

		enum EntityTypes { entDoor, entBossDoor, Item,  entTiledEntity, DmgBlockade, entTiledPushable, entFloorButton, entInstantiator, entAbreDoors, entArena, entTeleporter, PickableTool, entDoorCloser };
};


#endif
