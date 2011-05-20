#include "World.h"

// Constructora
World::World(int difficulty, GenOverworld* genOverw, DBManager* myDataB){
	// Asignamos dificultad
	gameDiff = difficulty;
	genOw = genOverw;
	myDB = myDataB;			// A�ADIDO DB PARA INVOCACION DE GENDECORATION
}

// Destructora
World::~World(){
	overworld = NULL;
	genOw = NULL;
	myDB = NULL;
}

// Setter Overworld
void World::setOverworld(Overworld* ow){
	overworld = ow;
}

// Construcci�n del overworld
void World::buildOverworld(){
	genOw->genFrontiers();
	genOw->genShape();
	genOw->assignTilesAndScreens();
	genOw->genGeoDetail();
	genOw->placeDungeons();
	genOw->placeFinalDungeon();
	genOw->placeSafeZones();
	genOw->genMainRoad();
	genOw->genRoadRamifications();
	genOw->genBlockades();
	genOw->placePowUPandPigeons();
	genOw->placeNPCs();
	genOw->genDecoration(myDB);
	genOw->genScreens();
	genOw->guardameSolids("solids.txt");
	genOw->guardameZonas("Zones.txt");
	genOw->guardamePant("Pant.txt");
	genOw->guardameAutotile("Auto.txt");
}