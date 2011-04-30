#pragma once

#ifndef _GENPUZZLE_H_
#define _GENPUZZLE_H_

#include <new>

#include "Screen.h"
#include "DunScreen.h"

typedef enum puzzle { pARENA};

class GenPuzzle{
	 /*
		El puzzle se modela como un conjunto de entidades, tanto los simples como los anidados, que volcamos 
		al vector entities de ds durante el metodo correspondiente en generate. 
		Se mezclar�a con los dem�s porque he supuesto que linkedto
		se utilizar�a para establecer la precedencia temporal entre las entidades, vamos que doorCloser y dooropener est�n linkedto arena y
		en funci�n de su estado actuan sin problemas con otras entidades que pueda haber en ese vector(?).
		Probablemente es prescindible el vector de puzzles, porque el identificador tambi�n lo llevan las entidades o eso he visto en el caso 
		de las puertas, adem�s el tipo ya no tiene sentido.

		Tambi�n he a�adido singleton. Aunque agreg� las entidades todav�a en Entity.h
	 */

	private:

		const static void* genPuzzle; // Controlamos la instanciaci�n de la clase con este puntero.

		short item;		// elemento que resulta tras resolver el puzzle.
		string zone;	// Zona en la que se encuentra la mazmorra.
		DBManager* db;

	public:

		GenPuzzle(DBManager* db);
		~GenPuzzle();

		// Sobrecarga de new para controlar la reserva de memoria
		static void* operator new (size_t size);

		// Genera el puzzle sobre ds con identificador id y tipo type
		void generate(DunScreen* ds, short id, short type);

		// Genera el puzzle arena sobre ds
		void enemyArena(DunScreen* ds); 
};

#endif