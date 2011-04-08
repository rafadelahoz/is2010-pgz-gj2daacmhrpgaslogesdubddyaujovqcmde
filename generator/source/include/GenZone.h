#pragma once

#ifndef _GEN_ZONE_H_
#define _GEN_ZONE_H_

#include <vector>

#include "Overworld.h"
#include "GenDungeon.h"
#include "OwScreen.h"

using namespace std;

// Clase contenedor que delimita una zona del mundo mediante un pol�gono.
/*
	Con un conjunto de puntos, por afinidad, se determina la forma de la zona sobre se realizar� toda la funcionalidad asociada a la clase.
	Esto abarca la posibilidad de colocar zonas seguras, mazmorras y comprobar dadas unas coordenadas enteras de un tile en mapTileMatrix, su pertenencia
	a la zona.
*/

class GenZone {

	protected:
		// Atributos de la clase GenZone

		GPolygon* shape; //shape vector de coordenadas de tiles que definen un pol�gono para una zona del mundo.
		string theme; // tema del mundo.
		string zone; //Tipo de la zona (bosque, desierto, etc...)
		int dungeonNumber; //N�mero de dungeons colocados hasta el momento

		int zoneNumber; //N�mero de la zona dentro del overworld(internamente, para nosotros)
		int dungEntranceTile;

		// Mini-matriz
		vector<OwScreen*>* screenList;
		Overworld* overworld;

		bool isTileInZone(MapTile* tile);
		
		
	public:
		// Construye un delimitador de zonas mediante un stl::vector indicando el tipo de la misma.
		/*
			shape vector de coordenadas de tile que definen un pol�gono para una zona del mundo.
			typeId tipo de zona.
		*/
		GenZone(string theme, string zone, int zoneNumber, GPolygon* zoneShape, Overworld* ow){};

		// Destructora
		virtual ~GenZone(){};

		// Devuelve el tipo de zona en forma de int.
		virtual string getTheme() = 0;

		// Permite modificar el tipo asociado a una zona.
		virtual void setTheme(string tId) = 0;

		// Permite cambiar el tipo de la zona
		virtual string getZone() = 0;

		// Devuelve el conjunto de puntos delimitador de zona.
		virtual GPolygon* getShape() = 0;

		// Permite modificar el delimitador de zona.
		virtual void setShape(GPolygon* s) = 0;

		// Comprueba si el tile pos en mapTileMatrix se encuentra en el pol�gono asociado a la zona y si es as� devuelve el MapTile correspondiente.
		/*
			pos coordenadas x e y de un tile en mapTileMatrix.
		*/
		virtual MapTile inZone(GPoint pos) = 0;

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
		virtual void placeDungeon(vector<int>* idTools,int dungNumber, int gameDiff,int typeId, vector<int>* keyObjects, int dungSize, int ratio,
			vector<int>* idBosses, vector<int>* idEnemies, vector<int>* idMiniBosses = NULL) = 0;

		// Por decidir, de primeras coloca la entrada a una zona segura.
		/*
			idZone tipo de zona segura.
			pos posici�n donde colocar la entrada a la zona segura. Si es NULL se coloca de forma pseudo-aleatoria.
		*/
		virtual void placeSafeZone(int idZone,GPoint* pos=NULL) = 0;

		// Invoca el genScreens de cada OwScreen en nuestra screenList.
		virtual void genScreens() = 0;

		//Creamos bloques grandes de Bloques Geol�gicos
		virtual void genGeoDetail(int screensPerRow) = 0;

		//Elige los tiles decorando la zona
		virtual void genDetail() = 0;

		// Devuelve el n�mero de orden de la mazmorra que se encuentra en la zona.
		virtual int getDungeonNumber() = 0;

		// Establece un nuevo n�mero de orden de la mazmorra de la zona.
		virtual void setDungeonNumber(int dunNum) = 0;

		// A�ade un OwScreen a screenList.
		virtual void addScreen(OwScreen* ows) = 0;

		virtual int getNumScreens() = 0;

		virtual int getDungEntranceTile() = 0;
		virtual int getZoneNumber() = 0;
};

#endif
