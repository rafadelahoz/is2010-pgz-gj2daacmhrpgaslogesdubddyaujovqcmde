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
		static const int NWALKABLES = 3;	// n�mero m�ximo de decoraciones "walkables" que puede colocar"

		bool place_upperTorchs(Screen* screen, int col, int row);
		bool place_leftTorchs(Screen* screen, int col, int row);
		bool place_rightTorchs(Screen* screen, int col, int row);

		bool place_upperTorch(Screen* screen, int col, int row);
		bool place_siderTorch(Screen* screen, int col, int row, DunDecorationPos pos);

		bool checkWallCollision(Decoration* d, Screen* screen);

		void place_FinalStatues(Screen* screen);
		void place_Stage(Screen* screen);

	protected:
		// Informaci�n de con qu� terrenos (y decoraciones) se est�n decorando las mazmorras
		struct screenInfo{
			short wallId;
			short terrainId;
			short torchId;
			short statueId;
		};

		// guarda informaci�n sobre los terrenos de la pantalla
		screenInfo info;

		// coloca "antorchas" en la parte superior, izquierda y derecha de la pantalla (si es posible)
		void place_torchs(Screen* screen);

		// coloca las estatuas por la pantalla si es posible
		void place_statues(Screen* screen);

		// coloca decoraciones por las que se pueda andar encima (m�ximo NWALKABLES)
		void place_walkables(Screen* screen);

		// coloca los terrenos y los muros
		void place_terrains(Screen* screen);

		// coloca las antorchas de la FS
		void place_torchsFS(Screen* screen);

	public:
		// Constructora
		DunDecorator(DBManager* db);

		// Destructora
		~DunDecorator();

		// Inicia el decorador con el tipo de zona, el tema del juego, y el tileset
		void init(string zone, short tileSetId);

		// Decora la pantalla pasada por par�metro (en funci�n de la incializaci�n anterior)
		void decorate(Screen* screen);

		// Decora la entrada de la mazmorra
		void decorateDunEntrance(Screen* screen, int col, int row);

		// Decora la pantalla pasada por par�metro como una Lock Screen
		void decorateLS(Screen* screen);

		// Decora la pantalla pasada por par�metro como una Final Screen
		void decorateFS(Screen* screen);
};

#endif
