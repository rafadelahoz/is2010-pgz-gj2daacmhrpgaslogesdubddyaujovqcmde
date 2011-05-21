#pragma once

#ifndef _DECORATOR_H_
#define _DECORATOR_H_

#include <list>

#include "AutoTiler.h"
#include "Screen.h"
#include "OwScreen.h"
#include "DBManager.h"

using namespace std;

class Decorator
{
	protected:

		string zone;	// Tipo de zona en la que se encuentra la pantalla
		string theme;	// Tem�tica del juego

		bool changedZone;	// booleano que nos dice cu�ndo se cambia de zona (para cambiar el estilo de decoraci�n o lo que sea)

		AutoTiler* autoTiler;
		DBManager* db;
		short idTileset;

		int** terrainIdMatrix;				// matriz de terrenos
		list<Decoration*> decorationList;	// lista de decoraciones

		// Devuelve true si se puede colocar, sino false
		bool checkDecoCollision(Decoration* d);
		
		// Comprueba si una decoraci�n se sale de la pantalla
		virtual bool isInBounds(Decoration* d, Screen* s);
		
		// Comprueba si hay algun s�lido en el espacio que ocupa la decoraci�n
		bool checkSolidCollision(Decoration* d, Screen* s);

		// Transforma los terrenos de la matriz a tiles
		void terrainsToTiles(Screen* screen);

		// Devuelve el n�mero de espacios libres de una pantalla
		int getFreeSpace(Screen* s);

		// Coloca decoraciones de manera sim�trica y devuelve el n�mero de espacios libres
		int place_symmetrics(Screen* s, int terrainId);

		// elimina las decoraciones de la lista
		void clearDecorations();

		// borra los terrenos de la matriz de terrenos
		void clearTerrains();

	public:
		// Constructora
		Decorator(DBManager* db);

		// Destructora
		~Decorator();

		// Inicia el decorador con el tipo de zona, el tema del juego, y el tileset
		virtual void init(string zone, string theme, short tileSetId);

		// Decora la pantalla pasada por par�metro (en funci�n de la incializaci�n anterior)
		virtual void decorate(Screen* screen){};

		// Elige un tile adecuado para un TiledPushable, por ejemplo
		short gimmeTile();
		// Elige un tile adecuado para un FloorButton
		short gimmeFloorButton();
};

#endif
