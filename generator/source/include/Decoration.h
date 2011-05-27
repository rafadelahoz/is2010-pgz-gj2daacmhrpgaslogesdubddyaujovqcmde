#pragma once

#ifndef _DECORATION_H_
#define _DECORATION_H_

#include <vector>
#include "Screen.h"

class Decoration
{
	public:

		enum DecorationSize {sNone, small, medium, big};
		enum DecorationType {tNone, solid, walkable, hangable, symmetric, dungeonStatue, dungeonEntrance, dunFloorEntrance, elementsFD};
		enum DecorationNear {nNone, Path, Water, Solid};

		struct DecorationData
		{
			int idDeco;
			std::vector<int> compatibleTerrains; // id del terreno con el que puede ir (�til para hangables)
			int width, height;
			DecorationType type;
			DecorationSize size;
			DecorationNear near;
			std::vector<int> tileTypes;	// vector que guarda los tipos de cada tile (guardados por filas)
			std::vector<int> tiles;		// vector que guarda el idenrificador de cada tile (guardados por filas)
		};

	private:
		DecorationData data;	// datos de la decoraci�n

	public:
		int x, y;	// posici�n (en tiles) de la decoraci�n en la pantalla

		// Constructora
		Decoration(DecorationData data);
		void init(int x, int y);
		// Destructora
		virtual ~Decoration();

		// M�todo que devuelve las entidades (bueno no recuerdo bien, debe ser review by Rafa)
		Entity* toEntities();

		// Getter del data
		DecorationData getDecorationData();
};

#endif