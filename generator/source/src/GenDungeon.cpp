#include "GenDungeon.h"

GenDungeon::GenDungeon() {
	dungeons = new vector<Dungeon*>();
}

GenDungeon::~GenDungeon() {
	for (vector<Dungeon*>::iterator it = dungeons->begin(); it < dungeons->end(); it++)
		delete *it;
	delete dungeons; dungeons = NULL;
}

Dungeon* GenDungeon::createDungeon(string zone, string theme, short gameDiff, short dungNumber, short ratio, short tool, short keyObj, DungeonPos dungeonPos, DBManager* db) {
	Dungeon* d = NULL;
	if (rand() % 2 == 0) /////////////////CUIDADO! SOLO SE HACEN DE TIPO JAVI !!!!!!!!!!!! /////////////////////////////////////////////////////////////////
		d = new DungeonJ(zone, theme, gameDiff, dungNumber, ratio, tool, keyObj, dungeonPos, db);
	else
		d = new DungeonJ(zone, theme, gameDiff, dungNumber, ratio, tool, keyObj, dungeonPos, db);

	d->generate();
	dungeons->push_back(d);
	return d;
}

Dungeon* GenDungeon::getDungeon(int i) {
	if (i < 0 || i >= dungeons->size()) return NULL;
	
	return dungeons->at(i);
}

int GenDungeon::getNumDungeons()
{
	return dungeons->size();
}

void GenDungeon::save() {
	for (vector<Dungeon*>::iterator it = dungeons->begin(); it < dungeons->end(); it++)
		(*it)->save();
}