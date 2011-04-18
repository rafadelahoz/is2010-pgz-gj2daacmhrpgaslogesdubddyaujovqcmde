#pragma once

#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include <vector>
#include <stdlib.h>
#include <time.h>
#include "DunScreen.h"

using namespace std;

/*
	Clase encargada de generar una mazmorra. Una vez generado el layout, colocadas las entidades que aparecer�n en ella,
	y generadas las pantallas de la misma, guarda un archivo con la informaci�n de la mazmorra. Este archivo ser� el que
	lea el m�dulo short�rprete posteriormente al ejecutar el juego.
*/

class Dungeon {
    protected:
        vector<Screen*>* screenList;	/* Lista de pantallas */
		short difficulty;				/* Dificultad del juego */
		short numDungeon;				/* N�mero de la mazmorra */
		short size;						/* Tama�o de la mazmorra (n� de pantallas) */
		short tool;						/* Identificador de la herramienta que va a estar en la mazmorra */
		short boss;						/* Identificador del boss que va a estar en la mazmorra */
		short miniboss;					/* Identificador del miniboss que va a estar en la mazmorra (-1 si no hay) */
		short keyItem;					/* Identificador del objeto clave que va a estar en la mazmorra */
		short ratio;					/* Ratio puzzle/acci�n */
		string zone;					// Zona a la que pertenece la mazmorra
		string theme;					// Tem�tica del juego
		short width, height;			// Dimensiones del layout
		short iniX, iniY;               // Posici�n de la pantalla inicial de la mazmorra
		DBManager* db;					// Puntero al manager de la base de datos

		// informaci�n sobre elementos persistentes de la mazmorra
		short n_puzzles;
		short n_collectables;
		short n_puertas;
		short n_minibosses;

	public:


		// Debe encargarse de realizar las operaciones con la BDI, para que las clases hijas se centren en generar la mazmorra
		Dungeon(string zone, string theme, short gameDiff, short dungNumber, short ratio, short tool, DBManager* db);

		// Destructora
		virtual ~Dungeon();

		virtual void generate() = 0;
		bool save();

		void index_collectables();
};


#endif
