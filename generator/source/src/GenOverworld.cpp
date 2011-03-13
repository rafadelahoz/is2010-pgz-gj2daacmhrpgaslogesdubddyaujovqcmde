#include "GenOverworld.h"

GenOverworld::GenOverworld(Overworld* overworld)
{
	this->overworld = overworld;
	zones = new vector<Zone*>();
	blockadeVerts = new vector<set<Point> >();
	mainRoadVerts = new vector<Point>();
	interestingPoints = new vector<Point>();
}

GenOverworld::~GenOverworld()
{
	overworld = NULL;	// Se encargar� de destruirlo el que lo crea

	vector<Zone*>::iterator it;
    for(it = zones->begin(); it != zones->end(); it++)
        if ((*it) != NULL)
        {
			delete (*it);
			(*it) = NULL;
        }
	delete zones;
	zones = NULL;

	if (blockadeVerts != NULL)
	{
		blockadeVerts->clear();
		delete blockadeVerts;
		blockadeVerts = NULL;
	}

	if (mainRoadVerts != NULL)
	{
		mainRoadVerts->clear();
		delete mainRoadVerts;
		mainRoadVerts = NULL;
	}

	if (interestingPoints != NULL)
	{
		interestingPoints->clear();
		delete interestingPoints;
		interestingPoints = NULL;
	}
}



void GenOverworld::genFrontiers(){
	cout << "Ejecutando funci�n <GenOverworld::genFrontiers()>" << endl;
}

void GenOverworld::genShape(){
	cout << "Ejecutando funci�n <GenOverworld::genShape()>" << endl;
}

void GenOverworld::genGeoDetail(){
	cout << "Ejecutando funci�n <GenOverworld::genGeoDetail()>" << endl;
}

void GenOverworld::genDecoration(BDInterface myDB)
{
    // Esto se cambiar� en un futuro, de momento es para meter porqueria en la matriz
	vector<int>* candidatos = myDB.getTiles(1);
	int aux;
	srand ( time(NULL) );

	for (int i = 0; i < overworld->mapTileMatrix->size(); i++)
	{
		aux = rand() % 3;
		overworld->mapTileMatrix->at(i).setTileId(candidatos->at(aux));
	}
}

void GenOverworld::placeDungeons(){
	cout << "Ejecutando funci�n <GenOverworld::placeDungeons()>" << endl;
}

void GenOverworld::placeSafeZones(){
	cout << "Ejecutando funci�n <GenOverworld::placeSafeZones()>" << endl;
}

void GenOverworld::genMainRoad(){
	cout << "Ejecutando funci�n <GenOverworld::genMainRoad()>" << endl;
}

void GenOverworld::genRoadRamifications(){
	cout << "Ejecutando funci�n <GenOverworld::genRoadRamifications()>" << endl;
}

void GenOverworld::genBlockades(){
	cout << "Ejecutando funci�n <GenOverworld::genBlockades()>" << endl;
}
