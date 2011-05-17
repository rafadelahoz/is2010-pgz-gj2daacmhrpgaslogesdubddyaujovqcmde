#pragma once

#ifndef _OWSCREEN_H_
#define _OWSCREEN_H_

#include <string.h>
#include <fstream>
#include <vector>
#include "MapTile.h"
#include "Screen.h"

using namespace std;


// Clase que se encarga de colocar los detalles mas peque�os y los enemigos en la
// pantalla y de generar los archivos de texto que ser�n leidos por el int�rprete.

class OwScreen : public Screen{

	private:
		// Atributos de la clase OwScreen.
		vector<MapTile*>* matrix;		// Vector con todos los tiles de la pantalla

		int screenNumber; //N�mero de pantalla dentro del overworld

		int zoneNum; //sirve para distinguir de las dem�s zonas y posible cambio a zoneNumber

		//bool contains(int elem, vector<int>* collect);	// Comprueba si un elemento est� en el vector
		//bool isThereAnyEntityAt(vector<Entity*>* entities, int pos);	// Comprueba si hay una entidad en la posici�n pos	
		

	public:

		// Constructora.
		OwScreen(short mapNumber, int screenN, vector<MapTile*>* screenMatrix, int zoneNum, short posX, short posY, short n_enemies, string zone, DBManager * db);

		// Destructora
		~OwScreen();

		// Coloca detalles en la pantalla ya sean arboles, farolas ...
		// Almacena el resultado en matrix
		void placeDetails();

		// Coloca enemigos gen�ricos en la pantalla
		// Almacena el resultado en matrix
		void placeEnemies();

		// Coloca NPCs en la pantalla
		void placeNPCs(int x, int y);

		//volcado de la info almacenada en las matrices de la clase padre.
		void generate();

		int getFreePos(vector<int>* posUsed);// Buscamos una posici�n random libre (sin entidades ni s�lidos)

		vector<MapTile*>* getMatrix();
		int getScreenNumber();
		int getZoneNum();
		short getSolid(int x, int y);
		short getSolid(int pos);
		void setSolid(int x, int y, short solid);
		void setSolid(int pos, short solid);
};

#endif
