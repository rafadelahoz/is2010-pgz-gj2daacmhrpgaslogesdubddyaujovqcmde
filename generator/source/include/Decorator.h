#pragma once

#ifndef _DECORATOR_H_
#define _DECORATOR_H_

#include <list>

#include "AutoTiler.h"
#include "Screen.h"
#include "OwScreen.h"

using namespace std;

class Decorator
{
	private:
		string zone;	// Tipo de zona en la que se encuentra la pantalla
		string theme;	// Tem�tica del juego
		AutoTiler* autoTiler;

		int** terrainIdMatrix;
		list<Decoration*> decorationList;

		// elimina las decoraciones de la lista
		void clearDecorations();

		// borra los terrenos de la matriz de terrenos
		void clearTerrains();

	public:
		// Constructora
		Decorator();

		// Destructora
		~Decorator();

		// Inicia el decorador con el tipo de zona, el tema del juego, y el tileset
		void init(string zone, string theme, string tileSetPath);

		// Decora la pantalla pasada por par�metro (en funci�n de la incializaci�n anterior)
		void decorate(Screen* screen);

		// Elige un tile adecuado para un TiledPushable, por ejemplo
		short gimmeTile();
		// Elige un tile adecuado para un FloorButton
		short gimmeFloorButton();
		// Devuelve true si se puede colocar, sino false
		bool checkDecoCollision(Decoration* d);
};

#endif
