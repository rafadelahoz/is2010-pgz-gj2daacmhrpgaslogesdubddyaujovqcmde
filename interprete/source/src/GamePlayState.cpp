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

#include "GamePlayState.h"
#include "CollisionTester.h"
#include "TiledPushable.h"
#include "GameItem.h"
#include "TiledEntity.h"
#include "NPC.h"
#include "FireBall.h"

GamePlayState::GamePlayState(int w, int h, Game* g) : GameState(g, w, h)
{
	localEntities = new list<Entity*>();

	offsetX = 0;
	offsetY = 16;

	hudController = NULL;
};

GamePlayState::~GamePlayState()
{
	// Ya se ocupa la destructora padre de borrarlas
	delete localEntities;
};

// Sobrecarga del m�todo padre para notificar al mapa de cambios
bool GamePlayState::_add(Entity* e)
{
	((ScreenMap*) map)->notify(ScreenMap::eCREATE, e);

	return GameState::_add(e);
}

// Sobrecarga del m�todo padre para notificar al mapa de cambios
bool GamePlayState::_remove(Entity* e)
{
	((ScreenMap*) map)->notify(ScreenMap::eDESTROY, e);
	localEntities->remove(e);

	return GameState::_remove(e);
}

// Sobrecarga del m�todo del padre para utilizar la lista de entidades locales
bool GamePlayState::add(Entity* e, bool local)
{
	if (e != NULL)
	{
		if (local)
			add_single(localEntities, e);

		return GameState::add(e);
	}

	return false;
};

bool GamePlayState::addLocal(Entity* e)
{
	return add_single(localEntities, e);
};

bool GamePlayState::add(Entity* e)
{
	return GamePlayState::add(e, true);
};

bool GamePlayState::remove(Entity* e)
{
	if (e != NULL)
	{
		int oldsize = localEntities->size();
		localEntities->remove(e);
		GameState::remove(e);
		return oldsize > localEntities->size();
	}
	return false;
};

// Sobrecarga del m�todo del padre para utilizar la lista de eliminables
bool GamePlayState::removeLocals()
{
	list<Entity*>::iterator it = localEntities->begin();
	while (it != localEntities->end())
	{
        if ((*it) != NULL)
        {
			remove(*it);
        }
		it = localEntities->begin();
	}

	return (localEntities->size() == 0);
};

// Sobrecarga del m�todo del padre para utilizar la lista de eliminables
void GamePlayState::hideLocals()
{
	for(list<Entity*>::iterator it = localEntities->begin();  it != localEntities->end(); it++)
	{
        if ((*it) != NULL)
        {
			(*it)->setVisible(false);
        }
	}
};

void GamePlayState::renderBG()
{
	// Se dibuja un rect�ngulo para pruebas de gr�ficos de forma temporal
//	game->getGfxEngine()->renderRectangle(0, 0, roomw, roomh, Color(255, 20, 20));
};

void GamePlayState::renderFG()
{
	// Huds y tal
	if (offsetX == 0 && offsetY == 16)
	{
		game->getGfxEngine()->renderRectangle(0, 0, roomw, 16, Color(50, 50, 50));
		game->getGfxEngine()->renderRectangle(0, roomh-16, roomw, 16, Color(50, 50, 50));

		if (hudController != NULL)
			hudController->onRender();
	}
};

void GamePlayState::onStep()
{
	if (hudController != NULL)
		hudController->onStep();
};

void GamePlayState::onRender()
{
    // Si se desea, se pintar�n elementos por debajo de los dem�s
    renderBG();

    // Si hay mapa, se pinta
    if (map != NULL)
        map->render(offsetX,offsetY);

    list<Entity*>::iterator i;
    // Pintamos todas las entidades visibles
	if (renderable->size() > 0)
		for(i = renderable->begin(); i != renderable->end(); i++)
			if ((*i) != NULL)
			{
				(*i)->x += offsetX;
				(*i)->y += offsetY;
				(*i)->onRender();
				(*i)->x -= offsetX;
				(*i)->y -= offsetY;
			}

    // Si se desea, se pintar�n elementos por encima de los dem�s
    renderFG();
};

void GamePlayState::setOffset(int x, int y)
{
	offsetX = x;
	offsetY = y;
};

std::pair<int, int> GamePlayState::getOffset()
{
	return make_pair(offsetX, offsetY);
}

void GamePlayState::pauseGameEntities()
{
	GameEntity* ent;
	std::list<Entity*>::iterator it = entities->begin();
	while (it != entities->end())
	{
		if (ent = dynamic_cast<GameEntity*>((*it)))
			ent->pause();
		it++;
	}
};

void GamePlayState::unpauseGameEntities()
{
	GameEntity* ent;
	std::list<Entity*>::iterator it = entities->begin();
	while (it != entities->end())
	{
		if (ent = dynamic_cast<GameEntity*>((*it)))
			ent->unpause();
		it++;
	}
};

Map* GamePlayState::getMap()
{
	return map;
}