#include <iostream>
#include "World.h"
#include "GenTypes.h"
#include "BDInterface.h"

// Cosas que nos dara decidator:
//------------------------------------------------
int wSize = 5;
vector<ZoneInfo>* zonesI;
vector<DungeonInfo>* dungeonsI;
vector<SafeZoneInfo>* safeZonesI;
//------------------------------------------------

int main(int argc, char *argv[])
{
// Procesar argumentos...
/* 
for(int i=1; i<argc; i++)
{
	cosa = argv[i];
}
*/
	BDInterface* myDB = new BDInterface();

	zonesI = new vector<ZoneInfo>();
	dungeonsI = new vector<DungeonInfo>();
	safeZonesI = new vector<SafeZoneInfo>();

	int diff;
	
	if (argc > 2)
		diff = (int)argv[1];
	else 
		diff = 1;
		
	World* w = new World(diff);
	// TODO: poner algo en los ****Info <----
	Overworld* ow = new Overworld(wSize, zonesI, dungeonsI, safeZonesI);
	w->setOverworld(ow);
	
	GenOverworld* gow = new GenOverworld(ow);
	gow->genFrontiers();
	gow->genShape();
	gow->genGeoDetail();
	gow->genDecoration(myDB);
	gow->placeDungeons();
	gow->placeSafeZones();
	gow->genMainRoad();
	gow->genRoadRamifications();
	gow->genBlockades();
	
	gow->genScreens(); // deberia llamar a un genScreens de Zone por cada zona que tenga
	
	// TODO: terminar ...

	cin.peek();
	return 0;
}
