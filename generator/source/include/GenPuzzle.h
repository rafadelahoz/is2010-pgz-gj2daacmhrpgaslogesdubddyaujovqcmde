#pragma once

#ifndef _GENPUZZLE_H_
#define _GENPUZZLE_H_

#include <new>

#include "Screen.h"
#include "DunScreen.h"

#define NPUZZLES 2

typedef enum puzzle { pARENA, pLINKEDARENA};

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
		/* ELIMINAR ID EN PROXIMO COMMIT */
		EntityItem* placeItem(DunScreen* ds, short id, short linkedTo);

		void placeEnemies(DunScreen* ds, short linkedTo);

	public:

		GenPuzzle(short item, DBManager* db);
		~GenPuzzle();

		/* Setters && Getters*/
		void setItem(short item);

		// Sobrecarga de new para controlar la reserva de memoria
		static void* operator new (size_t size);

		// Genera el puzzle sobre ds con identificador id y tipo type
		/*
			ds pantalla sobre la que se aplica el puzzle
			id necesario o no seg�n tengamos que identificar todas las entidades del puzzle como un todo o individualmente
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
};

#endif