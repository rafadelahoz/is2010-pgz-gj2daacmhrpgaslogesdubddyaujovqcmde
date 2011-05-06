#pragma once

#ifndef _DECORATION_H_
#define _DECORATION_H_

#include <vector>
#include "Screen.h"

class Decoration
{
	public:

		enum DecorationSize {sNone, small, medium, big};
		enum DecorationType {tNone, solid, walkable, hangable};

		struct DecorationData
		{
			int idDeco;
			std::vector<int> compatibleTerrains; // id del terreno con el que puede ir (�til para hangables)
			int width, height;
			DecorationType type;
			DecorationSize size;
			std::vector<int> tileTypes;
			std::vector<int> tiles;
		};

	private:
		
		int x, y;
		DecorationData data;

	public:
		// Constructora
		Decoration(DecorationData data);
		void init(int x, int y);
		// Destructora
		virtual ~Decoration();

		// M�todo que devuelve las entidades (bueno no recuerdo bien, debe ser review by Rafa)
		std::vector<Entity*> toEntities();

		// Getter del data
		DecorationData getDecorationData();
};

#endif