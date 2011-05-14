#pragma once

#ifndef _DUNDECORATOR_H_
#define _DUNDECORATOR_H_

#include <list>

#include "Decorator.h"
#include "DungeonAutoTiler.h"
#include "Screen.h"
#include "OwScreen.h"
#include "DBManager.h"

using namespace std;

class DunDecorator : public Decorator
{
	private:

	public:
		// Constructora
		DunDecorator(DBManager* db);

		// Destructora
		~DunDecorator();

		// Inicia el decorador con el tipo de zona, el tema del juego, y el tileset
		void init(string zone, string theme, short tileSetId);

		// Decora la pantalla pasada por par�metro (en funci�n de la incializaci�n anterior)
		void decorate(Screen* screen);

		/*
		// Elige un tile adecuado para un TiledPushable, por ejemplo
		short gimmeTile();
		// Elige un tile adecuado para un FloorButton
		short gimmeFloorButton();
		// Devuelve true si se puede colocar, sino false
		bool checkDecoCollision(Decoration* d);*/
		// Comprueba si una decoraci�n se sale de la pantalla
		bool isInBounds(Decoration* d, Screen* s);/*
		// Comprueba si hay algun s�lido en el espacio que ocupa la decoraci�n
		bool Decorator::checkSolidCollision(Decoration* d, Screen* s);*/
};

#endif
