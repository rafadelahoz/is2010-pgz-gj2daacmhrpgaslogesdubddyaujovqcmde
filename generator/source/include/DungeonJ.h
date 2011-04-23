#pragma once

#ifndef _DUNGEONJ_H_
#define _DUNGEONJ_H_

#include "Dungeon.h"
#include "DunScreen.h"
#include <vector>
#include "IGraphControl.h"
#include "DBManager.h"


#define ENEMY_NUMBER 16
#define LONG(x1,y1,x2,y2) std::abs(x1-x2) + std::abs(y1-y2)

using namespace std;

typedef enum zoneType{ ENTRANCE, PUZZLE, MINIBOSS, COLLECTABLE, BOSS, BOSSS_KEY};

// Clase hija de Dungeon
class DungeonJ: public Dungeon {
	
	private:
		// Informaci�n relacionada con el generador de layout
		int** layout;
		int nZones; // n�mero de zonas de la mazmorra.
		zoneType* dist;	// asignaci�n de elementos a zonas
		int* enemies; // distribuci�n de enemigos por zonas
		int n_enemies; // n�mero de enemigos en toda la mazmorra.
		
		int idLock; // contador para la asignaci�n de identificadores a bloqueos

		// Genera el layout principal de la mazmorra, quedando determinada la siguiente informaci�n:
		// - Habitaciones transitables y conexiones entre ellas
		// - Habitaci�n inicial
		// - Habitaci�n final
		void genLayout();

		// Nada
		void blockPath();

		// Distribuye a nivel de zona.
		//	- objeto clave 
		//	- herramienta
		//	- puzzles 
		//	- minibosses (si los hay)
		//	- boss
		void zoneAllocations();

		// Distribuye a nivel de pantalla:
		//	- objeto clave 
		//	- herramienta
		//	- puzzles 
		//	- minibosses (si los hay)
		//	- boss
		void placeItems();

		DunScreen* findScreen(int x, int y);

		//Cuenta las pantallas de la mazmorra
		int countRooms(int** layout);

		// Calcula la cantidad de enemigos para una pantalla a partir de su zona
		int getEnemies(int zone);

		// Coloca las llaves de la mazmorra
		void placeKeys(int zone);

		// Coloca el jefe, la habitaci�n del objeto clave y el bloqueo del jefe
		void placeBoss();

		// Dada una pantalla comprueba si tiene elementos
		bool checkElement(Screen* s);
		
		// Comprueba si el layout es correcto empleando checkzone
		bool checkDungeon(int** layout);

		// Comprueba si no existen zonas mal colocadas
		bool checkZone(int** layout, int x, int y);

		// asigna elementos importantes(llave boss, puzzle, colectable, boss, entrada...) a zonas
		void placeZoneItems();

	public:
		// Constructora, llama a la constructora de la superclase (que no deber�a hacer mucho)
		DungeonJ(string zone, string theme, int gameDiff, int dungNumber, int ratio, short tool, short keyObj, DBManager* db);
		~DungeonJ();
		
		// Ejecuta la generaci�n de la mazmorra, invocando secuencialmente a:
		// 	- genLayout()
		//	- blockPath()
		//	- placeItems()
		//	- genScreen()
		void generate();

		void genTable(int dungeonNumber,int gameDiff, double ratio);

};

#endif