/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by helping making it better or by playing it in its actual state.					//
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 2, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.			//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

#include "OwScreen.h"

//Constructora en la que nos pasan la MapTileMatrix y tenemos que meter nosotros los tiles.

OwScreen::OwScreen(short mapNumber, int screenN, vector<MapTile*>* screenMatrix, int zoneNum, short posX, short posY, short n_enemies, string zone, DBManager * db)
	: Screen(mapNumber, posX, posY, n_enemies, zone, db){
	screenNumber = screenN;
	matrix = screenMatrix;
	n_enemies = rand()%3+3;
	this->zoneNum = zoneNum;
}

OwScreen::~OwScreen()
{
	delete matrix; //esto no borra los MapTile que contiene.
	matrix = NULL;
}

void OwScreen::placeDetails()
{
	//cout << "Ejecutando funcion <OwScreen::placeDetails()>" << endl;
}

/*bool OwScreen::isThereAnyEntityAt(vector<Entity*>* entities, int pos){
	for (int i = 0; i < (int)entities->size(); i++)
		if (pos == entities->at(i)->x + (entities->at(i)->y*SCREEN_WIDTH)) return true;

	return false;
}*/

void OwScreen::placeEnemies()
{
	//cout << "Ejecutando funcion <OwScreen::placeEnemies()>" << endl;
	int pos = 0;
	int posX;
	int posY;
	enemy myEnemy;
	int freePos = getNumFreeTiles();
	int numTotalEnemies = freePos / (SCREEN_WIDTH*SCREEN_HEIGHT / n_enemies);  //posiciones libres / Proporci�n enemigo-PosicionesLibres
	int Nenemies = numTotalEnemies;		// N�mero de enemigos de un mismo tipo de enemigos
	int idEnemy = 0;
	vector<int>* enemiesUsed = new vector<int>();
	vector<int>* posUsed;

	posUsed = getPosUsedEntities();

	for(int i = 0; i < SCREEN_WIDTH; i++)
	{
		posUsed->push_back(i); // borde de arriba
		posUsed->push_back(i+SCREEN_WIDTH);  // borde de arriba+1
		posUsed->push_back(i+((SCREEN_HEIGHT-1)*SCREEN_WIDTH)); //borde de abajo
		posUsed->push_back(i+((SCREEN_HEIGHT-2)*SCREEN_WIDTH)); //borde de abajo-1
		
	}
	for(int i = 0; i < SCREEN_HEIGHT; i++)
	{
		posUsed->push_back(i*SCREEN_WIDTH); //lateral izq
		posUsed->push_back(i*(SCREEN_WIDTH)+1); // lateral izq +1
		if(i > 0){
			posUsed->push_back((i*SCREEN_WIDTH)-1); //lateral der
			posUsed->push_back((i*SCREEN_WIDTH)-2);	//lateral der -1
		}
	}

	bool freePositions = true;

	while ( freePositions && (int)enemies->size() < numTotalEnemies ){
		// Elegimos el n�mero de enemigos que colocaremos de un tipo asegur�ndonos que pongamos por lo menos uno
		Nenemies = rand() % (numTotalEnemies - enemies->size()) + 1;

		// Cogemos el id del enemigo
		idEnemy = db->getEnemy(zone);
		//while(contains(idEnemy, enemiesUsed))
			//idEnemy = db->getEnemy(zone);
		enemiesUsed->push_back(idEnemy);
		
		// Colocamos el numero de enemigos calculado en enemies con identificador idEnemie por la pantalla
		for (int j = 0; j < Nenemies; j++){
			// Calculamos una posici�n random libre (sin entidades ni solidos)
			//do{
				pos = getFreePos(posUsed);			

			if (pos != -1){
				// Creamos el enemigo y fijamos sus atributos
				myEnemy.id = idEnemy;
				posX = pos % SCREEN_WIDTH;
				posY = pos / SCREEN_WIDTH;
				myEnemy.posX = posX;
				myEnemy.posY = posY;
				myEnemy.linkedTo = -1;
				// A�adimos el enemigo a la lista de los mismos
				enemies->push_back(myEnemy);
				// A�adimos la posici�n elegida al vector de posiciones utilizadas
				posUsed->push_back(pos);
			}
			else 
				freePositions = false;

		}
	}

	delete enemiesUsed;
	enemiesUsed = NULL;

	delete posUsed;
	posUsed = NULL;
}

int OwScreen::getFreePos(vector<int>* posUsed){
    int posFirst;
    int pos = -1;

    posFirst = rand() % matrix->size();

    pos = (posFirst + 1) % matrix->size();

    if ((matrix->at(posFirst)->getSolid() == 0 || matrix->at(posFirst)->getSolid() == 3) && (!isThereAnyEntityAt(entities, posFirst)) && !(contains(posFirst,posUsed)))
        return posFirst;
    else

        while ((pos != posFirst)
            && ( !(matrix->at(pos)->getSolid() == 0 || matrix->at(pos)->getSolid() == 3)
                || isThereAnyEntityAt(entities, pos)
				|| contains(pos,posUsed)))
            pos = (pos + 1) % matrix->size();

    if (pos == posFirst)
        return -1;
    else
        return pos;
}

int OwScreen::getScreenNumber(){
	return screenNumber;
}

void OwScreen::generate()
{
	int x, y;

	//volcamos nuestra info de los tiles en la clase padre
	for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++){
		x = i % SCREEN_WIDTH;
		y = i / SCREEN_WIDTH;
		//!!!!!!!!!!!!!!!!!!!!! CUIDADO!!  Cableado para que salgo algo bonito, cuando se termine decorator ha de descomentarse y borrar la siguiente !!!!!!!!!!!!!!!!!!!!!!!!!!!!!>
		//tiles[x][y] = matrix->at(i)->getTileId();
		//tiles[x][y] = matrix->at(i)->getSolid();
		solids[x][y] = matrix->at(i)->getSolid();
	}

}
//getters & setters

vector<MapTile*>* OwScreen::getMatrix()
{
	return matrix;
}

int OwScreen::getZoneNum()
{
	return zoneNum;
}

short OwScreen::getSolid(int x, int y)
{
	int pos;
	pos = (y * SCREEN_WIDTH) + x;
	if(pos < (int)matrix->size())
		return matrix->at(pos)->getSolid();
	else
		return -1;
}

short OwScreen::getSolid(int pos)
{
	if(pos < (int)matrix->size())
		return matrix->at(pos)->getSolid();
	else return -1;
}

void OwScreen::setSolid(int pos, short solid)
{
	if(pos < (int)matrix->size())
		matrix->at(pos)->setSolid(solid);
}

void OwScreen::setSolid(int x, int y, short solid)
{
	int pos;
	pos = (y * SCREEN_WIDTH) + x;
	if(pos < (int)matrix->size())
		matrix->at(pos)->setSolid(solid);
}

void OwScreen::placeNPCs(int x, int y){
	//cout << "Ejecutando funcion <OwScreen::placeNPCs()>" << endl;
	
	// Se deber�an de comprobar que la posiciones x e y no colisionara con otra entidad?? -> checkDecoCollision
	// idCollectable y linkedTo a -1? idText de donde lo saco?
	NPCInfo n = db->getNPC(zone);
	EntityNPC* myNPC = new EntityNPC(TILEDENTITY, x, y, -1, -1, n.gfxId, n.npcType, 0);
	addEntity(myNPC);
}

int OwScreen::getNumFreeTiles()
{
	int freePos = 0;
	for(int i = 0; i < matrix->size();i++)
		if (matrix->at(i)->getSolid() == 0 || matrix->at(i)->getSolid() == 3)
			freePos++;

	return freePos;
}

vector<int> * OwScreen::getPosUsedEntities()
{
	vector<int>* posUsed = new vector<int>();
	Entity* e;
	for(int i = 0; i < entities->size(); i++)
	{
		e = entities->at(i);
		e->type;
	}

	return posUsed;
}