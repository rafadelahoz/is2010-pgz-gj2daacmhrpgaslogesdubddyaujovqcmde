#include "Entity.h"

Entity::Entity(short type, short x, short y, short idCollectable, short linkedTo) {
	this->type = type;
	this->x = x;
	this->y = y;
	this->idCollectable = idCollectable;
	this->linkedTo = linkedTo;
}

EntityItem::EntityItem(short type, short x, short y, short idCollectable, short linkedTo, short gfxId, short effect, short power) :
	Entity(type, x, y, idCollectable, linkedTo) {
	this->gfxId = gfxId;
	this->effect = effect;
	this->power = power;
}

EntityTiled::EntityTiled(short type, short x, short y, short idCollectable, short linkedTo, short tile, short terrainType) :
	Entity(type, x, y, idCollectable, linkedTo) {
	this->tile = tile;
	this->terrainType;
}

EntityDoor::EntityDoor(short type, short x, short y, short idCollectable, short linkedTo, short idTile) :
	Entity(type, x, y, idCollectable, linkedTo) {
	this->idTile = idTile;
}

EntityBossDoor::EntityBossDoor(short type, short x, short y, short idCollectable, short linkedTo, short idTile) :
	Entity(type, x, y, idCollectable, linkedTo) {
	this->idTile = idTile;
}

EntityDmgBlockade::EntityDmgBlockade(short type, short x, short y, short idCollectable, short linkedTo, short idGfx, short typeB, short idDmg) :
	Entity(type, x, y, idCollectable, linkedTo) {
	this->idGfx = idGfx;
	this->typeB = typeB;
	this->idDmg = idDmg;
}

EntityTiledPushable::EntityTiledPushable(short type, short x, short y, short idCollectable, short linkedTo, short idGfx) :
	Entity(type, x, y, idCollectable, linkedTo) {
	this->idGfx = idGfx;
}

EntityFloorButton::EntityFloorButton(short type, short x, short y, short idCollectable, short linkedTo, short idGfx) :
	Entity(type, x, y, idCollectable, linkedTo) {
	this->idGfx = idGfx;
}

EntityTeleporter::EntityTeleporter(short type, short x, short y, short idCollectable, short linkedTo, short idMap, short screenX, short screenY, short tileX, short tileY) :
	Entity(type, x, y, idCollectable, linkedTo) {
	this->idMap = idMap;
	this->screenX = screenX;
	this->screenY = screenY;
	this->tileX = tileX;
	this->tileY = tileY;
}

EntityTool::EntityTool(short type, short x, short y, short idCollectable, short linkedTo, short idTool) :
	Entity(type, x, y, idCollectable, linkedTo) {
	this->idTool = idTool;
}

Entity::~Entity() {}

EntityItem::~EntityItem() {}

EntityTiled::~EntityTiled() {}

EntityDoor::~EntityDoor() {}

EntityBossDoor::~EntityBossDoor() {}

EntityDmgBlockade::~EntityDmgBlockade() {}

EntityTiledPushable::~EntityTiledPushable() {}

EntityFloorButton::~EntityFloorButton() {}

EntityTeleporter::~EntityTeleporter() {}

EntityTool::~EntityTool() {}

bool Entity::save(FILE* file) {
	short buffer[5];
	buffer[0] = type;
	buffer[1] = x;
	buffer[2] = y;
	buffer[3] = idCollectable;
	buffer[4] = linkedTo;
	if (fwrite(buffer, sizeof(short), 5, file) < 0)
		return false;
	return true;
}

bool EntityItem::save(FILE* file) {
	if (!Entity::save(file)) return false;
	short buffer[3];
	buffer[0] = gfxId;
	buffer[1] = effect;
	buffer[2] = power;
	if (fwrite(buffer, sizeof(short), 3, file) < 0) 
		return false;
	return true;
}

bool EntityTiled::save(FILE* file) {
	if (!Entity::save(file)) return false;
	short buffer[2];
	buffer[0] = tile;
	buffer[1] = terrainType;
	if (fwrite(buffer, sizeof(short), 2, file) < 0) 
		return false;
	return true;
}

bool EntityDoor::save(FILE* file) {
	if (!Entity::save(file)) return false;
	short buffer[1];
	buffer[0] = idTile;
	if (fwrite(buffer, sizeof(short), 1, file) < 0) 
		return false;
	return true;
}

bool EntityBossDoor::save(FILE* file) {
	if (!Entity::save(file)) return false;
	short buffer[1];
	buffer[0] = idTile;
	if (fwrite(buffer, sizeof(short), 1, file) < 0) 
		return false;
	return true;
}

bool EntityDmgBlockade::save(FILE* file) {
	if (!Entity::save(file)) return false;
	short buffer[3];
	buffer[0] = idGfx;
	buffer[1] = typeB;
	buffer[2] = idDmg;
	if (fwrite(buffer, sizeof(short), 3, file) < 0) 
		return false;
	return true;
}

bool EntityTiledPushable::save(FILE* file ) {
	if (!Entity::save(file)) return false;
	short buffer[1];
	buffer[0] = idGfx;
	if (fwrite(buffer, sizeof(short), 1, file) < 0) 
		return false;
	return true;
}

bool EntityFloorButton::save(FILE* file ) {
	if (!Entity::save(file)) return false;
	short buffer[1];
	buffer[0] = idGfx;
	if (fwrite(buffer, sizeof(short), 1, file) < 0) 
		return false;
	return true;
}

bool EntityTeleporter::save(FILE* file) {
	if (!Entity::save(file)) return false;
	short buffer[5];
	buffer[0] = idMap;
	buffer[1] = screenX;
	buffer[2] = screenY;
	buffer[3] = tileX;
	buffer[4] = tileY;
	if (fwrite(buffer, sizeof(short), 5, file) < 0) 
		return false;
	return true;
}

bool EntityTool::save(FILE* file) {
	if (!Entity::save(file)) return false;
	short buffer[1];
	buffer[0] = idTool;
	if (fwrite(buffer, sizeof(short), 1, file) < 0) 
		return false;
	return true;
}