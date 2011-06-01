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

#include "ToolMelee.h"


ToolMelee::ToolMelee(int x, int y, Game* game, GameState* world) : Tool(x, y, game, world)
{
}

ToolMelee::~ToolMelee(){};

void ToolMelee::init(bool passive, Player* p, int idTool, int damage, short damageType, std::string graphicpath)
{
	Tool::init(passive, p, idTool, damage, damageType);

	// cargamos las diferentes animaciones de la herramienta
	loadAnimations(graphicpath, getConfigurationFileName(graphicpath));
}

void ToolMelee::onInit()
{
	if (player->changeState(Player::Attack))
		activate();
	else
		// hay que avisar a toolController de que no se puede atacar
		player->getController()->getToolController()->toolFinished(idTool);
}

void ToolMelee::activate()
{
	ToolAnimData data;
	std::string name;
	Direction dir = player->getDir();

	// Ejecutamos la animaci�n correspondiente en funci�n de la direcci�n a la que mira el player
	switch(dir){
	case UP:
		name = "up";
		break;
	case DOWN:
		name = "down";
		break;
	case LEFT:
		name = "left";
		break;
	case UPLEFT:
		name = "left";
		break;
	case DOWNLEFT:
		name = "left";
		break;
	case RIGHT:
		name = "right";
		break;
	case UPRIGHT:
		name = "right";
		break;
	case DOWNRIGHT:
		name = "right";
		break;
	}

	data = animList.at(name);						// cogemos los datos de la animaci�n
	playAnim(name);									// ejecutamos la animaci�n
	player->playAnim(playeranim, data.animSpeed);	// Hacemos que el player ejecute la animaci�n

	placeTool();	// Colocamos el arma en funci�n de la animaci�n actual
}

void ToolMelee::onInitStep()
{
	// Si la herramienta a acabado, se lo indicamos a ToolController
	if (((SpriteMap*)graphic)->animFinished())
	{
		player->changeState(Player::Normal);
		player->getController()->getToolController()->toolFinished(idTool);
	}
}

void ToolMelee::onEndStep()
{
	placeTool();
}

void ToolMelee::onCollision(CollisionPair other, Entity* e)
{
	Tool::doDamage(other, e, player->getDir()); // hacemos da�o
	Tool::animOnCollision(other, e);			// Animaci�n a realizar al golpear con algo
}