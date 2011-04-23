#pragma once

#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <vector>
#include <string>
#include <stdlib.h>
#include "DBManager.h"
#include "Entity.h"

#define SCREEN_WIDTH 14		// Ancho de pantalla en n�mero de tiles
#define SCREEN_HEIGHT 12 	// Alto de la pantalla en n�mero de tiles
#define TILE_WIDTH 16		// Ancho del tile en p�xeles
#define TILE_HEIGHT 16		// Alto del tile en p�xeles

using namespace std;

struct enemy {
	short id;			// ID del enemigo en la BDJ
	short posX, posY;	// Posici�n del enemigo en la pantalla
	short linkedTo;		// idCollectable de la entidad a la que est� conectado
};

struct tileFG {		// Tile en foreground
	short id;			// identificador del tile
	short posX, posY;	// posici�n del tile en la pantalla
};

struct puzzle_t {
	short type;		// Tipo del puzzle
	short id;			// Id para identificar el puzzle dentro de la mazmorra
};

typedef enum entityType { DOOR, BOSSDOOR, ITEM, TILEDENTITY, DMGBLOCKADE, TILEDPUSHABLE, FLOORBUTTON, INSTANTIATOR, ABREDOORS, ARENA, TELEPORTATOR, TOOL };

class Screen {
	protected:
		short solids[SCREEN_WIDTH][SCREEN_HEIGHT]; // Disposici�n f�sica de la pantalla.
		short tiles[SCREEN_WIDTH][SCREEN_HEIGHT];  // Distribuci�n gr�fica de la pantalla.

		DBManager* db;			// Puntero al manager de la base de datos del generador

		short idTileSet; 		// identificador del tileSet que usa la pantalla
		short idMusic;			// identificador de la m�sica que suena en la pantalla

		short mapNumber;		// Numero de mapa al que pertence.
		short posIniX, posIniY;	// Posici�n inicial del personaje en la pantalla

		// Elementos que aparecen en la pantalla
		vector<Entity*>* entities; 	// Lista de entidades que aparecen en la pantalla
		vector<enemy>* enemies;		// Lista de enemigos que aparecen en la pantalla
		vector<tileFG>* fgTiles;		// Lista de tiles en foreground
		vector<puzzle_t>* puzzles;		// Lista de puzzles que aparecen en la pantalla

		short posX, posY;   // Coordenada en x, y de la pantalla en el mapa (ya sea de mundo o mazmorra)
		short n_enemies; 	// N�mero de enemigos que deber�a haber en la pantalla
		short n_entities;	// N�mero de entidades que hay en la pantalla
		short n_tilesFG; 	// N�mero de tiles en foreground de la pantalla
		short n_puzzles; 	// N�mero de puzzles de la pantalla

		string zone;	// Tipo de zona en la que se encuentra la pantalla
		string theme;	// Tem�tica del juego

		void saveFGTiles(FILE* file);	// guarda los tiles de foreground en el archivo file abierto
		void savePuzzles(FILE* file);	// guarda el n�mero y la lista de puzzles de la pantalla
		void saveEntities(FILE* file);	// guarda el n�mero y la lista de las entidades de la pantalla en el file abierto
		void saveEnemies(FILE* file);	// guarda el n�mero y la lista de los enemigos de la pantalla en file

	public:
		// Constructora de la pantalla
		/*
			mapNumber n�mero de mapa al que pertence esta screen.
			posX coordenada en x de la pantalla en el layout de la mazmorra.
			posY coordenada en y de la pantalla en el layout de la mazmorra.
			n_enemies es el n�mero de enemigos que debe aparecer en la pantalla
			zone y theme son la zona del juego a la que pertenece la pantalla y la tem�tica del juego 
			db es una referencia a la interfaz con la base de datos
		*/
		Screen(short mapNumber, short posX, short posY, short n_enemies, string zone, string theme, DBManager* db);

		// Destructora
		virtual ~Screen();

		// Guarda la informaci�n de la pantalla en un archivo
		bool save();

		virtual void generate() = 0;

        // Getters
		short getPosX();
		short getPosY();
		short getSolid(short x, short y);
		short getNEnemies();
		vector<Entity*>* getEntities();
};


#endif
