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
#ifndef _COLTESTER_H_
#define _COLTESTER_H_

#include "Entity.h"
#include "Stamp.h"
#include "Player.h"

class CollisionTester : public Entity
{
private:
	int xsp;
public:
	CollisionTester(int x, int y, Game* g, GameState* gs) : Entity(x, y, g, gs)
	{
		mask = new MaskBox(0, 0, 16, 16, "coltest");
		graphic = new Stamp("data/graphics/coltest.png", g->getGfxEngine());

		xsp = -(rand()%4+2);

		depth = y;
	};

	void onStep()
	{
		x += xsp;
		if (x < -mask->width) instance_destroy();
	};

	void onCollision(CollisionPair pair, Entity* other)
	{
		if (pair.b == "solid1")
		{
			instance_destroy();
		}
		else if (pair.b == "player")
		{
			((Player*) other)->setLastHitDirection(((Player*) other)->computeHitDirection(this, other));
			((Player*) other)->onDamage(5, 0xFF);
			/*Direction d;
			int ocx, ocy, mcx, mcy, vunit, hunit;

			mcx = x+mask->xoffset;
			mcy = y+mask->yoffset;

			ocx = other->x+other->mask->xoffset+(other->mask->width/2);
			ocy = other->y+other->mask->yoffset+(other->mask->height/2);

			vunit = mask->height/3;
			hunit = mask->width/3;

			if (ocx < mcx+hunit)
			{
				if (ocy < mcy+vunit) d = DOWNRIGHT;
				else if (ocy >= mcy+vunit && ocy < mcy+vunit*2) d = RIGHT;
				else d = UPRIGHT;
			}
			else if (ocx >= mcx+hunit && ocx < mcx+hunit*2)
			{
				if (ocy < mcy+vunit) d = DOWN;
				else if (ocy >= mcy+vunit && ocy < mcy+vunit*2) d = NONE;
				else d = UP;
			}
			else
			{
				if (ocy < mcy+vunit) d = DOWNLEFT;
				else if (ocy >= mcy+vunit && ocy < mcy+vunit*2) d = LEFT;
				else d = UPLEFT;
			}

			((Player*) other)->setLastEnemyDirection(d);
			((Player*) other)->onDamage(5, 0x1);*/
		}
	};
};

#endif