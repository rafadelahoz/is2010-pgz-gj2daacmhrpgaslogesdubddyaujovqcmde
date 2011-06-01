/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by helping making it better or by playing it in its actual state.					//
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

#ifndef _GENPUZZLE_H_
#define _GENPUZZLE_H_

#include <new>

#include "DunScreen.h"
#include "Decorator.h"

#define NPUZZLES 4

typedef enum puzzle { pARENA, pLINKEDARENA, pBUTTON, pBOSSARENA };

class DunScreen;

class GenPuzzle{
	 /*
		El puzzle se modela como un conjunto de entidades, tanto los simples como los anidados, que vamos a�adiendo al vector de entidades
		de la screen con linkedto al idPuzzle que a�adimos al vector de puzzles e idCollectable igual a la posici�n en el vector de entidades.

		En cuanto a enemigos e items estos estan linkedto a la entityPuzzleElement que corresponda y tienes como idCollectable -1. (no son persistentes?)
		
		Su uso es sencillo basta con indicar la screen, el idpuzzle (un contador similar a idLock) y el tipo de puzzle. 
		Generate incrementa el id de forma que no es necesario incrementar el contador con invocaci�n de genPuzzle->generate()
		sino que basta con guardar el retorno para tener el id con el que realizar la siguiente llamada a generate o para asignar
		a n_puzzles la cantidad definitiva de puzzles.

		Tambi�n he aplicado el patr�n singleton, pijer�as... ten�a que colarlo en alg�n sitio xD
	 */

	private:

		const static void* genPuzzle; // Controlamos la instanciaci�n de la clase con este puntero.

		short item; // Entidad que se genera tras resolver el puzzle.
		DBManager* db; // Puntero a la base de datos

		/* Robos a DunScreen */
		EntityItem* placeItem(DunScreen* ds, short linkedTo);

		void placeEnemies(DunScreen* ds, short linkedTo, int nEnemies);

		void addDoors(DunScreen* ds,int order);

		Decorator* decorator;

	public:

		GenPuzzle(short item, DBManager* db, string zone, string theme, short tileSetId);
		~GenPuzzle();

		/* Setters && Getters*/
		void setItem(short item);

		// Sobrecarga de new para controlar la reserva de memoria
		static void* operator new (size_t size);

		// Genera el puzzle sobre ds con identificador id y tipo type
		/*
			ds pantalla sobre la que se aplica el puzzle
			id identificador de conjuntos de entidades puzzle
			type tipo de puzzle a generar.
		*/
		short generate(DunScreen* ds, short id, short type);

		// Genera el puzzle arena sobre ds
		/*
			ds pantalla sobre la que se aplica el puzzle
			id para identificar el puzzle.
			linked si es cierto genera un nuevo puzzle que tendr� que resolverse antes que el que se acaba de generar
			persistent si item != -1, y es cierto genera recompensa con cada "subpuzzle" dentro de la secuencia de enlazados.
						si es falso solo en el �ltimo.
		*/
		void enemyArena(DunScreen* ds, bool linked, bool persistent, short& id);

		void bossArena(DunScreen* ds, bool linked, bool persistent, short& id); 
		// T�pico puzzle de pulsar un bot�n con un bloque
		void button(DunScreen* ds, bool linked, bool persistent, short& id);
		// Nos devuelve una posici�n no ocupada de la pantalla
		void get_valid_position(DunScreen* ds, int* x, int* y);
};

#endif