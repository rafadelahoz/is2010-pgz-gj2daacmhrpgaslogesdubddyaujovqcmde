#pragma once

#ifndef _DUNGEONJ_H_
#define _DUNGEONJ_H_

#include "Dungeon.h"
#include "DunScreen.h"
#include <vector>
#include "IGraphControl.h"
#include "DBManager.h"

// no logr� crear un array de enums <.<
#define ENTRANCE 0
#define PUZZLE 1
#define MINIBOSS 2
#define COLLECTABLE 3
#define BOSS 4

using namespace std;

// Clase hija de Dungeon
class DungeonJ: public Dungeon {
	
	private:
		// Informaci�n relacionada con el generador de layout
		int ** layout;
		int nZones;
		int* dist;	// asignaci�n de elementos a zonas

		// Genera el layout principal de la mazmorra, quedando determinada la siguiente informaci�n:
		// - Habitaciones transitables y conexiones entre ellas
		// - Habitaci�n inicial
		// - Habitaci�n final
		void genLayout();

		// Nada
		void blockPath();

		// Distribuye por las zonas de la mazmora lo siguiente:
		//	- objeto clave 
		//	- herramienta
		//	- puzzles 
		//	- minibosses (si los hay)
		//	- boss
		void placeItems();

		DunScreen* findScreen(int x, int y);

		int countRooms(int** layout);
		
	public:
		// Constructora, llama a la constructora de la superclase (que no deber�a hacer mucho)
		DungeonJ(string zone, string theme, int gameDiff, int dungNumber, int ratio, short tool, DBManager* db);
		~DungeonJ();
		
		// Ejecuta la generaci�n de la mazmorra, invocando secuencialmente a:
		// 	- genLayout()
		//	- blockPath()
		//	- placeItems()
		//	- genScreen()
		void generate();
};

#endif