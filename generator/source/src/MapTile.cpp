#include "MapTile.h"

// Constructora.
MapTile::MapTile(){
	tileId = -1;
	solid = -1;
	zoneNumber = -1;
	GPoint p;
	p.x = -1;
	p.y = -1;
	entity = new GenEntity();
	entity->dest = p;
	entity->type = Nothing;
}

// Destructora.
MapTile::~MapTile(){
	delete entity;
	entity = NULL;
}

// Getters.
int MapTile::getTileId(){
	return tileId;
}

int MapTile::getSolid(){
	return solid;
}

int MapTile::getZoneNumber(){
	return zoneNumber;
}

GenEntity* MapTile::getEntity(){
	return entity;
}

// Setters.
void MapTile::setTileId(int id){
	tileId = id;
}

void MapTile::setSolid(int sol){
	solid = sol;
}

void MapTile::setZoneNumber(int num){
	zoneNumber = num;
}

void MapTile::setEntity(GenEntity* ent){
	entity = ent;
}
