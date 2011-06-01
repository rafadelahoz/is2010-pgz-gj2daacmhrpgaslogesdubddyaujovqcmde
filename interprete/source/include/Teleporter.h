/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by working on it and helping it be better or by playing it in it's actual state	//																			
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 1, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA					//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

#pragma once
#ifndef _TELEPORTER_H_
#define _TELEPORTER_H_

#include "Entity.h"
#include "Stamp.h"
#include "PGZGame.h"

class Teleporter : public Entity
{
public:

	enum TeleportType {STEP, INSIDE, UP, DOWN, LEFT, RIGHT, DUNGEON, DUNGEONFINAL};

	TeleportType teleportType;
	TransitionEffect transitionEffect;

	MapLocation destination;

	Teleporter(MapLocation m, int x, int y, Game* g, GameState* gs, int w = 16, int h = 16);

	~Teleporter();

	void onCollision(CollisionPair pair, Entity* other);

	void setTeleportType(TeleportType type);
	void setTransition(TransitionEffect te);

	bool isInside(Mask* mask);

	void onRender();
};

#endif