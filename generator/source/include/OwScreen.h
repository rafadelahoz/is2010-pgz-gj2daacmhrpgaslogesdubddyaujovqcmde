#pragma once

#ifndef _OWSCREEN_H_
#define _OWSCREEN_H_

#include <string.h>
#include <fstream>
#include <vector>
#include "MapTile.h"
#include "GenTypes.h"
//#include "Overworld.h"

using namespace std;

// Clase que se encarga de colocar los detalles mas peque�os y los enemigos en la
// pantalla y de generar los archivos de texto que ser�n leidos por el int�rprete.

class OwScreen{

	private:
		// Atributos de la clase OwScreen.
		vector<MapTile*>* matrix;		// Vector con todos los tiles de la pantalla
		vector<int>* enemyList;		// Vector con los enemigos disponibles de la zona
		vector<MapTile*>* detailsList;	// Vector con los detalles disponibles de la zona
		int screenNumber; //N�mero de pantalla dentro del overworld

	public:

		int zoneId;	// id de la zona a la que pertenece

		// Constructora.
		OwScreen(int screenN, vector<MapTile*>* screenMatrix, int zoneNum);

		// Destructora
		~OwScreen();

		// Coloca detalles en la pantalla ya sean arboles, farolas ...
		// Almacena el resultado en matrix
		void placeDetails();

		// Coloca enemigos gen�ricos en la pantalla
		// Almacena el resultado en matrix
		void placeEnemies();

		// Devuelve el path del archivo con toda la informaci�n de la pantalla que cargar� el int�rprete.
		string createScreenFiles();

		void writeInFile(const string& cad, ofstream& sal)const;

		// Setters
		void setEnemyList(vector<int>* lEnemy);
		void setDetailsList(vector<MapTile*>* lDetail);

		int getScreenNumber();
};

#endif
