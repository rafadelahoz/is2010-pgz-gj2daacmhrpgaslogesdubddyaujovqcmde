#pragma once
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Player.h"
#include "GameData.h"
#include "ScreenMap.h"
#include "HelperTypes.h"
//#include "iInteractable.h"

// HERE THERE BE SHIT

#define D_INTERACT 3;

class iInteractable {
	public:
		void onInteract(Player* p){};
};



// STOP (HAMMERTIME!)

class Controller : public Entity {

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
		enum State {NORMAL, TRANSITION};
		enum TransitionEffect {SCROLL, FADE};

		// Estado del controlador
		State state;
		

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
		
	public:
		// CONSTRUCORES Y DESTRUCTORES
		Controller(std::string path, Game* g, GameState* gs);
		~Controller();
		
		// M�TODOS PERTENECIENTES A ENTITY
		void onStep();
		void onRender();

		// M�TODOS PROPIOS
		// Iniliza lo necesario para el funcionamiento del gameplay
		bool init(std::string path);

		// Cambio de pantalla dentro del mismo mapa
		bool change_screen(Dir dir);

		// El player p solicita un cambio al mapa m con una transici�n te
		bool change_map(GameData::MapId m, Player* p, TransitionEffect te);
		void attack(int idtool, Player* player);
		
		void setState(State st);
		void setTransitionEffect(TransitionEffect te);

		/*
		// m�todos de an�lisis
		// Avisan al HUD controller de que deben cambiar el correspondiente datos
		void hud_modLife(player, life);
		void hud_modMagic(player, magic);
		void hud_modMoney(player, money);
		void hud_modSelectedTool(player, tool, ammo?);
		void hud_modPlayers();

		*/
};


#endif