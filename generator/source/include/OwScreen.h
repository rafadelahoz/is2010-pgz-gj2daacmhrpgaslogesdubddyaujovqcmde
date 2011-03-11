#pragma once

#ifndef _OWSCREEN_H_
#define _OWSCREEN_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "MapTile.h"

using namespace std;

//! Clase que se encarga de colocar los detalles mas peque�os y los enemigos en la 
//! pantalla y de generar los archivos de texto que ser�n leidos por el int�rprete.

class OwScreen{

	private:
		//! Atributos de la clase OwScreen.
		vector<MapTile>* matrix;	// Vector con todos los tiles de la pantalla
		ZoneInfo* infoZone;			// Guarda la informaci�n de zona
		
	public:
		
		// Constructora. Guarda en infoZone la informaci�n de zona pasada por par�metro
		OwScreen(ZoneInfo zonesInfo);
		
		//! Destructora
		~OwScreen();
		
		//! Crea un vector dependiendo del atributo infoZone de los posibles detalles a colocar.
		//! Coloca detalles en la pantalla ya sean arboles, farolas ...
		//! Almacena el resultado en matrix
		void placeDetails();
		
		//! Crea un vector dependiendo del atributo infoZone de los posibles enemigos a colocar.
		//! Coloca enemigos gen�ricos en la pantalla
		//! Almacena el resultado en matrix
		void placeEnemies();
		
		//! Devuelve el path del archivo con toda la informaci�n de la pantalla que cargar� el int�rprete.
		string createScreenFiles();
		
};


#endif // _ZONE_H_