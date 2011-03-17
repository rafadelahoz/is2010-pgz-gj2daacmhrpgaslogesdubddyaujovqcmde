#pragma once

#ifndef _GEN_OVERWORLD_H_
#define _GEN_OVERWORLD_H_

#include <vector>
#include <string>
#include <set>
#include <time.h>


#include "Zone.h"
#include "DBInterface.h"
#include "GenTypes.h"

using namespace std;

// Clase principal del m�dulo generador de mundos.
/*
	Clase principal del m�dulo generador de mundos.
	Describe la morfolog�a del mundo en creaci�n en base a sus zonas, las formas de estas y los caminos que las atraviesan.
	Adem�s, dispone de los m�todos principales que determinar� el mundo resultante.
*/

class GenOverworld {
	private:
		vector<Zone*>* zones;						// Lista de zonas del overworld
		vector<set<Point> >* blockadeVerts;			// Lista de conjuntos de puntos por donde el main road ha atravesado las fronteras.
		vector<Point>* mainRoadVerts;				// Almacena la estructura del camino principal
		vector<Point>* interestingPoints;			// Lista de los puntos interesantes que generar� genRoadRamifications()
		Overworld* overworld;						// Puntero al overworld que modifica

		OwScreen* makeNewScreen(int iniT, int screenNumber);
		int checkTileinZone(MapTile* mTile);

	public:

		// Constructora
		GenOverworld(Overworld* overworld);

		// Destructora
		~GenOverworld();

		// Divide el mapa en tantas zonas como le indique overworld
		void genFrontiers();

		// Modifica las zonas generadas por genFrontiers() para darles una forma m�s poligonal / atractiva / menos random.
		// Modifica mapTileMatrix.
		void genShape();

		// Asigna cada MapTile de mapTileMatrix a su zona, adem�s crea OwScreen tambien asignando su zona.
		void assignTilesScreens();

		// Coloca grandes bloques s�lidos
		void genGeoDetail();

		// Elige el tile a colocar en los huecos libres del overworld. Esto puede realizarse mediante capas.
		void genDecoration(DBInterface* myDB);

		// Encapsula el propio m�todo de la clase Zone
		void placeDungeons();

		// An�logo a placeDungeons()
		void placeSafeZones();

		// Conecta las zonas que componen el mapa seg�n el orden de las mazmorras que contienen
		void genMainRoad();

		//Genera caminos desde el camino principal perforando bloques (como pueden ser monta�as o bosques) y almacena
		//los posibles puntos interesantes. �stos son los ultimos puntos perforados de los caminos �interesantes�
		//(puede considerarse un camino interesante aquel camino que sea largo, o que haya perforado mucho en un bosque...).
		//Esta lista de puntos interesantes se utilizar� posteriormente para colocar coleccionables o cualquier otra
		//cosa que indicara el generador de vida.
		void genRoadRamifications();

		// Bloquea los caminos que conectan zonas
		void genBlockades();

		// Invoca al genScreens() de las Zonas.
		void genScreens();
};


#endif
