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
	private:
		// devuelve las coordenadas de la caja que representa la base de la decoraci�n
		void getSolidBox(Decoration* d, int* boxX, int* boxY, int* boxW, int* boxH);

	protected:

		string zone;	// Tipo de zona en la que se encuentra la pantalla

		bool changedZone;	// booleano que nos dice cu�ndo se cambia de zona (para cambiar el estilo de decoraci�n o lo que sea)

		AutoTiler* autoTiler;
		DBManager* db;
		short idTileset;

		int** terrainIdMatrix;				// matriz de terrenos
		list<Decoration*> decorationList;	// lista de decoraciones

		// recibe como par�metro la decoraci�n y devuelve la fila en la que se encuentra la base (s�lo v�lido para decoraciones de una fila de base)
		int getDecoSolidBase(Decoration* d);

		// Devuelve true si se puede colocar, sino false
		bool checkDecoCollision(Decoration* d);
		
		// Comprueba si una decoraci�n se sale de la pantalla
		virtual bool isInBounds(Decoration* d, Screen* s);
		
		// Comprueba si hay algun s�lido en el espacio que ocupa la decoraci�n
		bool checkSolidCollision(Decoration* d, Screen* s);

		// Comprueba que no choca con entidades
		bool checkEntitiesCollision(Decoration* d, Screen* s);

		// Comprueba que no bloquea caminos
		bool checkBlockingPath(Decoration* d, Screen* s);

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
		virtual void init(string zone, short tileSetId);

		// Decora la pantalla pasada por par�metro (en funci�n de la incializaci�n anterior)
		virtual void decorate(Screen* screen){};

		// Elige un tile adecuado para un TiledPushable, por ejemplo
		short gimmeTile();
		// Elige un tile adecuado para un FloorButton
		short gimmeFloorButton();
};

#endif
