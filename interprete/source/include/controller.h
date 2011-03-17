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
		
		enum State {NORMAL, TRANSITION};
		enum TransitionEffect {SCROLL, FADE};

		// Estado del controlador
		State state;
		
		// Alto y ancho del juego
		int width;
		int height;
		
		(Player*)* players;
		Entity* hud;
		int numPlayers;

		GameData* data;
		// mapa actual
		ScreenMap* screenMap;
		
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
		Controller(Game* g, GameState* gs, int players);
		~Controller();
		
		// M�TODOS PERTENECIENTES A ENTITY
		void onStep();
		void onRender();

		// M�TODOS PROPIOS
		bool change_screen(Dir dir);
		bool change_map();
		void attack(int idtool, Player* player);
		
		void setState(State st);
		void setTransitionEffect(TransitionEffect te);
};


#endif