#pragma once

#ifndef __TOOl_H__
#define __TOOL_H__

#include "GameEntity.h"
#include <string>

class Player;

class Tool: public GameEntity
{
protected:
	bool passive;	// true: herramienta pasiva, false: herramienta activa
	Player* player;		// Puntero al player

public:
	Tool(int x, int y, Game* game, GameState* world, string graphicPath);
	~Tool();

	// inicia la herramienta
	void init(bool passive, Player* p);

	// getter de si la herramienta es pasiva o no
	bool isPassive() {return passive;};
	
	// acci�n a realizar cuando se active la herramienta
	virtual void acivate() = 0;
	
};
#endif __TOOL_H__
