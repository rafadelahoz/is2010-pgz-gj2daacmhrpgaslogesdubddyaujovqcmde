#include "Overworld.h"

// Constructora.
Overworld::Overworld(int worldS, int wDiff, int numZones, int numDungeons, int numSafeZones){
	// Asignamos variables a atributos.
	worldSize = worldS;
	worldDiff = wDiff;
	this->numZones = numZones;
	this->numDungeons = numDungeons;
	this->numSafeZones = numSafeZones;

	mapTileMatrix = new vector<MapTile*>();

	// Calculamos un tama�o del mundo a partir de worldSize.
	tileWorldSizeH = SCREEN_HEIGHT * 15;// ((rand() % (5 * worldSize)) + 10); 	// Aseguramos un mundo de al menos 5 x 5 pantallas.
	tileWorldSizeW = SCREEN_WIDTH * 15;// ((rand() % (5 * worldSize)) + 10);

	// Almacenamos el n�mero de pantallas del mundo.
	worldSizeH = 15; 
	worldSizeW = 15; 

	// Inicializamos tileMapMatrix
	for (int i=0; i< (tileWorldSizeH*tileWorldSizeW); i++)
		mapTileMatrix->push_back(new MapTile());


	// ESTO HABRA QUE CAMBIARLO *************************
	startLocation.x = 0;
	startLocation.y = 0;
	n_puzzles = 0;
	n_collectables = 0;
	n_blockades = 0;
	screenList = new vector<OwScreen*>();
}


Overworld::~Overworld(){
	vector<MapTile*>::iterator it;
    for(it = mapTileMatrix->begin(); it != mapTileMatrix->end(); it++)
        if ((*it) != NULL)
        {
			delete (*it);
			(*it) = NULL;
        }
	delete mapTileMatrix;
	mapTileMatrix = NULL;

	vector<OwScreen*>::iterator it2;
	for(it2 = screenList->begin(); it2 != screenList->end(); it2++)
        if ((*it2) != NULL)
        {
			delete (*it2);
			(*it2) = NULL;
        }
	delete screenList;
	screenList = NULL;
}

bool Overworld::save()
{
	// Abrimos el archivo de mazmorra m<ID>h
	char fname[MAX_STR_LENGTH];
	sprintf(fname, "map/m%dh", 0); //por ahora solo un mapa mundi y le corresponde con el 0
	FILE* file = fopen (fname, "w");
	// Guardamos la informaci�n del mundo
	if (file != NULL) {
		// Guardamos el tipo de mapa 
		short type[1];
		type[0] = 0;	// Tipo mundo
		fwrite(type, sizeof(short), 1, file);

		// Guardamos la informaci�n del mundo
		short size[2];
		size[0] = worldSizeW;  //width
		size[1] = worldSizeH;  //height
		fwrite(size, sizeof(short), 2, file);	// ancho y alto del mundo en pantallas
		
		// layout
		// inicializamos el layout 
		bool** layout = new bool*[worldSizeW];
		for (int i = 0; i < worldSizeW; i++) {
			layout[i] = new bool[worldSizeH];
			for (int j = 0; j < worldSizeH; j++)
				layout[i][j] = true;
		}

		// Se utilizan todas las celdas del layout

		// guardamos el layout
		for (int i = 0; i < worldSizeW; i++)
			fwrite(layout[i], sizeof(bool), worldSizeH, file);

        // nos deshacemos de la memoria que hemos usado para guardar el layout
		for (int i = 0; i < worldSizeW; i++) {
			delete layout[i]; 
			layout[i] = NULL;
		}
		delete layout; layout = NULL;

		// guardamos la pantalla inicial del mundo 
		short iniLocation[2];
		iniLocation[0] = startLocation.x;
		iniLocation[1] = startLocation.y;
		fwrite(iniLocation, sizeof(short), 2, file);
		
		// informaci�n general del mundo
		short persistent[4];
		persistent[0] = n_puzzles;
		persistent[1] = n_collectables;
		persistent[2] = n_blockades; 
		persistent[3] = 0; // minibosses?
		fwrite(persistent, sizeof(short), 4, file);

		fclose(file);

		// informaci�n de las pantallas
		vector<OwScreen*>::iterator it;
		for (it = screenList->begin(); it < screenList->end(); it++)
		{
			//(*it)->generate();
			(*it)->save();
		}
		return true;
	}
	return false;
}

void Overworld::addScreen(OwScreen* screen)
{
	screenList->push_back(screen);
}

// Getters utiles:
int Overworld::getNumZones(){
	return numZones;
}

int Overworld::getNumDungeons(){
	return numDungeons;
}

int Overworld::getNumSafeZones(){
	return numSafeZones;
}

int Overworld::getWorldDiff(){
	return worldDiff;
}

GPoint Overworld::getStartLocation()
{
	return startLocation;
}

int Overworld::getTileWorldSizeH()
{
	return tileWorldSizeH;
}

int Overworld::getTileWorldSizeW()
{
	return tileWorldSizeW;
}

int Overworld::getWorldSizeH()
{
	return worldSizeH;
}

int Overworld::getWorldSizeW()
{
	return worldSizeW;
}

MapTile* Overworld::getMapTile(int x, int y)
{
    return mapTileMatrix->at(y*tileWorldSizeW + x);
}

MapTile* Overworld::getMapTile(int pos)
{
    return mapTileMatrix->at(pos);
}

/*******************************FUNCIONES A�ADIDAS PARA DEBUG*********************************************/
void Overworld::guardameSolids(string path){

	string fichero (path);
	ofstream f_lista (fichero.c_str());
	if (!f_lista) {
		cout << "El fichero " << fichero << " no existe.";
		exit (0);
	}

	for(int i = 0; i < tileWorldSizeH*tileWorldSizeW; i++){
		if( mapTileMatrix->at(i)->getTileId() == 222)
			f_lista << "g";
		else if( mapTileMatrix->at(i)->getSolid() == 0)
			f_lista << "�";
		else if( mapTileMatrix->at(i)->getSolid() == 2)
			f_lista << "#";
		else if( mapTileMatrix->at(i)->getSolid() == 3)
			f_lista << "p";
		else
			f_lista << "0";
		if((i+1) % SCREEN_WIDTH == 0)
			f_lista << "|";
		else
			f_lista << " ";
		if((i+1) % tileWorldSizeW == 0)
		{
			f_lista << endl;
			if(((i+1) / tileWorldSizeW) % SCREEN_HEIGHT == 0)
			{
				for(int j = 0; j < tileWorldSizeW-1; j++)
					f_lista << "--";
				f_lista << "-" << endl;
			}
		}
	}

	f_lista.close();
}

void Overworld::guardameZonas(string path){

	string fichero (path);
	ofstream f_lista (fichero.c_str());
	if (!f_lista) {
		cout << "El fichero " << fichero << " no existe.";
		exit (0);
	}

	for(int i = 0; i < tileWorldSizeH*tileWorldSizeW; i++){
		if ( mapTileMatrix->at(i)->getZoneNumber() == 0 )
			f_lista << "*";
		else if (mapTileMatrix->at(i)->getTileId() == 100 )
			f_lista << "B";
		else if (mapTileMatrix->at(i)->getTileId() == 100 )
			f_lista << "M";
		else if (mapTileMatrix->at(i)->getTileId() == 100 )
			f_lista << "_";
		else if ( mapTileMatrix->at(i)->getSolid() == 3 )
			f_lista << "p";
		else 
			f_lista << mapTileMatrix->at(i)->getZoneNumber();

		if((i+1) % SCREEN_WIDTH == 0)
			f_lista << "|";
		else
			f_lista << " ";
		if((i+1) % tileWorldSizeW == 0)
		{
			f_lista << endl;
			if(((i+1) / tileWorldSizeW) % SCREEN_HEIGHT == 0)
			{
				for(int j = 0; j < tileWorldSizeW-1; j++)
					f_lista << "--";
				f_lista << "-" << endl;
			}
		}
	}

	f_lista.close();
}

void Overworld::guardamePant(string path){

	string fichero (path);
	ofstream f_lista (fichero.c_str());
	if (!f_lista) {
		cout << "El fichero " << fichero << " no existe.";
		exit (0);
	}

	for(int i = 0; i < screenList->size(); i++)
	{
		f_lista << screenList->at(i)->getZoneNum() << " ";
		if((i+1) % (tileWorldSizeW/SCREEN_WIDTH) == 0)
			f_lista << endl;
	}

	f_lista.close();
}