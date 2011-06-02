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
	private:
		static const int N_ATTEMPTS = 5; // n�mero de veces que va a intentar colocar una decoraci�n

		// coloca tantas decoraciones como indique nDecos, posUsed es el vector de posiciones ya utilizadas
		void place_decos(Screen* screen, int nDecos, Decoration::DecorationSize size, std::vector<int>* posUsed);

		// Devuelve la id de la decoraci�n que coloca
		int place_deco(Screen* s, Decoration::DecorationSize size, Decoration::DecorationType type,int idDeco, int pos);

	protected:

		// Informaci�n de con qu� terrenos (y decoraciones) se est�n decorando las mazmorras
		struct screenInfo{
			short solidId;
			short waterId;
			short pathId;
			short floorId;
		};

		// guarda informaci�n sobre los terrenos de la pantalla
		screenInfo info;

	public:

		// Constructora
		WorldDecorator(DBManager* db);

		// Destructora
		~WorldDecorator();

		// Inicia el decorador con el tipo de zona, el tema del juego, y el tileset
		void init(string zone, short tileSetId);

		// Decora la pantalla pasada por par�metro (en funci�n de la incializaci�n anterior)
		virtual void decorate(Screen* screen);

		// col, row es la posici�n del teletransporte
		void decorateDunEntrance(Screen* screen, int col, int row);
};

#endif
