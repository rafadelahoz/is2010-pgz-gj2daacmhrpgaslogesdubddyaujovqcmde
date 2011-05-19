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
	public:
		// Constructora
		WorldDecorator(DBManager* db);

		// Destructora
		~WorldDecorator();

		// Inicia el decorador con el tipo de zona, el tema del juego, y el tileset
		void init(string zone, string theme, short tileSetId);

		// Decora la pantalla pasada por par�metro (en funci�n de la incializaci�n anterior)
		virtual void decorate(Screen* screen);

		// Coloca decoraciones de manera sim�trica y devuelve el n�mero de espacios libres
		bool place_deco(Screen* s, Decoration::DecorationSize size, Decoration::DecorationType type, int pos);
};

#endif
