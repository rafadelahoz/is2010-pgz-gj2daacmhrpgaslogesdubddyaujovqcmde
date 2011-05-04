#pragma once

#ifndef __TOOLSHOOT_H__
#define __TOOLSHOOT_H__

#include "ToolAmmo.h"
#include "Tool.h"

class Tool;

class ToolShoot : public Tool
{
private:

	// Munici�n de la herramienta de disparo
	ToolAmmo* ammo;
	std::string ammoGfxpath;

public:
	ToolShoot(int x, int y, Game* game, GameState* world);
	~ToolShoot();

	void activate();

	// La herramienta act�a en el momento en el que se crea
	void onInit();

	void init(bool passive, Player* p, int idTool, int damage, short damageType, std::string graphicpath, std::string ammoGfxpath);

	void onInitStep();
	void onEndStep();

};

#endif