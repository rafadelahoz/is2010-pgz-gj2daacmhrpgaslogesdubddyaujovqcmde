#pragma once
#ifndef __GAMEPLAYSTATE_H__
#define __GAMEPLAYSTATE_H__

#include <list>

#include "GameState.h"
#include "controller.h"
#include "TileTextLabel.h"

class Controller;

class GamePlayState : public GameState
{
	friend class Controller;

	private:
		TextLabel* anus;

	public:

		// Constructora
		GamePlayState(int w, int h, Game* g);

		// Destructora
		~GamePlayState();

		// Sobrecarga del m�todo del padre para utilizar la lista de eliminables
		bool add(Entity* e, bool condemned = true);

		// Envoltorio del m�todo del padre para utilizar la lista de eliminables
		// que comprueba automaticamente si la entidad est� en deathRow
		bool removeEntity(Entity* e);

		// Temporal
		void renderBG();

		void renderFG()
		{
			anus->render(0, 0);
		};
};

#endif