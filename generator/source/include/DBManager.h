#pragma once

#ifndef _DBMANAGER_H_
#define _DBMANAGER_H

#define DB_PATH "./PGZ.db"
#define MAX_STR_LENGTH 255

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <set>
#include <vector>

#include "DefTypes.h"			// Incluye los structs que de datos que formar�n la BDJ

using namespace std;

typedef enum entityEffect { NONE, HP, MP, MONEY, KEY, HPMAX, MPMAX, BOSSKEY };

class DBManager {
	private:
		sqlite3* db;					// Puntero a la base de datos a tratar
		bool db_status;					// Estado de la base de datos tras ejecutar el �ltimo comando
		short keyGfxId;					// Id del gr�fico de las llaves que van a aparecer en el juego
		short bossKeyGfxId;				// Id del gr�fico de las llaves del jefe que van a aparercer en el juego
		
		// Datos usados hasta el momento que habr� que volcar a la BDJ
		set<enemy_t>* enemies;			// Conjunto de enemigos del juego
		set<npc_t>* npcs;				// Conjunto de NPCs del juego
		set<tool_t>* tools;				// Conjunto de herramientas del juego
		set<item_t>* items;				// Conjunto de �tems del juego
		set<item_t>* powUps;			// Conjunto de PowerUps del juego
		set<exchange_t>* exchange;		// Conjunto de objetos de intercambio del juego
		set<boss_t>* bosses;			// Conjunto de bosses del juego
		set<block_t>* blocks;			// Conjunto de bloqueos que aparecen en el juego
		set<worldGen_t>* worldGens;		// Conjunto de generadores de mundo
		//set<dungeonGen_t>* dungeonGens;	// Conjunto de generadores de mazmorra
		set<player_t>* players;			// Conjunto de players del juego

		vector<gfx_t>* graphics;		// Vector de gr�ficos usados en el juego
		vector<sfx_t>* sounds;			// Vector de sonidos usados en el juego

		short last_exchange;			// Id del �ltimo objeto de intercambio en la cadena

		int rowNumber(char* query);	// Devuelve el n�mero de filas que produce la consulta
		void savePlayers();			// Guarda el conjunto de players en el archivo de players de la BDJ
		void saveEnemies();			// Guarda el conjunto de enemigos en el archivo de enemigos de la BDJ
		void saveNPCs();			// Guarda el conjunto de npcs en el archivo de enemigos de la BDJ
		void saveTools();			// Guarda el conjunto de herramientas en el archivo de herramientas de la BDJ
		void saveItems();			// Guarda el conjunto de �tems en el archivo de enemigos de la BDJ
		void savePowUps();			// Guarda el conjunto de power ups en el archivo de power ups de la BDJ
		void saveExchange();		// Guarda el conjunto de objetos de intercambio en el archivo de objetos de intercambio de la BDJ
		void saveBosses();			// Guarda el conjunto de bosses en el archivo de bosses de la BDJ
		void saveBlocks();			// Guarda el conjunto de bloqueos en el archivo de bloqueos de la BDJ

		char* getPath(char* table, short id);	// Devuelve el path al archivo de la tabla 'table' con id 'id'

		void saveGfx();							// Guarda los archivos de gr�ficos en la BDJ
		void copyGfx();							// Copia los archivos de gr�ficos de la BDD a la BDJ
		void saveSfx();							// Guarda los archivos de sonido en la BDJ
		void copySfx();							// Copia los archivos de sonido de la BDD a la BDJ

	public:
		DBManager();
		~DBManager();

		short getEnemy(string zone, string theme);
		short getPowUp(string theme);
		short getZone(string theme);
		short getExchange(string theme);
		short getBlock(string theme, string zone, short tool);
		short getTool(string theme);
		short getItem(string zone);
		short getNPC(string zone, string theme);
		short getWorldGen(string theme);
		short getDungeonGen(string theme);
		short getPlayer(string theme);

		short getGfxId(char* table, short id);	// Devuelve el id del gr�fico de la fila de identificador 'id' de la tabla 'table'
		short getPowUpEffect(short id);			// Devuelve el efecto del power up de identificador 'id'
		short getKeyGfxId();					// Devuelve el id del gr�fico de la llave que se usa en el juego (tarea de decidator)
		short getBossKeyGfxId();				// Devuelve el id del gr�fico de la llave del boss que se usa en el juego

		void getKey(string theme);				// Obtiene la llave que se usar� en el juego
		void getBossKey(string theme);			// Obtiene la llave del jefe que se usar� en el juego

		void save();							// Vuelca todos los datos y archivos necesarios a la BDJ
};

#endif