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

#include "ComponentAnim.h"

ComponentAnim::ComponentAnim(Game* game, Enemy* e, std::string gfxPath)
{
	this->e = e;
	this->game = game;
	this->gfxPath = gfxPath;

	height = 0;

	loadAnimations(getConfigurationFileName(gfxPath));
	((SpriteMap*) e->graphic)->playAnim(getAnimName(STAND, DOWN));
};

ComponentAnim::~ComponentAnim()
{
	if (e->graphic != NULL)
		delete e->graphic, e->graphic = NULL;
};

void ComponentAnim::onCRender()
{
	e->y -= height;

	e->Entity::onRender();

	e->y += height;
};

void ComponentAnim::onCStep()
{
	((SpriteMap*) e->graphic)->playAnim(getAnimName(e->currentAnim, e->dir));
};


std::string ComponentAnim::getAnimName(StandardEnemyAnimation anim, Direction dir)
{
	if (dir == NONE) dir = e->dir;
	// Se obtiene el nombre de la animaci�n a partir del enum
	std::map<std::pair<StandardEnemyAnimation, Direction>, std::string>::iterator it;
	it = animList.find(make_pair(anim, dir));
	// Si el iterador alcanca el final de la lista, no est� la anim
	if (it == animList.end())
		return "";
	else
		return (*it).second;
};


bool ComponentAnim::playAnim(StandardEnemyAnimation anim, int speed, Direction dir)
{
	if (dir == NONE)
		dir = e->dir;
	// Si la animaci�n no existe, seguro que no se puede
	std::string name = getAnimName(anim, dir);
	if (name == "")
		return false;

	// Si la animaci�n no tiene datos, algo va mal
	EnemyAnimData data = getAnimationData(anim, dir);
	if (data.numFrames < 0)
		return false;

	if (speed < 0)
		speed = data.animSpeed;

	// 1. Comprobaci�n de estado actual: �permite manipulaci�n?
	if (e->currentAnim != NOTHING)
		return false;
	// 2. Establecer nueva animaci�n
	((SpriteMap*) e->graphic)->playAnim(name, speed, false, true);
	e->currentAnim = anim;

	return true;
};

std::string ComponentAnim::getConfigurationFileName(std::string fname)
{
	// El fname debe tener extensi�n
	if (fname.length() < 5)
		return "";

	std::string cfgname = fname;
	// Se quita la extensi�n del fname (.png)
	for (int i = 0; i < 3; i++)
		cfgname.pop_back();
	cfgname.append("cfg");

	return cfgname;
};

bool ComponentAnim::loadAnimations(std::string fname)
{
	int maskW = 0, maskH = 0, xoffset = 0, yoffset = 0;
	int ncol = 0, nrow = 0;

	// Carga el archivo de config y lee
	FILE* f = fopen(fname.c_str(), "rb");

	// Si el archivo es inv�lido, no se puede hacer nada
	if (f == NULL)
		return false;

	// 0. Columnas y filas
	if (fscanf(f, "%d %d", &ncol, &nrow) < 2)
		return false;


	// 1. Dimensiones de la m�scara
	if (fscanf(f, "%d %d %d %d", &maskW, &maskH, &xoffset, &yoffset) < 2)
		return false;

	e->mask = new MaskBox(e->x, e->y, maskW, maskH, "enemy", xoffset, yoffset);
	setShadow(maskW);
	e->graphic = new SpriteMap(gfxPath, ncol, nrow, game->getGfxEngine());
	//SpriteMap* gfx = ((SpriteMap*) e->graphic);
	// 2. Leer todas las animaciones
	// Stand
	loadAnimation(STAND, UP, "stu", f);
	loadAnimation(STAND, DOWN, "std", f);
	loadAnimation(STAND, LEFT, "stl", f);
	loadAnimation(STAND, RIGHT, "str", f);

	// Walk
	loadAnimation(WALK, UP, "wau", f);
	loadAnimation(WALK, DOWN, "wad", f);
	loadAnimation(WALK, LEFT, "wal", f);
	loadAnimation(WALK, RIGHT, "war", f);

	// AtkMelee
	loadAnimation(ATKMELEE, UP, "amu", f);
	loadAnimation(ATKMELEE, DOWN, "amd", f);
	loadAnimation(ATKMELEE, LEFT, "aml", f);
	loadAnimation(ATKMELEE, RIGHT, "amr", f);

	// AtkRanged
	loadAnimation(ATKRANGED, UP, "aru", f);
	loadAnimation(ATKRANGED, DOWN, "ard", f);
	loadAnimation(ATKRANGED, LEFT, "arl", f);
	loadAnimation(ATKRANGED, RIGHT, "arr", f);

	// AtkSpecial
	loadAnimation(ATKSPECIAL, UP, "asu", f);
	loadAnimation(ATKSPECIAL, DOWN, "asd", f);
	loadAnimation(ATKSPECIAL, LEFT, "asl", f);
	loadAnimation(ATKSPECIAL, RIGHT, "asr", f);

	// Damaged
	loadAnimation(DAMAGED, UP, "dmu", f);
	loadAnimation(DAMAGED, DOWN, "dmd", f);
	loadAnimation(DAMAGED, LEFT, "dml", f);
	loadAnimation(DAMAGED, RIGHT, "dmr", f);

	// Dead
	loadAnimation(DEAD, UP, "deu", f);
	loadAnimation(DEAD, DOWN, "ded", f);
	loadAnimation(DEAD, LEFT, "del", f);
	loadAnimation(DEAD, RIGHT, "der", f);

	fclose(f);

	return true;
};

bool ComponentAnim::loadAnimation(StandardEnemyAnimation anim, Direction direction, std::string name, FILE* from)
{
	// Cargar animaci�n indicada de from
	if (from == NULL || name == "")
		return false;

	int numFrames, speed;
	int* frameList;
	EnemyFrameData frameData;
	EnemyAnimData animData;

	// Leemos datos de la animaci�n

	// 0.Speed
	if (fscanf(from, "%d", &speed) < 1)
		return false;

	// 1.N� frames
	if (fscanf(from, "%d", &numFrames) < 1)
		return false;

	animData.animSpeed = speed;
	animData.numFrames = numFrames;

	// Se instancia el contenedor de frames
	frameList = new int[numFrames];

	// Se carga cada frame
	for (int i = 0; i < numFrames; i++)
	{
		frameData = loadAnimationFrame(from);
		frameList[i] = frameData.frameId;
		animData.frameData.push_back(frameData);
	}

	// Se a�ade la animaci�n al graphic con el nombre indicado
	SpriteMap* gfx = ((SpriteMap*) e->graphic);
	gfx->addAnim(name, frameList, numFrames, speed, true);

	// Se mappea la anim
	animList.insert(make_pair(make_pair(anim, direction), name));
	// Y se agrega su info
	animDataList.insert(make_pair(make_pair(anim, direction), animData));
	delete frameList;

	return true;
};

ComponentAnim::EnemyFrameData ComponentAnim::loadAnimationFrame(FILE* from)
{
	// Se carga un frame de from
	// Se prepara un frame vac�o
	EnemyFrameData fd;
	fd.frameId = 0;
	fd.collisionMask.offsetX = 0;
	fd.collisionMask.offsetY = 0;
	fd.collisionMask.width = 0;
	fd.collisionMask.height = 0;
	fd.hotspotX = 0;
	fd.hotspotY = 0;

	// El archivo debe ser v�lido
	if (from == NULL)
		return fd;

	// Se lee el frameId
	if (fscanf(from, "%d", &fd.frameId) < 1)
		return fd;

	// Se lee el hotspot
	if (fscanf(from, "%d %d", &fd.hotspotX, &fd.hotspotY) < 1)
		return fd;

	// Y por ahora ya
	return fd;
};

ComponentAnim::EnemyAnimData ComponentAnim::getAnimationData(StandardEnemyAnimation anim, Direction dir)
{
	if (dir == NONE) dir = e->dir;
	// Se obtiene el nombre de la animaci�n a partir del enum
	std::map<std::pair<StandardEnemyAnimation, Direction>, EnemyAnimData>::iterator it;
	it = animDataList.find(make_pair(anim, dir));
	// Si el iterador alcanca el final de la lista, no est� la anim
	if (it == animDataList.end())
	{
		EnemyAnimData a; a.animSpeed = -1; a.numFrames = -1;
		return a;
	}
	else
		return (*it).second;
};

void ComponentAnim::setHeight(int h)
{
	height = h;
};

int ComponentAnim::getHeight()
{
	return height;
};

void ComponentAnim::setShadow(int width)
{
	if (width <= 8)
		e->initShadow(GameEntity::sNone);
	else if (width <= 16)
		e->initShadow(GameEntity::sSmall);
	else if (width < 20)
		e->initShadow(GameEntity::sMedium);
	else
		e->initShadow(GameEntity::sBig);
}

std::string ComponentAnim::getGfxPath()
{
	return gfxPath;
}