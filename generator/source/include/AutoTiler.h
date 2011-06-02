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

#ifndef _AUTOTILER_H_
#define _AUTOTILER_H_

#include "Terrain.h"
#include "AutoTerrain.h"
#include "NormalTerrain.h"
#include "Decoration.h"
#include <vector>

class AutoTiler
{
	protected:
//		string zone;						// Tipo de zona en la que se encuentra la pantalla	(No necesario)
//		string theme;						// Tem�tica del juego								(No necesario)
		std::vector<Terrain*> terrainList;		// Vector de terrenos
		std::vector<Decoration::DecorationData> decorationList;	// Vector de decoraciones

		// -------------------------- M�todos auxiliares ---------------------------------------------

		int findTerrain(int id); // devuelve la posici�n en la lista de terrenos del terreno indicado por id

		std::string getConfigurationFileName(std::string gfxPath);

		bool loadTerrainList(FILE* file);
		bool loadDecorationList(FILE* file);

		Terrain* loadTerrain(FILE* file);
		Decoration::DecorationData loadDecoration(FILE* file);
		bool loadDecorationTiles(FILE* file, int num, Decoration::DecorationData* dest);

		Decoration::DecorationSize computeSize(Decoration::DecorationData data);

		int chipsetWidth;

		// Devuelve el archivo abierto para el DungeonAutoTiler y WorldAutoTiler sigan leyendo datos
		FILE* loadTilesetConfig(std::string tilesetPath);

	public:
		// Constructora	
		AutoTiler();

		// Destructora
		~AutoTiler();

		//int getTerrain();
		virtual Terrain* getTerrain(int id);
		virtual int getTerrainId(Terrain::TerrainType type);
		//int getVariation(int id);
		virtual int getVariation(int id, Terrain::TerrainType type);

		//Decoration* getDecoration(mySize size);
		Decoration* getDecoration(int idDeco);
		Decoration* getDecoration(Decoration::DecorationType type, Decoration::DecorationSize size, int idTerrain, Decoration::DecorationNear near = Decoration::DecorationNear::nNone);
};

#endif