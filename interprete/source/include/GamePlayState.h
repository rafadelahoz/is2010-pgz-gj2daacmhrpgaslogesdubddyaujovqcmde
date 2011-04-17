#pragma once
#ifndef __GAMEPLAYSTATE_H__
#define __GAMEPLAYSTATE_H__

#include <list>

#include "GameState.h"
#include "controller.h"
#include "TileTextLabel.h"
#include "ScreenMap.h"

class Controller;

class GamePlayState : public GameState
{
	friend class Controller;

	protected:

		list<Entity*>* localEntities;

        bool _add(Entity* e);
        bool _remove(Entity* e);
		
	public:

		// Constructora
		GamePlayState(int w, int h, Game* g);

		// Destructora
		~GamePlayState();

		// Sobrecarga del m�todo del padre para utilizar la lista de eliminables
		bool add(Entity* e, bool local = true);

		// Envoltorio del m�todo del padre para utilizar la lista de eliminables
		// que comprueba automaticamente si la entidad est� en deathRow
		bool removeLocals();

		// Temporal
		void renderBG();

		void onStep();
};

#endif