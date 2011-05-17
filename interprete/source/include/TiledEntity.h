#pragma once
#ifndef __TILEDENTITY_H__
#define __TILEDENTITY_H__

#include "Entity.h"
#include "Tileset.h"

class TiledEntity : public Entity
{
private:
	TileSet* tileset;
	short tileId;
	short* tilesType;
	short nTiles;
	short width;
	short height;
public:
	TiledEntity(int x, int y, Game* g, GameState* gs);

	~TiledEntity();

	void init(TileSet* tset, short tile, short* tilesType, short nTiles, short width);

	void onRender();
};

#endif