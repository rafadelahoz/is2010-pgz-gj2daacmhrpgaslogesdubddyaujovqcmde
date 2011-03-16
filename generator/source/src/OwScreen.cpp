#include "OwScreen.h"

/*OwScreen::OwScreen(int iniT, int screenW, Overworld* ow, int screenH){
	screenNumber = screenH;

	matrix = new vector<MapTile>();

    int iniTile = iniT;
	for (int i = 0; i< screenH; i++){
        iniTile = iniT + 16*screenW*i;
	    for (int j = 0; j< screenW; j++){
            matrix->push_back(ow->mapTileMatrix->at(j+iniTile));
	    }
	}
}*/


//Constructora en la que nos pasan la MapTileMatrix y tenemos que meter nosotros los tiles.
OwScreen::OwScreen(int init, vector<MapTile*>* mapMatrix, int screenN, vector<MapTile*>* enemies, vector<MapTile*>* details){
	
	matrix = new vector<MapTile*>();

	int iniTile;
	for (int i = 0; i < screenHeight; i++)
	{
		iniTile = init + screenHeight*screenWidth*i;
		for (int j = 0; j < screenWidth; j++)
			matrix->push_back(mapMatrix->at(j+iniTile));
	}
	
    screenNumber = screenN;
	enemyList = enemies;
	detailsList = details;
}

//Constructora en la que nos pasan los tiles ya metidos, no necesitamos la MapTileMatrix
OwScreen::OwScreen(int init, int screenN, vector<MapTile*>* tiles, vector<MapTile*>* enemies, vector<MapTile*>* details){
	matrix = tiles;	
    screenNumber = screenN;
	enemyList = enemies;
	detailsList = details;
}

OwScreen::~OwScreen(){		// BORRADO MIRAR MEJOR
	delete matrix;
	matrix = NULL;
}

void OwScreen::placeDetails(){
	cout << "Ejecutando funci�n <OwScreen::placeDetails()>" << endl;
}

void OwScreen::placeEnemies(){
	cout << "Ejecutando funci�n <OwScreen::placeEnemies()>" << endl;
}

string OwScreen::createScreenFiles(){
	cout << "Ejecutando funci�n <OwScreen::createScreenFiles()> ya implementada" << endl;
	//Tengo que guardar:
	ofstream file;
	string path = "../screens/screen";
	path += screenNumber;
	path += ".screen";
	file.open(path.c_str(), ios::binary);

	if (file.good())
		cout << "Archivo abierto correctamente" <<endl;
	else
		cout << "Archivo abierto incorrectamente" <<endl;


	//Tile Inicial
	//file.write((char*)& iniTile, sizeof(int));
	//Tile Final
	//file.write((char*)& endTile, sizeof(int));
	//N�mero de Pantalla
	file.write((char*)& screenNumber, sizeof(int));

	//Tile por tile
	for (int i=0; i<matrix->size(); i++){
		MapTile* tile = matrix->at(i);
		//TileId
		file.write((char*) tile->getTileId(), sizeof(int));
		//Solid
		file.write((char*) tile->getSolid(), sizeof(int));
		//ZoneNumber
		file.write((char*) tile->getZoneNumber(), sizeof(int));
		//Entidad
        //Type
		file.write((char*) tile->getEntity()->type, sizeof(EntType));
        //DungeonPath
        writeInFile(tile->getEntity()->dungeonPath, file);
        //Dest
        file.write((char*) tile->getEntity()->dest.x, sizeof(int));
        file.write((char*) tile->getEntity()->dest.y, sizeof(int));
	}

	if (file.good())
		cout << "Archivo guardado correctamente" <<endl;
	else
		cout << "Archivo incorrectamente guardado" <<endl;

	file.close();

	return path;
}

void OwScreen::writeInFile(const string& cad, ofstream& sal)const{
    int size = cad.size();
    sal.write((char*)& size, sizeof(int));
    sal.write((char*) cad.data(), sizeof(char) * cad.size());
}

