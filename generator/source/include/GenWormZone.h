#pragma once

#ifndef _GEN_WORMZONE_H_
#define _GEN_WORMZONE_H_

#include <queue>

#include "GenZone.h"
#include "Screen.h"

#define BRUSHW 4
#define BRUSHH 4

using namespace std;

// Clase contenedor que delimita una zona del mundo mediante un pol�gono.
/*
	Con un conjunto de puntos, por afinidad, se determina la forma de la zona sobre se realizar� toda la funcionalidad asociada a la clase.
	Esto abarca la posibilidad de colocar zonas seguras, mazmorras y comprobar dadas unas coordenadas enteras de un tile en mapTileMatrix, su pertenencia
	a la zona.
*/

class GenWormZone : public GenZone {

	private:

		int genWormDetail();

		queue<int>* genVectorDirectionsRandom();
		queue<int>* genVectorDirectionsRight();
		queue<int>* genVectorDirectionsLeft();
		queue<int>* genVectorDirectionsDown();
		queue<int>* genVectorDirectionsUp();
		
		void iniBrush(int tile, int brush[BRUSHW][BRUSHH], int tilesPerRow);
		void placeSolids(int brush[BRUSHW][BRUSHH]);
		void placeSolids2(int brush[BRUSHW][BRUSHH], int lastMove);
		bool canMoveDirection(int direction, int brush[BRUSHW][BRUSHH], int tilesPerRow);
		void moveBrush(int nextDir, int brush[BRUSHW][BRUSHH], int tilesPerRow);
		void sorrundTile(int pos, OwScreen* s, int id);
		int getTileOfScreen();
		bool isFrontierNear(int iniT, int range);
		

	public:
		// Construye un delimitador de zonas mediante un stl::vector indicando el tipo de la misma.
		/*
			shape vector de coordenadas de tile que definen un pol�gono para una zona del mundo.
			typeId tipo de zona.
		*/
		GenWormZone(string zone, int zoneNumber, short tileSetId, Overworld* ow, short numEnemies,
					GenDungeon* genDungeon, short numDungeon, short idTool, short ratioDungeon, vector<SafeZoneInfo>* safeZones, Decorator* decorator, DBManager* myDB);

		// Destructora
		~GenWormZone();

		// Invoca el genScreens de cada OwScreen en nuestra screenList.
		void genScreens();

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
		void placeDungeon();

		void placeBlockades();

		// Por decidir, de primeras coloca la entrada a una zona segura.
		/*
			idZone tipo de zona segura.
			pos posici�n donde colocar la entrada a la zona segura. Si es NULL se coloca de forma pseudo-aleatoria.
		*/
		void placeSafeZone(int idZone,GPoint* pos=NULL);

		//Creamos bloques grandes de Bloques Geol�gicos
		void genGeoDetail();

		//Elige los tiles decorando la zona
		void genDetail();

};

#endif // _ZONE_H_
