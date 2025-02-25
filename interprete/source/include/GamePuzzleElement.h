/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by working on it and helping it be better or by playing it in it's actual state	//																			
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
#ifndef __GAMEPUZZLEELEMENT_H__
#define __GAMEPUZZLEELEMENT_H__

#include "GamePuzzle.h"

class GamePuzzleElement
{
protected:
	GamePuzzle* puzzle;
public:
	GamePuzzleElement();
	virtual ~GamePuzzleElement();
	virtual void init(GamePuzzle* puzzle);

	virtual void solvePuzzle();
	virtual void unsolvePuzzle();
	//! Cambia el valor del puzzle y devuelve el nuevo
	virtual bool switchPuzzle();
	virtual bool isPuzzleSolved();
};

#endif