#pragma once

#ifndef _DUNGEONAUTOTILER_H_
#define _DUNGEONAUTOTILER_H_

#include "AutoTiler.h"
#include <vector>
#include <map>
#include "DungeonWall.h"

	enum DunDecorationPos {top, left, right};
	struct DungeonDecoData{
		DunDecorationPos pos; // si es hangable (antorchas) nos dice en qu� pared ponerla
	};

class DungeonAutoTiler : public AutoTiler{
	
private:
		// mapa que guarda la 
		std::map<int, DungeonDecoData> dungeonDecos;

		// Carga la configuraci�n propia de la mazmorra
		bool loadDungeonConfig(FILE* file);

		// Carga todos los muros posibles
		bool loadWalls(FILE* file);

		// Carga todas las decoraciones posibles
		bool loadDunDecorationList(FILE* file);

		// Carga los datos de cada muro
		DungeonWall* loadWall(FILE* file);

		// Carga la lista de decoraciones de mazmorra
		bool loadDungeonDecoList(FILE* file);

		// Carga los datos de cada decoraci�n de mazmorra
		void loadDungeonDeco(FILE* file);

	public:

		// Constructora
		DungeonAutoTiler(std::string tileSetPath);
		// Destructora
		~DungeonAutoTiler();

		// devuelve la decoraci�n de una antorcha para esa direcci�n
		Decoration* getDungeonTorch(DunDecorationPos pos);
};

#endif