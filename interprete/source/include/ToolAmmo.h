/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by working on it and helping it be better or by playing it in it's actual state	//																			
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 2, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.			//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

#pragma once

#ifndef __TOOLAMMO_H__
#define __TOOLAMMO_H__

#include "SpriteMap.h"
#include "Tool.h"

class Tool;

// ALEJANDROOOOOOOOOOOOOOOOOOOOOOOOOO

// ACU�RDATE DE HACER LA MASCARA ANTES DE NADAAAAAAAAAA QUE SINO PETA EL IS INBOUNDS!!!!



class ToolAmmo : public Tool
{
private:
	// ------------------------------- Atributos ------------------------------------
	int speed;	// velocidad de la munici�n
	Direction dir;	// direcci�n en la que ha sido lanzada la munici�n

	// ---------------------------- M�todos auxiliares ----------------------------------
	bool loadConfig(std::string graphicpath, std::string fname);

public:
	ToolAmmo(int x, int y, Game* game, GameState* world);
	~ToolAmmo();

	virtual void activate();

	// La herramienta act�a en el momento en el que se crea
	void onInit();

	virtual void init(bool passive, Player* p, int idTool, int damage, short damageType, std::string graphicpath, Direction dir);

	// realizmos el movimiento de la munici�n
	virtual void onStep();

	// Har� da�o (o no) y se destruir� en el momento en que colisione con algo
	virtual void onCollision(CollisionPair other, Entity* e);
};

#endif