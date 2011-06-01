/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//or by playing it in its actual state.												//
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 1, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA					//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

#pragma once

#ifndef _GEN_OVERWORLD_H_
#define _GEN_OVERWORLD_H_

#include <string>
#include <set>


#include "Overworld.h"
#include "GenZone.h"
#include "DBManager.h"
#include "WorldDecorator.h"

using namespace std;

// Clase principal del m�dulo generador de mundos.
/*
	Clase principal del m�dulo generador de mundos.
	Clase abstracta que hace el papel de abstract builder en el patr�n builder.
*/

class GenOverworld {
	protected:
		vector<GenZone*>* genZones;						// Lista de zonas del overworld
		vector<set<GPoint> >* blockadeVerts;			// Lista de conjuntos de puntos por donde el main road ha atravesado las fronteras.
		vector<GPoint>* mainRoadVerts;				// Almacena la estructura del camino principal
		vector<int>* mainRoadTiles;					// Almacena los tiles por los que pasa el mainRoad, para recorrerlo despu�s
		vector<GPoint>* interestingPoints;			// Lista de los puntos interesantes que generar� genRoadRamifications()
		Overworld* overworld;						// Puntero al overworld que modifica
		DBManager* myDB;							// Base de datos.
		Decorator* decorator;
		GenDungeon* genDungeon;

		virtual OwScreen* makeNewScreen(int iniT, int screenNumber) = 0;		
		virtual int checkTileinZone(MapTile* mTile) = 0;

	public:

		// Constructora
		GenOverworld(Overworld* overworld, vector<GenZone*>* genZones, GenDungeon* genDungeon, Decorator* decorator, DBManager* myDB);

		// Destructora
		virtual ~GenOverworld();

		// Divide el mapa en tantas zonas como le indique overworld
		virtual void genFrontiers() = 0;

		// Modifica las zonas generadas por genFrontiers() para darles una forma m�s poligonal / atractiva / menos random.
		// Modifica mapTileMatrix.
		virtual void genShape() = 0;

		// Asigna cada MapTile de mapTileMatrix a su zona, adem�s crea OwScreen tambien asignando su zona.
		virtual void assignTilesAndScreens() = 0;

		// llama al genGeoDetail de cada zona
		virtual void genGeoDetail() = 0;

		// Elige el tile a colocar en los huecos libres del overworld. Esto puede realizarse mediante capas.
		virtual void genDecoration(DBManager* myDB) = 0;

		// Encapsula el propio m�todo de la clase Zone
		virtual void placeDungeons() = 0;

		// An�logo a placeDungeons()
		virtual void placeSafeZones() = 0;

		// Conecta las zonas que componen el mapa seg�n el orden de las mazmorras que contienen
		virtual void genMainRoad() = 0;

		//Genera caminos desde el camino principal perforando bloques (como pueden ser monta�as o bosques) y almacena
		//los posibles puntos interesantes. �stos son los ultimos puntos perforados de los caminos �interesantes�
		//(puede considerarse un camino interesante aquel camino que sea largo, o que haya perforado mucho en un bosque...).
		//Esta lista de puntos interesantes se utilizar� posteriormente para colocar coleccionables o cualquier otra
		//cosa que indicara el generador de vida.
		virtual void genRoadRamifications() = 0;

		// Bloquea los caminos que conectan zonas
		virtual void genBlockades() = 0;
		//coloca los NPC's por el mundo
		virtual void placeNPCs() = 0;
		//coloca la mazmorra final
		virtual void placeFinalDungeon() = 0;
		// Coloca los Pigeons en los interestingPoints
		virtual void placePowUPandPigeons() = 0;

		// Invoca al genScreens() de las Zonas.
		virtual void genScreens() = 0;

		void guardameSolids(string path);
		void guardameZonas(string path);
		void guardamePant(string path);
		void guardameAutotile(string path);
};


#endif
