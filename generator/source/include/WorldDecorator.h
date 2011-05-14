#pragma once

#ifndef _WORLDDECORATOR_H_
#define _WORLDDECORATOR_H_

#include <list>

#include "Decorator.h"
#include "WorldAutoTiler.h"
#include "Screen.h"
#include "OwScreen.h"
#include "DBManager.h"

using namespace std;

class WorldDecorator : public Decorator
{
	protected:

		// elimina las decoraciones de la lista
		void clearDecorations();

		// borra los terrenos de la matriz de terrenos
		void clearTerrains();

	public:
		// Constructora
		WorldDecorator(DBManager* db);

		// Destructora
		~WorldDecorator();

		// Inicia el decorador con el tipo de zona, el tema del juego, y el tileset
		void init(string zone, string theme, short tileSetId);

		// Decora la pantalla pasada por par�metro (en funci�n de la incializaci�n anterior)
		virtual void decorate(Screen* screen);

		// Transforma los terrenos de la matriz a tiles
		void terrainsToTiles(Screen* screen);

		// Elige un tile adecuado para un TiledPushable, por ejemplo
		short gimmeTile();
		// Elige un tile adecuado para un FloorButton
		short gimmeFloorButton();
		// Devuelve true si se puede colocar, sino false
		bool checkDecoCollision(Decoration* d);
		// Comprueba si una decoraci�n se sale de la pantalla
		bool isInBounds(Decoration* d, Screen* s);
		// Comprueba si hay algun s�lido en el espacio que ocupa la decoraci�n
		bool checkSolidCollision(Decoration* d, Screen* s);
};

#endif
