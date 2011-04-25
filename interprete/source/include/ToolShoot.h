#pragma once

#ifndef __TOOLSHOOT_H__
#define __TOOLSHOOT_H__

#include "Tool.h"
#include "ToolController.h"

class ToolShoot : public Tool
{
private:
	// Carga todas las animaciones de una imagen
	/*	graphicpath: ruta de la imagen
		fname: fichero de configuraci�n de la imagen */
	bool loadAnimations(std::string graphicpath, std::string fname);

public:
	ToolShoot(int x, int y, Game* game, GameState* world);
	~ToolShoot();

	void activate();

	// La herramienta act�a en el momento en el que se crea
	void onInit();

	void init(bool passive, Player* p, Player::PlayerAnim playeranim, int idTool, std::string graphicpath);

	void onInitStep(){};
	void onRender(){};
	void onEndStep(){};
	void onCollision(){};

};

#endif