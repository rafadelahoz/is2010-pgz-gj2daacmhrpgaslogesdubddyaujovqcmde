#pragma once

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__


#include "Player.h";
#include "GameData.h";
#include "ScreenMap.h";
#include "HelperTypes.h";
#include "iInteractable.h";

#include <map>;

// Distancia (en p�xeles) m�nima necesaria entre el player y una entidad interactuable
// para que el player interact�e con ella al pulsar un bot�n
#define D_INTERACT 3;

class Controller : public Entity {

		enum State {NORMAL, TRANSITION};
		enum TransitionEffect {SCROLL, FADE};

		struct PortInfo{
			MapId mapId;
			Player* p;
			TransitionEffect te;
		};

	private:
		
		// DATOS EXTERNOS

		// Alto y ancho del juego
		int width;
		int height;
		
		(Player*)* players;
		int numPlayers;

		Entity* hud;

		GameData* data;

		// mapa actual
		ScreenMap* screenMap;


		// DATOS PROPIOS

		HUDController* hudController;
		ToolController* toolController;



		// Estado del controlador
		State state;
		

		/* --------------
			   LOAD
		 -------------- */

		// Carga una partida guardada, actualizando DATA
		bool load_slot(std:string slotpath);

		// loadMap(mapId int, (bloque de info a devolver ));
		// loadScreen(int id o cosa, (bloque de info a devolver)); <- o lo mete directamente en gamestate y screenmap




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
		map<portInfo, int>* active_teleports;
		
	public:
		// CONSTRUCORES Y DESTRUCTORES
		Controller(std::string path, Game* g, GameState* gs);
		~Controller();
		
		// M�TODOS PERTENECIENTES A ENTITY
		void onStep();
		void onRender();

		void onTimer(int timer);

		// M�TODOS PROPIOS
		// Iniliza lo necesario para el funcionamiento del gameplay
		bool init(std::string path);

		// Cambio de pantalla dentro del mismo mapa
		bool move_to_screen(Dir dir);

		// El player p solicita un cambio al mapa m con una transici�n te
		bool change_map(GameData::MapId m, Player* p, TransitionEffect te, bool brute = false);

		void attack(int idtool, Player* player);
		
		void setState(State st);
		void setTransitionEffect(TransitionEffect te);
};


#endif