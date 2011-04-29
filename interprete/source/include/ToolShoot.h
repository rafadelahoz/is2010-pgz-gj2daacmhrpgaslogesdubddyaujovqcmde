#pragma once

#ifndef __TOOLSHOOT_H__
#define __TOOLSHOOT_H__

//#include "Tool.h"
#include "ToolController.h"
#include "ToolAmmo.h"

class ToolShoot : public Tool
{
private:

	// Munici�n de la herramienta de disparo
	ToolAmmo* ammo;

public:
	ToolShoot(int x, int y, Game* game, GameState* world);
	~ToolShoot();

	void activate();

	// La herramienta act�a en el momento en el que se crea
	void onInit();

	void init(bool passive, Player* p, int idTool, int damage, short damageType, std::string graphicpath);

	void onInitStep();
	void onEndStep();

};

#endif