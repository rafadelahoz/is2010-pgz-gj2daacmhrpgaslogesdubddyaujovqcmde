#pragma once

#ifndef _ZONE_H_
#define _ZONE_H_

#include <vector>
#include <queue>

#include "Overworld.h"
#include "GenDungeon.h"
#include "OwScreen.h"
#include "GenTypes.h"

#define BRUSHW 4
#define BRUSHH 4

using namespace std;

// Clase contenedor que delimita una zona del mundo mediante un pol�gono.
/*
	Con un conjunto de puntos, por afinidad, se determina la forma de la zona sobre se realizar� toda la funcionalidad asociada a la clase.
	Esto abarca la posibilidad de colocar zonas seguras, mazmorras y comprobar dadas unas coordenadas enteras de un tile en mapTileMatrix, su pertenencia
	a la zona.
*/

class Zone {

	private:
		// Atributos de la clase Zone.

		GPolygon* shape; //shape vector de coordenadas de tiles que definen un pol�gono para una zona del mundo.
		int typeId; //Tipo de la zona (bosque, desierto, etc...)
		int dungeonNumber; //N�mero de dungeon que toca a esta zona

		int zoneNumber; //N�mero de la zona dentro del overworld(internamente, para nosotros)
		int dungEntranceTile;
		
		// Mini-matriz
		vector<OwScreen*>* screenList;
		Overworld* overworld;

		bool isTileInZone(MapTile* tile);

		int genWormDetail(int screensPerRow);

		queue<int>* genVectorDirections();
		void iniBrush(int tile, int brush[BRUSHW][BRUSHH], int tilesPerRow);
		void placeSolids(int brush[BRUSHW][BRUSHH]);
		bool canMoveDirection(int direction, int brush[BRUSHW][BRUSHH], int tilesPerRow);
		void moveBrush(int nextDir, int brush[BRUSHW][BRUSHH], int tilesPerRow);

		int getTileOfScreen();
		bool isFrontierNear(int iniT);

		void sorrundTile(int pos, OwScreen* s, int id);
		

	public:
		// Construye un delimitador de zonas mediante un stl::vector indicando el tipo de la misma.
		/*
			shape vector de coordenadas de tile que definen un pol�gono para una zona del mundo.
			typeId tipo de zona.
		*/
		Zone(int zoneTypeId, GPolygon* s, Overworld* ow);

		// Destructora
		~Zone();

		// Devuelve el tipo de zona en forma de int.
		int getTypeId();

		// Permite modificar el tipo asociado a una zona.
		void setTypeId(int tId);

		// Devuelve el conjunto de puntos delimitador de zona.
		GPolygon* getShape();

		// Permite modificar el delimitador de zona.
		void setShape(GPolygon* s);

		// Comprueba si el tile pos en mapTileMatrix se encuentra en el pol�gono asociado a la zona y si es as� devuelve el MapTile correspondiente.
		/*
			pos coordenadas x e y de un tile en mapTileMatrix.
		*/
		MapTile inZone(GPoint pos);

		/* Permite colocar un mazmorra dentro de la zona de forma pseudo-aleatoria o bien mediante una posici�n especificada por par�metro.
			Se ha de tomar decisiones sobre la tool y el keyObject que se le proporcionar� al generador de mazmorras entre los conjuntos dados.
			As� mismo se determina la dificultad de la mazmorra mediante los par�metros gameDiff y dungNumber en este nivel.
		*/
		/*
			idTools conjunto de identificadores de herramienta. Se ha de seleccionar uno de ellos.
			dungNumber n�mero de mazmorras colocadas hasta el momento.
			gameDiff dificultad del juego.
			typeId tipo de la zona en la que se encuentra la mazmorra
			keyObjects conjunto de posibles objetos clave de la mazmorra a generar. Se ha de seleccionar uno de ellos.
			dungSize tama�o de la mazmorra (en n�mero de habitaciones aproximadas)
			ratePuzzleAction ratio puzzle/acci�n.
			idMiniBosses conjunto de minibosses que puede seleccionar el generador de mazmorras. Si es NULL no hay miniboss en la mazmorra.
			idBosses conjunto de bosses que puede seleccionar el generador de mazmorras.
			idEnemies conjunto de enemigos que puede seleccionar el generador de mazmorras.
			pos posici�n donde colocar la mazmorra en la zona.
		*/
		void placeDungeon(vector<int>* idTools,int dungNumber, int gameDiff,int typeId, vector<int>* keyObjects, int dungSize, int ratio,
			vector<int>* idBosses, vector<int>* idEnemies, vector<int>* idMiniBosses = NULL);

		// Por decidir, de primeras coloca la entrada a una zona segura.
		/*
			idZone tipo de zona segura.
			pos posici�n donde colocar la entrada a la zona segura. Si es NULL se coloca de forma pseudo-aleatoria.
		*/
		void placeSafeZone(int idZone,GPoint* pos=NULL);
		
		// Invoca el genScreens de cada OwScreen en nuestra screenList.
		void genScreens();

		//Creamos bloques grandes de Bloques Geol�gicos
		void genGeoDetail();

		// Devuelve el n�mero de orden de la mazmorra que se encuentra en la zona.
		int getDungeonNumber();

		// Establece un nuevo n�mero de orden de la mazmorra de la zona.
		void setDungeonNumber(int dunNum);

		// A�ade un OwScreen a screenList.
		void addScreen(OwScreen* ows);

		void genDetail();

		int getNumScreens();

		inline void setZoneNumber(int number){zoneNumber = number;}

		int getDungEntranceTile();
		int getZoneNumber();
};

#endif // _ZONE_H_
