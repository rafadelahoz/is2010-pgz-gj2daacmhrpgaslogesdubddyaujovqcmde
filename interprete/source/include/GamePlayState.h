#pragma once
#ifndef __GAMEPLAYSTATE_H__
#define __GAMEPLAYSTATE_H__

#include <list>

#include "GameState.h"
#include "controller.h"

class GamePlayState : public GameState
{
private:
	// Controller & friends here
	Controller* controller;
	// Lista de Entidades a borrar y eliminar bajo una transici�n
	std::list<Entity*> deathRow;

	// Comprueba si la entidad pertenece al deathRow
	bool isCondemned(Entity* e);
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
};

#endif