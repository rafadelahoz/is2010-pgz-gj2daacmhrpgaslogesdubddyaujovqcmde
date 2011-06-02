/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by helping making it better or by playing it in its actual state.					//
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 2, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.			//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

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
