#pragma once

#ifndef __TOOLBOOMERANG_H__
#define __TOOLBOOMERANG_H__

#include "SpriteMap.h"
#include "Tool.h"

class Tool;

class ToolBoomerang : public Tool
{
private:
	// ------------------------------- Atributos ------------------------------------
	int speed;	// velocidad del boomerang
	int range;	// tiempo que avanza
	bool launched;
	Direction dir;	// direcci�n en la que ha sido lanzado

	// ---------------------------- M�todos auxiliares ----------------------------------
	bool loadConfig(std::string graphicpath, std::string fname);
	void move();

public:
	ToolBoomerang(int x, int y, Game* game, GameState* world);
	~ToolBoomerang();

	virtual void activate();

	// La herramienta act�a en el momento en el que se crea
	void onInit();

	virtual void init(bool passive, Player* p, int idTool, int damage, short damageType, std::string graphicpath);

	// realizmos el movimiento de la munici�n
	virtual void onStep();

	// Har� da�o (o no) y se destruir� en el momento en que colisione con algo
	virtual void onCollision(CollisionPair other, Entity* e);

	//virtual void onDestroy();

};

#endif